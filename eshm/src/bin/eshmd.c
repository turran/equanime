#include "Eshm.h"
#include "eshm_private.h"

#include <getopt.h>

/* TODO
 * handle command line parameters, to daemonize or not, etc
 * Use eina error info/error handling
 *
 */

/* TODO check if this structure can be shared with the one at the library */
typedef struct _Eshmd_Segment
{
	int shmid;
	int ref;
	Eina_Bool locked; /* TODO differentiate between a read lock and a write lock */
	Ecore_Con_Client *owner;
	size_t size;
} Eshmd_Segment;

typedef struct _Eshmd_Client
{
	Eina_List *segments;
} Eshmd_Client;

typedef struct _Eshmd
{
	Eina_Hash *hash;
	Eina_List *clients;

	Ecore_Con_Server *srv;
	void *buffer;
	int length;
} Eshmd;

Eshmd _eshmd;
int eshm_log_dom = -1;

static key_t _key_new(void)
{
	static num;
	char path[PATH_MAX];
	char *tmp;

	/* get the tmp dir */
	if (!(tmp = getenv("TMPDIR")))
	{
		tmp = strdup("/tmp");
	}
	snprintf(path, PATH_MAX, "%s/eshmd%d", tmp, num);
	return ftok(path, 'e');
}

/* Protocol message handlers */
static Eshm_Error msg_segment_new(Ecore_Con_Client *c, Eshm_Message_Segment_New *sn, void **reply)
{
	Eshmd_Segment *s;
	Eshm_Reply_Segment_New *rsn;
	struct shmid_ds ds;
	key_t key;

	INF("Requesting a new segment with id %s", sn->id);

	/* check if the segment already exists on the hash of segments */
	s = eina_hash_find(_eshmd.hash, sn->id);
	if (s)
	{
		INF("Segment with id %s already exists", sn->id);
		return ESHM_ERR_EXIST;
	}
	/* create a new segment */
	*reply = calloc(1, sizeof(Eshm_Reply_Segment_New));
	rsn = *reply;

	key = _key_new();
	rsn->shmid = shmget(key, sn->size, IPC_CREAT | 0);
	if (rsn->shmid < 0)
	{
		ERR("error getting the segment of name %s [%s] with key %d", sn->id, strerror(errno), key);
	}
	/* set the permissions */
	shmctl(rsn->shmid, IPC_STAT, &ds);
	ds.shm_perm.mode = 0666;
	shmctl(rsn->shmid, IPC_SET, &ds);

	s = calloc(1, sizeof(Eshmd_Segment));
	s->size = sn->size;
	s->shmid = rsn->shmid;
	s->ref++;
	s->owner = c;
	eina_hash_add(_eshmd.hash, sn->id, s);

	INF("New Segment created with id number %d", rsn->shmid);

	return ESHM_ERR_NONE;
}

static Eshm_Error msg_segment_get(Ecore_Con_Client *c, Eshm_Message_Segment_Get *sn, void **reply)
{
	Eshmd_Segment *s;
	Eshm_Reply_Segment_Get *rsn;

	INF("Requesting segment of name %s", sn->id);

	/* check if the segment already exists on the hash of segments */
	s = eina_hash_find(_eshmd.hash, sn->id);
	if (!s)
	{
		WRN("Segment not found");
		return ESHM_ERR_NEXIST;
	}
	s->ref++;
	/* get a new segment */
	*reply = calloc(1, sizeof(Eshm_Reply_Segment_New));
	rsn = *reply;
	rsn->shmid = s->shmid;
	rsn->size = s->size;

	return ESHM_ERR_NONE;
}

static int msg_segment_delete(Ecore_Con_Client *c, Eshm_Message_Segment_Delete *m, void **reply)
{
	/* decrement the segment reference count */
	/* delete the segment */
	return ESHM_ERR_NONE;
}

static int msg_segment_lock(Ecore_Con_Client *c, Eshm_Message_Segment_Lock *m, void **reply)
{
	Eshmd_Segment *s;

	/* lock the segment */
	INF("Locking the segment with id %s", m->id);
	s = eina_hash_find(_eshmd.hash, m->id);
	if (!s)
		return ESHM_ERR_NEXIST;
	else if (m->write && c != s->owner)
		return ESHM_ERR_ACCESS;
	/* if the request was for a write and the client is not the owner of
	 * the segment, return an error */
	return ESHM_ERR_NONE;
}

static int msg_segment_unlock(Ecore_Con_Client *c, Eshm_Message_Segment_Unlock *m, void **reply)
{
	/* unlock the segment */
	INF("Unlocking the segment with id %s", m->id);
	return ESHM_ERR_NONE;
}

static void help(void)
{
	printf("-d debug\n");
	printf("-b background\n");
}

int _client_add(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Add *e = event;
	Eshmd_Client *c;

	DBG("Client added %p", e->client);
	c = calloc(1, sizeof(Eshmd_Client));
	ecore_con_client_data_set(e->client, c);
}

int _client_del(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Add *e = event;
	Eshmd_Client *c;

	DBG("Client deleted %p", e->client);
	c = ecore_con_client_data_get(e->client);
	/* TODO unref all the segments */
}

int _client_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Data *cdata = event;
	Eshm_Message *m;
	void *body;
	void *reply = NULL;
	unsigned int m_length;
	Eshm_Error err;

	/* check if we got a full message */
	if (!_eshmd.buffer)
	{
		_eshmd.buffer = cdata->data;
		_eshmd.length = cdata->size;
		cdata->data = NULL;
	}
	else
	{
		_eshmd.buffer = realloc(_eshmd.buffer, _eshmd.length + cdata->size);
		memcpy(((unsigned char *)_eshmd.buffer) + _eshmd.length, cdata->data, cdata->size);
		_eshmd.length += cdata->size;
		cdata->data = NULL;
	}
	if (_eshmd.length < sizeof(Eshm_Message))
		return 0;
	/* ok, we have at least a message header */
	m = _eshmd.buffer;
	m_length = sizeof(Eshm_Message) + m->size;
	if (_eshmd.length < m_length)
		return 0;
	/* parse the header */
	DBG("Message received of type %d with msg num %d of size %d", m->type, m->id, m->size);

	body = eshm_message_decode(eshm_message_name_get(m->type), (unsigned char *)m + sizeof(Eshm_Message), m->size);
	if (!body)
	{
		ERR("Error Decoding");
		/* TODO check if the message needed a reply and if so
		 * send it the error number */
		goto shift;
	}
	/* check for the reply */
	/* TODO replace this with an array of function pointers */
	switch (m->type)
	{
		case ESHM_MSG_TYPE_SEGMENT_NEW:
			err = msg_segment_new(cdata->client, body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_LOCK:
			err = msg_segment_lock(cdata->client, body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_UNLOCK:
			err = msg_segment_unlock(cdata->client, body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_GET:
			err = msg_segment_get(cdata->client, body, &reply);
			break;
		case ESHM_MSG_TYPE_SEGMENT_DELETE:
			break;
	}
shift:
	if (eshm_message_reply_has(m->type) == EINA_TRUE)
	{
		Eshm_Reply r;
		Eshm_Message_Name rname;
		void *rbody;

		DBG("Sending reply");
		r.id = m->id;
		r.error = err;
		eshm_message_reply_name_get(m->type, &rname);
		DBG("Message encoded %d %d", rname, eshm_message_name_get(m->type));
		if (reply)
			rbody = eshm_message_encode(rname, reply, &r.size);
		else
			r.size = 0;
		ecore_con_client_send(cdata->client, &r, sizeof(Eshm_Reply));
		if (r.size)
			ecore_con_client_send(cdata->client, rbody, r.size);
	}
	/* free in case we have served a complete message */
	if (_eshmd.length > m_length)
	{
		unsigned char *tmp;
		unsigned int n_length;

		tmp = _eshmd.buffer;
		n_length = _eshmd.length - m_length;

		_eshmd.buffer = malloc(n_length);
		memcpy(_eshmd.buffer, tmp + m_length, n_length);
		free(tmp);
	}
	else
	{
		free(_eshmd.buffer);
		_eshmd.buffer = NULL;
	}
	return 0;
}


int main(int argc, char **argv)
{
	char *short_options = "bdl:";
	struct option long_options[] = {
		{"background", 0, 0, 'b'},
		{"debug", 0, 0, 'd'},
		{"log", 0, 0, 'l'},
		{0, 0, 0, 0}
	};
	int option;
	char c;
	int background = 0;
	int debug = 0;
	FILE *f;

	/* handle the parameters */
	while ((c = getopt_long(argc, argv, short_options, long_options,
	                                &option)) != -1)
	{
		switch (c)
		{
			case 'b':
				background = 1;
				break;
			case 'd':
				debug = 1;
				break;
			default:
				help();
				return 0;
		}
	}

	if (background)
	{
		int ret;

		ret = daemon(1, 1);
		printf("ret = %d\n", ret);
	}
	eina_init();
	ecore_init();
	ecore_con_init();

	eshm_log_dom = eina_log_domain_register("eshmd", NULL);

	if (!debug)
	{
		/* create a logging file */
		f = fopen("eshmd.log", "w");
		if (!f)
			goto err_log;
		eina_log_print_cb_set(eina_log_print_cb_file, f);
	}
	_eshmd.buffer = NULL;
	_eshmd.srv = ecore_con_server_add(ECORE_CON_LOCAL_USER, ESHMD_NAME, ESHMD_PORT, NULL);
	if (!_eshmd.srv)
	{
		ERR("Can't create the server");
		goto err_server;
	}
	_eshmd.hash = eina_hash_string_superfast_new(NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD, _client_add, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL, _client_del, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA, _client_data, NULL);

	eshm_message_init();
	ecore_main_loop_begin();
	eshm_message_shutdown();

err_server:
	if (!debug)
		fclose(f);
err_log:
	ecore_con_shutdown();
	ecore_shutdown();
	eina_shutdown();
	return 0;
}


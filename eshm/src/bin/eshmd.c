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
	Eix_Client *owner;
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

	Eix_Server *srv;
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
static Eshm_Error msg_segment_new(Eix_Client *c, Eshm_Message_Segment_New *sn, void **reply)
{
	Eshmd_Segment *s;
	Eshm_Reply_Segment_New *rsn;
	struct shmid_ds ds;
	key_t key;

	printf("segment new\n");
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

	printf("ok! %p\n", reply);
	return EIX_ERR_NONE;
}

static Eshm_Error msg_segment_get(Eix_Client *c, Eshm_Message_Segment_Get *sn, void **reply)
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

	return EIX_ERR_NONE;
}

static int msg_segment_delete(Eix_Client *c, Eshm_Message_Segment_Delete *m, void **reply)
{
	/* decrement the segment reference count */
	/* delete the segment */
	return EIX_ERR_NONE;
}

static int msg_segment_lock(Eix_Client *c, Eshm_Message_Segment_Lock *m, void **reply)
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
	return EIX_ERR_NONE;
}

static int msg_segment_unlock(Eix_Client *c, Eshm_Message_Segment_Unlock *m, void **reply)
{
	/* unlock the segment */
	INF("Unlocking the segment with id %s", m->id);
	return EIX_ERR_NONE;
}

static void help(void)
{
	printf("-d debug\n");
	printf("-b background\n");
}

int _client_add(void *data, int type, void *event)
{
	Eix_Event_Client_Add *e = event;
	Eshmd_Client *c;

	DBG("Client added %p", e->client);
	c = calloc(1, sizeof(Eshmd_Client));
	ecore_con_client_data_set(e->client, c);
}

int _client_del(void *data, int type, void *event)
{
	Eix_Event_Client_Add *e = event;
	Eshmd_Client *c;

	DBG("Client deleted %p", e->client);
	//c = ecore_con_client_data_get(e->client);
	/* TODO unref all the segments */
}

static int _server_process(Eix_Client *c, unsigned int type, void *msg,
		void **reply)
{
	int err = EIX_ERR_NONE;
	printf("processing msg %d\n", type);
	switch (type)
	{
		case ESHM_MSG_SEGMENT_NEW:
			err = msg_segment_new(c, msg, reply);
			break;
		case ESHM_MSG_SEGMENT_LOCK:
			err = msg_segment_lock(c, msg, reply);
			break;
		case ESHM_MSG_SEGMENT_UNLOCK:
			err = msg_segment_unlock(c, msg, reply);
			break;
		case ESHM_MSG_SEGMENT_GET:
			err = msg_segment_get(c, msg, reply);
			break;
		case ESHM_MSG_SEGMENT_DELETE:
			break;
		default:
			break;
	}
	return err;
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
	eshm_common_init();
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
	_eshmd.srv = eix_new(ESHMD_NAME, ESHMD_PORT, _server_process);
	eshm_common_server_setup(_eshmd.srv);
	if (!_eshmd.srv)
	{
		ERR("Can't create the server");
		printf("cannot\n");
		goto err_server;
	}
	_eshmd.hash = eina_hash_string_superfast_new(NULL);
	ecore_event_handler_add(EIX_EVENT_CLIENT_ADD, _client_add, NULL);
	ecore_event_handler_add(EIX_EVENT_CLIENT_DEL, _client_del, NULL);

	ecore_main_loop_begin();

err_server:
	if (!debug)
		fclose(f);
err_log:
	eshm_common_shutdown();
	return 0;
}

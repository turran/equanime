#include <string.h>

#include "Eix.h"
#include "Ecore.h"
#include "Ecore_Con.h"

#define ERR(...) EINA_LOG_DOM_ERR(_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_log_dom, __VA_ARGS__)

typedef struct _Eix
{
	Eina_Array *modules;
	Ecore_Con_Server *srv;
	void *buffer;
	int length;
} Eix;

static Eina_Array *_modules = NULL;
static Eix _equd;
static int _log_dom = -1;
char *options = NULL;
char *module = NULL;
Eina_Bool options_help = EINA_FALSE;
Eina_Bool modules_list = EINA_FALSE;

static int _client_add(void *data, int type, void *event)
{
	Eix_Client *client;
	Ecore_Con_Event_Client_Add *e = event;

	if (ecore_con_client_server_get(e->client) != _equd.srv)
		return ECORE_CALLBACK_RENEW;
	client = eix_client_new(e->client);
	ecore_con_client_data_set(e->client, client);
	return ECORE_CALLBACK_RENEW;
}

static int _client_del(void *data, int type, void *event)
{
	Eix_Client *client;
	Ecore_Con_Event_Client_Add *e = event;

	if (ecore_con_client_server_get(e->client) != _equd.srv)
		return ECORE_CALLBACK_RENEW;
	client = ecore_con_client_data_get(e->client);
	free(client);
	return ECORE_CALLBACK_RENEW;
}

static int _client_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Data *cdata = event;
	Eix_Client *c;
	Eix_Message *m;
	void *body;
	void *reply = NULL;
	unsigned int m_length;
	Eix_Error err;
	Eix *eq;
	/* FIXME */
	Ecore_Con_Client *cl = cdata->client; /* workaround for the event loop */

	printf("1 cdata %p %p\n", cl, cdata->client);
	if (ecore_con_client_server_get(cdata->client) != _equd.srv)
		return ECORE_CALLBACK_RENEW;
	c = ecore_con_client_data_get(cdata->client);
	if (!c) return ECORE_CALLBACK_RENEW;
	/* check if we got a full message */
	if (!_equd.buffer)
	{
		_equd.buffer = cdata->data;
		_equd.length = cdata->size;
		cdata->data = NULL;
	}
	else if (cdata->data)
	{
		_equd.buffer = realloc(_equd.buffer, _equd.length + cdata->size);
		memcpy(((unsigned char *)_equd.buffer) + _equd.length, cdata->data, cdata->size);
		_equd.length += cdata->size;
		cdata->data = NULL;
	}
	else
		return ECORE_CALLBACK_RENEW;
message:
	if (_equd.length < sizeof(Eix_Message))
		return ECORE_CALLBACK_RENEW;
	/* ok, we have at least a message header */
	m = _equd.buffer;
	m_length = sizeof(Eix_Message) + m->size;
	if (_equd.length < m_length)
		return ECORE_CALLBACK_RENEW;
	/* parse the header */
	DBG("Message received of type %d with msg num %d of size %d", m->type, m->id, m->size);
#if 0
	if (m->type > EIX_MSG_TYPE_SURFACE_DOWNLOADR) {
		ERR("Invalid message type %d", m->type);
		goto end;
	}
#endif

	body = eix_message_decode(eix_message_name_get(m->type), (unsigned char *)m + sizeof(Eix_Message), m->size);
	if (!body)
	{
		ERR("Error Decoding\n");
		/* TODO check if the message needed a reply and if so
		 * send it the error number */
		goto shift;
	}
	/* check for the reply */
	printf("2 cdata %p %p\n", cl, cdata->client);
	err = eix_client_process(c, eix_message_name_get(m->type), body, &reply);
	printf("3 cdata %p %p\n", cl, cdata->client);
shift:
	if (eix_message_reply_has(m->type) == EINA_TRUE)
	{
		Eix_Reply r;
		Eix_Message_Name rname;
		void *rbody;

		r.id = m->id;
		r.error = err;
		eix_message_reply_name_get(m->type, &rname);
		DBG("Encoding reply. type %d", m->type);
		if (reply)
			rbody = eix_message_encode(rname, reply, &r.size);
		else
			r.size = 0;
		DBG("Sending reply %d %d %d", r.id, r.error, r.size);
		printf("4 cdata %p %p\n", cl, cdata->client);
		ecore_con_client_send(cl, &r, sizeof(Eix_Reply));
		if (r.size)
			ecore_con_client_send(cl, rbody, r.size);
	}
end:
	/* free in case we have served a complete message */
	if (_equd.length > m_length)
	{
		unsigned char *tmp;
		unsigned int n_length;

		tmp = _equd.buffer;
		n_length = _equd.length - m_length;

		_equd.buffer = malloc(n_length);
		_equd.length = n_length;
		memcpy(_equd.buffer, tmp + m_length, n_length);
		free(tmp);
		goto message;
	}
	else
	{
		free(_equd.buffer);
		_equd.buffer = NULL;
	}
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _hosts_cb(Eix_Host *h, const char *name)
{
	if (modules_list)
		printf("%s\n", eix_host_name_get(h));
	else if (!strcmp(eix_host_name_get(h), name))
	{
		if (options_help)
		{
			Eix_Option *op;
			op = eix_host_options_get(h);
			if (!op) return;
			while (op->name != NULL)
			{
				printf("%s\t: %s (%s)\n", op->name, op->description, op->def);
				op++;
			}
		}
		else
		{
			eix_host_init(h, &_backend, options);
		}
	}
}


static void _server_init(void)
{
	Ecore_Con_Server *srv;

	_equd.srv = ecore_con_server_add(ECORE_CON_LOCAL_USER,
			EIX_NAME, EIX_PORT, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD, _client_add, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL, _client_del, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA, _client_data, &_equd);
	_equd.buffer = NULL;
	_log_dom = eina_log_domain_register("equd", NULL);
}

static void _server_shutdown(void)
{
	ecore_con_server_del(_equd.srv);
}

static void _server_setup(void)
{
	/* parse the cmd line options */
	eix_hosts_get(_hosts_cb, module);
}

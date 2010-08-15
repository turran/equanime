#include "Eix.h"
#include "eix_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ERR(...) EINA_LOG_DOM_ERR(_log, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_log, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_log, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_log, __VA_ARGS__)

/*
 * A message is composed of:
 * +----+------+------+-----------------
 * | id | type | size |
 * +----+------+------+-----------------
 * <-----header------>.<-----body------>
 */

typedef struct _Eix_Message
{
	unsigned int id; /* id of the message */
	unsigned int type; /* type of message */
	unsigned int size; /* size of the body */
} Eix_Message;

/*
 * A reply is composed of:
 * +----+-------+------+-----------------
 * | id | error | size |
 * +----+-------+------+-----------------
 * <------header------>.<-----body------>
 */

typedef struct _Eix_Reply
{
	unsigned int id; /* id of the message this reply replies to */
	unsigned int error; /* in case of any error set by the daemon */
	unsigned int size; /* size of the body */
} Eix_Reply;

typedef struct _Eix_Message_Descriptor
{
	unsigned int id;
	unsigned int reply_id;
	Eet_Data_Descriptor *edd;
	Eet_Data_Descriptor *reply_edd;
} Eix_Message_Descriptor;

struct _Eix_Server
{
	Ecore_Con_Server *svr;
	Eina_Hash *messages; /* an array to store the message description */
	Eix_Message *msg; /* last message sent */
	Eix_Reply *reply; /* reply for the last message sent */
	void *rbody; /* decoded body of the reply message */
	unsigned char *buffer; /* buffer where the data received from server is stored */
	unsigned int length; /* length of the buffer */
	Eix_Server_Process process;
	Eina_List *clients;
	int events;
	Eina_Bool delete_me;
	void *data;
	Ecore_Timer *timer;
};

struct _Eix_Client
{
	Eix_Server *server;
	Ecore_Con_Client *clnt;
	void *buffer;
	int length;
	int events;
	Eina_Bool delete_me;
	void *data;
};

static int _init = 0;
static int _log = -1;
static Eina_List *_servers = NULL;
static Ecore_Event_Handler *_handler[6];
static Eet_Data_Descriptor *_descriptors[2];
/******************************************************************************
 *                                  Helpers                                   *
 ******************************************************************************/
static inline Eina_Bool _server_exist(Ecore_Con_Server *s)
{
	return !!eina_list_data_find(_servers, ecore_con_server_data_get(s));
}

static inline Eina_Bool _client_server_exist(Ecore_Con_Client *c)
{
	return !!_server_exist(ecore_con_client_server_get(c));
}
/******************************************************************************
 *                               Message helpers                              *
 ******************************************************************************/
static inline Eina_Bool _has_reply(Eix_Message_Descriptor *desc)
{
	if (desc->reply_id)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}
static inline Eina_Bool _reply_type_get(Eix_Message_Descriptor *desc, int *n)
{
	if (_has_reply(desc) == EINA_FALSE)
		return EINA_FALSE;
	*n = desc->reply_id;
	return EINA_TRUE;
}

static int _client_process(Eix_Client *c, unsigned int type, void *msg,
		void **reply)
{
	Eix_Server *es;

	es = c->server;
	if (!es || !es->process) return EIX_ERR_NONE;

	if (type < EIX_MESSAGE_LAST)
	{
        	Eix_Reply_Sync *r;

		/* TODO whenever we implement async replies, we should flush all the pending events */
		r = *reply = calloc(1, sizeof(Eix_Reply_Sync));
		return EIX_ERR_NONE;
	}
	else return es->process(c, type, msg, reply);
}

static Eix_Message * _message_new(unsigned int type)
{
	static int id = 0;
	Eix_Message *m;

	m = malloc(sizeof(Eix_Message));
	m->id = id;
	m->type = type;
	/* TODO this will cause an overflow sometime */
	id++;

	return m;
}

static inline Eix_Message_Descriptor * _descriptor_get(Eix_Server *s, int type)
{
	return eina_hash_find(s->messages, (const void *)&type);
}

/******************************************************************************
 *                            Ecore Con Callbacks                             *
 ******************************************************************************/
static inline _client_event_free(Eix_Client *ec, void *ev)
{
	ec->events--;
	if (ec->delete_me && !ec->events)
		free(ec);
	free(ev);
}

static inline _server_event_free(Eix_Server *es, void *ev)
{
	es->events--;
	if (es->delete_me && !es->events)
		free(es);
	free(ev);
}

static void _client_add_free(void *data, void *ev)
{
	Eix_Event_Client_Add *event = ev;

	_client_event_free(event->client, ev);
}

static void _client_del_free(void *data, void *ev)
{
	Eix_Event_Client_Del *event = ev;

	_client_event_free(event->client, ev);
}

static void _server_add_free(void *data, void *ev)
{
	Eix_Event_Server_Add *event = ev;

	_server_event_free(event->server, ev);
}

static void _server_del_free(void *data, void *ev)
{
	Eix_Event_Server_Del *event = ev;

	_server_event_free(event->server, ev);
}

static Eina_Bool _client_add(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Add *e = event;
	Eix_Client *ec;
	Eix_Server *es;
	Eix_Event_Client_Add *ev;

	es = ecore_con_server_data_get(ecore_con_client_server_get(e->client));
	if (!eina_list_data_find(_servers, es)) return ECORE_CALLBACK_RENEW;

	ec = calloc(1, sizeof(Eix_Client));
	ec->server = es;
	ec->clnt = e->client;
	ecore_con_client_data_set(e->client, ec);
	ec->events++;

	DBG("New client %p connected to server %p", ec, es);
	ev = malloc(sizeof(Eix_Event_Client_Add));
	ev->client = ec;
	ecore_event_add(EIX_EVENT_CLIENT_ADD, ev, _client_add_free, NULL);

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _client_del(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Add *e = event;
	Eix_Event_Client_Del *ev;
	Eix_Client *ec;

	if (!_client_server_exist(e->client)) return ECORE_CALLBACK_RENEW;
	ec = ecore_con_client_data_get(e->client);
	ec->delete_me = EINA_TRUE;
	ec->events++;

	DBG("Client %p deleted from server %p", ec, ec->server);
	ev = malloc(sizeof(Eix_Event_Client_Del));
	ev->client = ec;
	ecore_event_add(EIX_EVENT_CLIENT_DEL, ev, _client_del_free, NULL);

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _client_data(void *data, int type, void *event)
{
	Eix_Message_Descriptor *desc;
	Ecore_Con_Event_Client_Data *e = event;
	Eix_Client *ec;
	Eix_Server *es;
	Eix_Message *m;
	void *body;
	void *reply = NULL;
	unsigned int m_length;
	Eix_Error err;

	es = ecore_con_server_data_get(ecore_con_client_server_get(e->client));
	if (!eina_list_data_find(_servers, es)) return ECORE_CALLBACK_RENEW;
	ec = ecore_con_client_data_get(e->client);

	DBG("Client %p sent data to server %p", ec, es);
	/* check if we got a full message */
	if (!ec->buffer)
	{
		ec->buffer = e->data;
		ec->length = e->size;
		e->data = NULL;
	}
	else if (e->data)
	{
		ec->buffer = realloc(ec->buffer, ec->length + e->size);
		memcpy(((unsigned char *)ec->buffer) + ec->length, e->data, e->size);
		ec->length += e->size;
		e->data = NULL;
	}
	else
		return ECORE_CALLBACK_RENEW;
message:
	if (ec->length < sizeof(Eix_Message))
		return ECORE_CALLBACK_RENEW;

	/* ok, we have at least a message header */
	m = ec->buffer;
	m_length = sizeof(Eix_Message) + m->size;
	if (ec->length < m_length)
		return ECORE_CALLBACK_RENEW;

	/* parse the header */
	DBG("Message received of type %d with msg num %d of size %d", m->type, m->id, m->size);
	desc = _descriptor_get(es, m->type);
	if (!desc)
	{
		ERR("Can not find a %d decoder", m->type);
		err = EIX_ERR_CODEC;
		goto shift;
	}
	body = eet_data_descriptor_decode(desc->edd, (unsigned char *)m + sizeof(Eix_Message), m->size);
	if (!body)
	{
		ERR("Error Decoding");
		err = EIX_ERR_CODEC;
		goto shift;
	}
	/* check for the reply */
	err = _client_process(ec, m->type, body, &reply);
shift:
	if (_has_reply(desc) == EINA_TRUE)
	{
		Eix_Reply r;
		void *rbody;
		int rtype;

		r.id = m->id;
		r.error = err;
		_reply_type_get(desc, &rtype);
		desc = _descriptor_get(es, rtype);
		if (!desc)
		{
			r.error = EIX_ERR_CODEC;
			/* TODO what happens with reply itself */
			r.size = 0;
			goto reply_end;
		}
		DBG("Encoding reply. type %d", m->type);
		if (reply)
			rbody = eet_data_descriptor_encode(desc->edd, reply, &r.size);
		else
			r.size = 0;
reply_end:
		DBG("Sending reply %d %d %d", r.id, r.error, r.size);
		ecore_con_client_send(e->client, &r, sizeof(Eix_Reply));
		if (r.size)
			ecore_con_client_send(e->client, rbody, r.size);
	}
end:
	/* free in case we have served a complete message */
	if (ec->length > m_length)
	{
		unsigned char *tmp;
		unsigned int n_length;

		tmp = ec->buffer;
		n_length = ec->length - m_length;

		ec->buffer = malloc(n_length);
		ec->length = n_length;
		memcpy(ec->buffer, tmp + m_length, n_length);
		free(tmp);
		goto message;
	}
	else
	{
		free(ec->buffer);
		ec->buffer = NULL;
	}
	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _server_add(void *data, int type, void *event)
{
	Ecore_Con_Event_Server_Add *e = event;
	Eix_Server *es;
	Eix_Event_Server_Add *ev;

	if (!_server_exist(e->server)) return ECORE_CALLBACK_RENEW;
	es = ecore_con_server_data_get(e->server);
	es->events++;

	DBG("Server %p added", es);
	ev = malloc(sizeof(Eix_Event_Server_Add));
	ev->server = es;
	ecore_event_add(EIX_EVENT_SERVER_ADD, ev, _server_add_free, NULL);

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _server_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Server_Data *e = event;
	Eix_Server *es;
	unsigned int m_length;

	if (!_server_exist(e->server)) return ECORE_CALLBACK_RENEW;
	es = ecore_con_server_data_get(e->server);
	DBG("Server %p received data", es);
	if (!es->msg)
	{
		ERR("How do we receive a reply with no msg first??\n");
		return ECORE_CALLBACK_RENEW;
	}
	if (!es->buffer)
	{
		es->buffer = e->data;
		es->length = e->size;
		e->data = NULL;
	}
	else
	{
		es->buffer = realloc(es->buffer, es->length + e->size);
		memcpy(((unsigned char *)es->buffer) + es->length, e->data, e->size);
		es->length += e->size;
		e->data = NULL;
	}
	if (es->length < sizeof(Eix_Reply))
		return ECORE_CALLBACK_RENEW;

	es->reply = (Eix_Reply *)es->buffer;
	m_length = sizeof(Eix_Reply) + es->reply->size;
	if (es->length < m_length)
		return ECORE_CALLBACK_RENEW;

	/* ok we have a full message */
	DBG("Reply received %d", es->reply->id);
	if (es->reply->size)
	{
		Eix_Message_Descriptor *desc;
		int rtype;

		desc = _descriptor_get(es, es->msg->type);
		if (!desc)
		{
			ERR("Not found a valid message %d", es->msg->type);
			goto end;
		}
		if (!_reply_type_get(desc, &rtype))
		{
			ERR("Not found a valid reply for %d", es->msg->type);
			goto end;
		}

		desc = _descriptor_get(es, desc->reply_id);
		DBG("Decoding data[%d] %d", rtype, es->reply->size);
		es->rbody = eet_data_descriptor_decode(desc->edd, es->buffer + sizeof(Eix_Reply), es->reply->size);
	}
	/* copy the reply into a new buffer */
	es->reply = malloc(sizeof(Eix_Reply));
	memcpy(es->reply, es->buffer, sizeof(Eix_Reply));
	/* realloc the tmp buffer */
	if (es->length > m_length)
	{
		unsigned char *tmp;
		unsigned int n_length;

		tmp = es->buffer;
		n_length = es->length - m_length;

		es->buffer = malloc(n_length);
		memcpy(es->buffer, tmp + m_length, n_length);
		free(tmp);
	}
	else
	{
end:
		free(es->buffer);
		es->buffer = NULL;
	}

	return ECORE_CALLBACK_RENEW;
}

static Eina_Bool _server_del(void *data, int type, void *event)
{
	Ecore_Con_Event_Server_Del *e = event;
	Eix_Event_Server_Del *ev;
	Eix_Server *es;

	if (!_server_exist(e->server)) return ECORE_CALLBACK_RENEW;
	es = ecore_con_server_data_get(e->server);
	
	es->delete_me = EINA_TRUE;
	es->events++;

	DBG("Server %p deleted", es);
	ev = malloc(sizeof(Eix_Event_Server_Del));
	ev->server = es;
	ecore_event_add(EIX_EVENT_SERVER_DEL, ev, _server_del_free, NULL);
}

static Eina_Bool _timeout_cb(void *data)
{
	Eix_Server *es = data;

	ecore_main_loop_quit();

	return ECORE_CALLBACK_CANCEL;
}

static Eina_Bool _idler_cb(void *data)
{
	Eix_Server *e = (Eix_Server *)data;

	if (e->reply)
	{
		if (e->timer)
		{
			ecore_timer_del(e->timer);
			e->timer = NULL;
		}
		ecore_main_loop_quit();
		return ECORE_CALLBACK_CANCEL;
	}
	return ECORE_CALLBACK_RENEW;
}

static Eix_Error _server_send(Eix_Server *e, Eix_Message_Descriptor *desc,
		Eix_Message *m, void *data,
		double timeout, void **rdata)
{
	Eix_Error error = EIX_ERR_NONE;
	Ecore_Idler *idler;
	//Ecore_Timer *timer;
	int ret;

	e->msg = m;
	DBG("Sending message of type %d and id %d", m->type, m->id);
	ret = ecore_con_server_send(e->svr, m, sizeof(Eix_Message));
	ret = ecore_con_server_send(e->svr, data, m->size);

	if (_has_reply(desc) == EINA_FALSE)
		goto no_reply;

	idler = ecore_idler_add(_idler_cb, e);
	if (timeout) e->timer = ecore_timer_add(timeout, _timeout_cb, e);
	/* wait for the response */
	ecore_main_loop_begin();

	if (!e->reply)
	{
		error = EIX_ERR_TIMEOUT;
		goto no_reply;
	}
	DBG("Finished lock reply of type %d", e->reply->id);

	error = e->reply->error;

	if (rdata)
		*rdata = e->rbody;
	free(e->reply);
no_reply:
	free(e->msg);
	e->msg = NULL;
	e->reply = NULL;

	return error;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
int EIX_EVENT_SERVER_ADD;
int EIX_EVENT_SERVER_DEL;
int EIX_EVENT_CLIENT_ADD;
int EIX_EVENT_CLIENT_DEL;

/**
 * Initialize the Eix library. You must call this function before any
 * other call to an Eix function.
 */
EAPI void eix_init(void)
{
	Eet_Data_Descriptor *edd;
	Eet_Data_Descriptor_Class eddc;

	if (_init) return;

	_init++;
	eina_init();
	eet_init();
	ecore_init();
	ecore_con_init();
	_log = eina_log_domain_register("eix", NULL);
	_handler[0] = ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA, _server_add, NULL);
	_handler[1] = ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA, _server_data, NULL);
	_handler[2] = ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DEL, _server_del, NULL);
	_handler[3] = ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD, _client_add, NULL);
	_handler[4] = ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL, _client_del, NULL);
	_handler[5] = ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA, _client_data, NULL);

	EIX_EVENT_CLIENT_ADD = ecore_event_type_new();
	EIX_EVENT_CLIENT_DEL = ecore_event_type_new();
	EIX_EVENT_SERVER_ADD = ecore_event_type_new();
	EIX_EVENT_SERVER_DEL = ecore_event_type_new();

	/* Core messages */
	/* sync */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eix_Message_Sync", sizeof(Eix_Message_Sync));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[0] = edd;
	/* sync */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eix_Reply_Sync", sizeof(Eix_Reply_Sync));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[1] = edd;
}
/**
 * Shutdowns the Eix library. Once you have finished using Equanime
 * call this function to deinitialize everything
 */
EAPI void eix_shutdown(void)
{
	if (_init == 1)
	{
		int i;

		/* TODO delete every server */
		for (i = 0; i < 6; i++)
		{
			ecore_event_handler_del(_handler[i]);
		}
		for (i = 0; i < 2; i++)
		{
			eet_data_descriptor_free(_descriptors[i]);
		}
		eina_log_domain_unregister(_log);
		ecore_con_shutdown();
		ecore_shutdown();
		eet_shutdown();
		eina_shutdown();
	}
	_init--;
}
/**
 * Connects to an Eix server
 * @param[in] name The name to use for the connection
 * @param[in] port The port to use for the connection
 * @return An Eix server in case the connection was succesfull or NULL
 * otherwise
 */
EAPI Eix_Server * eix_connect(const char *name, int port)
{
	Eix_Server *es;
	Ecore_Con_Server *svr;

	es = calloc(1, sizeof(Eix_Server));
	if (!es) return NULL;
	/* try to connect to the server */
	svr = ecore_con_server_connect(ECORE_CON_LOCAL_USER, name,
			port, es);
	if (!svr)
	{
		free(es);
		return NULL;
	}
	es->svr = svr;
	es->messages = eina_hash_int32_new(NULL);
	eix_server_message_add(es, EIX_MESSAGE_SYNC, _descriptors[0], EIX_REPLY_SYNC); 
	eix_server_message_add(es, EIX_REPLY_SYNC, _descriptors[1], 0); 
	_servers = eina_list_append(_servers, es);

	return es;
}
/**
 * Creates a new Eix server
 * @param[in] name The server's name
 * @param[in] port The server's port to use
 */
EAPI Eix_Server * eix_new(const char *name, int port,
		Eix_Server_Process process)
{
	Eix_Server *es;
	Ecore_Con_Server *svr;

	es = calloc(1, sizeof(Eix_Server));
	if (!es) return NULL;
	svr = ecore_con_server_add(ECORE_CON_LOCAL_USER,
			name, port, es);

	if (!svr)
	{
		free(es);
		return NULL;
	}

	es->svr = svr;
	es->messages = eina_hash_int32_new(NULL);
	es->process = process;
	eix_server_message_add(es, EIX_MESSAGE_SYNC, _descriptors[0], EIX_REPLY_SYNC); 
	eix_server_message_add(es, EIX_REPLY_SYNC, _descriptors[1], 0); 
	_servers = eina_list_append(_servers, es);

	return es;
}
/**
 *
 */
EAPI Eix_Error eix_message_server_send(Eix_Server *es, int type,
		void *data, double timeout, void **rdata)
{
	Eix_Message *m;
	void *body;
	Eix_Message_Descriptor *desc;

	desc = _descriptor_get(es, type);
	if (!desc || !desc->edd) return EIX_ERR_CODEC;

	m = _message_new(type);
	body = eet_data_descriptor_encode(desc->edd, data, &m->size);
	if (!body)
		return EIX_ERR_CODEC;

	return _server_send(es, desc, m, body, timeout, rdata);
}

/**
 * Flushes every message and waits until the server
 * has processed every message
 * @param[in] e Eix The equanime connection
 */
EAPI void eix_sync(Eix_Server *e)
{
	Eix_Message_Sync m;
	Eix_Reply_Sync *r = NULL;
	Eix_Error error;

	/* send the command to the server */
	error = eix_message_server_send(e, EIX_MESSAGE_SYNC, &m, 0, (void **)&r);
	if (error) return;
	free(r);
}

/**
 *
 */
EAPI void eix_client_data_set(Eix_Client *c, void *data)
{
	c->data = data;
}

/**
 *
 */
EAPI void * eix_client_data_get(Eix_Client *c)
{
	return c->data;
}

EAPI void eix_server_data_set(Eix_Server *s, void *data)
{
	s->data = data;
}

/**
 *
 */
EAPI void * eix_server_data_get(Eix_Server *s)
{
	return s->data;
}

/**
 *
 */
EAPI void eix_client_del(Eix_Client *c)
{
	if (c->delete_me) return;

	c->delete_me = 1;
	if (!c->events)
	{
		ecore_con_client_del(c->clnt);
		c->server->clients = eina_list_remove(c->server->clients, c);

		if (c->buffer) free(c->buffer);
		free(c);
	}
}

/**
 *
 */
EAPI Eix_Server * eix_client_server_get(Eix_Client *c)
{
	return c->server;
}

/**
 *
 */
EAPI void eix_server_del(Eix_Server *s)
{
	if (s->delete_me) return;

	s->delete_me = 1;
	if (!s->events)
	{
		Eix_Client *c;

		EINA_LIST_FREE(s->clients, c)
			eix_client_del(c);

		ecore_con_server_del(s->svr);
		_servers = eina_list_remove(_servers, s);

		if (s->buffer) free(s->buffer);
		free(s);
	}
}

/**
 *
 */
EAPI void eix_server_message_add(Eix_Server *e,
		unsigned int id, Eet_Data_Descriptor *edd,
		unsigned int reply_id)
{
	Eix_Message_Descriptor *mdesc;

	if (!e || !edd) return;

	mdesc = _descriptor_get(e, id);
	if (mdesc)
	{
		WRN("Adding a message with the same id %d", id);
		return;
	}
	mdesc = calloc(1, sizeof(Eix_Message_Descriptor));
	mdesc->id = id;
	mdesc->reply_id = reply_id;
	mdesc->edd = edd;

	eina_hash_add(e->messages, (const void *)&id, mdesc);
}

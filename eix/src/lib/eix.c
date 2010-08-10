#include "Eix.h"
#include "eix_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Eix_Message_Description
{

} Eix_Message_Description;

struct _Eix_Server
{
	Ecore_Con_Server *svr;
	Eina_Array *messages; /* an array to store the message description */
	Eix_Message *msg; /* last message sent */
	Eix_Reply *reply; /* reply for the last message sent */
	void *rbody; /* decoded body of the reply message */
	unsigned char *buffer; /* buffer where the data received from server is stored */
	unsigned int length; /* length of the buffer */
};

struct _Eix_Client
{
	Ecore_Con_Client *clnt;
	void *buffer;
	int length;
};

static int _init = 0;
static Eina_List *_servers = NULL;
static Ecore_Event_Handler *_handler[6];
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
 *                            Ecore Con Callbacks                             *
 ******************************************************************************/
static void _server_setup(Eix_Server *es, Ecore_Con_Server *svr)
{
	es->svr = svr;
	es->messages = eina_array_new(10);
	/* TODO add the messages */
	_servers = eina_list_append(_servers, es);
}

static void _client_setup(Eix_Client *ec, Ecore_Con_Client *clnt)
{
	ec->clnt = clnt;
	ecore_con_client_data_set(clnt, ec);
}

static int _client_add(void *data, int type, void *event)
{
	Eix_Client *ec;
	Ecore_Con_Event_Client_Add *e = event;

	if (!_client_server_exist(e->client)) return ECORE_CALLBACK_RENEW;

	ec = calloc(1, sizeof(Eix_Client));
	_client_setup(ec, e->client);

	return ECORE_CALLBACK_RENEW;
}

static int _client_del(void *data, int type, void *event)
{
	Eix_Client *ec;
	Ecore_Con_Event_Client_Add *e = event;

	if (!_client_server_exist(e->client)) return ECORE_CALLBACK_RENEW;
	ec = ecore_con_client_data_get(e->client);
	free(ec);

	return ECORE_CALLBACK_RENEW;
}

static int _client_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Data *e = event;
	Eix_Client *ec;
	Eix_Message *m;
	void *body;
	void *reply = NULL;
	unsigned int m_length;
	Eix_Error err;

	if (!_client_server_exist(e->client)) return ECORE_CALLBACK_RENEW;
	ec = ecore_con_client_data_get(e->client);
	if (!ec) return ECORE_CALLBACK_RENEW;

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

	body = eix_message_decode(eix_message_name_get(m->type), (unsigned char *)m + sizeof(Eix_Message), m->size);
	if (!body)
	{
		ERR("Error Decoding\n");
		/* TODO check if the message needed a reply and if so
		 * send it the error number */
		goto shift;
	}
	/* check for the reply */
	/* TODO */
	//err = eix_client_process(ec, eix_message_name_get(m->type), body, &reply);
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

static int _server_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Server_Data *e = event;
	Eix_Server *es;
	Eix_Message_Name rname;
	unsigned int m_length;

	if (!_server_exist(e->server)) return ECORE_CALLBACK_RENEW;
	es = ecore_con_server_data_get(e->server);
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
		eix_message_reply_name_get(es->msg->type, &rname);
		DBG("Decoding data[%d] %d", rname, es->reply->size);
		es->rbody = eix_message_decode(rname, es->buffer + sizeof(Eix_Reply), es->reply->size);
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
		free(es->buffer);
		es->buffer = NULL;
	}

	return ECORE_CALLBACK_RENEW;
}

static int _server_del(void *data, int type, void *event)
{
}

static int _timeout_cb(void *data)
{
	Eix_Server *es = data;

	printf("timer!!!\n");
	/* TODO set the reply to an error */
	return 0;
}

static Eix_Error _server_send(Eix_Server *e, Eix_Message *m, void *data,
		double timeout, void **rdata)
{
	Eix_Error error = EIX_ERR_NONE;
	Ecore_Timer *timer;
	int ret;

	e->msg = m;

	DBG("Sending message of type %d and id %d", m->type, m->id);
	ret = ecore_con_server_send(e->svr, m, sizeof(Eix_Message));
	ret = ecore_con_server_send(e->svr, data, m->size);

	if (eix_message_reply_has(m->type) == EINA_FALSE)
		goto no_reply;

	/* TODO register a timeout callback */
	if (timeout)
		timer = ecore_timer_add(timeout, _timeout_cb, e);
	/* wait for the response */
	while (!e->reply)
	{
		ecore_main_loop_iterate();
	}
	if (timeout)
		ecore_timer_del(timer);

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
int eix_log = -1;
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
int EIX_EVENT_SERVER_ADD;
int EIX_EVENT_SERVER_DEL;
int EIX_EVENT_SERVER_DATA;
int EIX_EVENT_CLIENT_ADD;
int EIX_EVENT_CLIENT_DEL;
int EIX_EVENT_CLIENT_DATA;

/**
 * Initialize the Eix library. You must call this function before any
 * other call to an Eix function.
 */
EAPI void eix_init(void)
{
	if (_init) return;

	_init++;
	eina_init();
	eet_init();
	ecore_init();
	ecore_con_init();
	eix_log = eina_log_domain_register("eix", NULL);
	_handler[0] = ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA, _server_data, NULL);
	_handler[1] = ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DEL, _server_del, NULL);
	_handler[2] = ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD, _client_add, NULL);
	_handler[3] = ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL, _client_del, NULL);
	_handler[4] = ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA, _client_data, NULL);

	EIX_EVENT_CLIENT_ADD = ecore_event_type_new();
	EIX_EVENT_CLIENT_DEL = ecore_event_type_new();
	EIX_EVENT_SERVER_ADD = ecore_event_type_new();
	EIX_EVENT_SERVER_DEL = ecore_event_type_new();
	EIX_EVENT_CLIENT_DATA = ecore_event_type_new();
	EIX_EVENT_SERVER_DATA = ecore_event_type_new();
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
		eina_log_domain_unregister(eix_log);
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
	_server_setup(es, svr);

	return es;
}
/**
 * Creates a new Eix server
 * @param[in] name The server's name
 * @param[in] port The server's port to use
 */
EAPI Eix_Server * eix_new(const char *name, int port)
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

	_server_setup(es, svr);
}
/**
 *
 */
Eix_Error eix_message_server_send(Eix_Server *es, Eix_Message_Type type,
		void *data, double timeout, void **rdata)
{
	Eix_Message *m;
	void *body;

	m = eix_message_new(type);
	body = eix_message_encode(eix_message_name_get(m->type),
		data, &m->size);
	if (!body)
		return EIX_ERR_CODEC;

	return _server_send(es, m, body, timeout, rdata);
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
	error = eix_message_server_send(e, EIX_MSG_TYPE_SYNC, &m, 0, (void **)&r);
	if (error) return;
	/* allocate all the hosts and give them back to the user */
	free(r);
}

EAPI void eix_server_message_add(Eix_Server *e, unsigned int id,
		Eix_Message_Type type, Eet_Data_Descriptor *edd)
{
	/* add the descriptor to the server's message array/hash */

}


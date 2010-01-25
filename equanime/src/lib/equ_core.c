#include "Equanime.h"
#include "equ_private.h"

#include "Ecore_Con.h"
#include "Ecore.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equanime
{
	Ecore_Con_Server *svr;
	Equ_Message *msg; /* last message sent */
	Equ_Reply *reply; /* reply for the last message sent */
	void *rbody; /* decoded body of the reply message */
	unsigned char *buffer; /* buffer where the data received from server is stored */
	unsigned int length; /* length of the buffer */
};

static int _init = 0;

void output(void *data, const char *string)
{
	printf("%s", string);
}

static int _server_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Server_Data *e = event;
	Equ_Message_Name rname;
	Equanime *eq = data;
	unsigned int m_length;

	if (!eq->msg)
	{
		ERR("How do we receive a reply with no msg first??\n");
		return 0;
	}
	if (!eq->buffer)
	{
		eq->buffer = e->data;
		eq->length = e->size;
		e->data = NULL;
	}
	else
	{
		eq->buffer = realloc(eq->buffer, eq->length + e->size);
		memcpy(((unsigned char *)eq->buffer) + eq->length, e->data, e->size);
		eq->length += e->size;
		e->data = NULL;
	}
	if (eq->length < sizeof(Equ_Reply))
		return 0;

	eq->reply = (Equ_Reply *)eq->buffer;
	m_length = sizeof(Equ_Reply) + eq->reply->size;
	if (eq->length < m_length)
		return 0;

	/* ok we have a full message */
	DBG("Reply received %d %d", eq->reply->id);
	if (eq->reply->size)
	{
		equ_message_reply_name_get(eq->msg->type, &rname);
		DBG("Decoding data[%d] %d", rname, eq->reply->size);
#if 0
		{
			char *buf = eq->buffer + sizeof(Equ_Reply);
			int i;
			for (i = 0; i < eq->reply->size; i++)
			{
				printf("buf = %c\n", *buf++);
			}
		}
#endif
		eq->rbody = equ_message_decode(rname, eq->buffer + sizeof(Equ_Reply), eq->reply->size);
#if 0
		{
			Equ_Reply_Hosts_Get *rhg;

			rhg = eq->rbody;
			printf("%p %d\n", rhg->hosts[0].name, rhg->hosts[1].id);
			printf("[ %p %d\n", eq->buffer + sizeof(Equ_Reply), eq->reply->size);
			eet_data_text_dump(eq->buffer + sizeof(Equ_Reply), eq->reply->size, output, NULL);
		}
#endif
	}
	/* copy the reply into a new buffer */
	eq->reply = malloc(sizeof(Equ_Reply));
	memcpy(eq->reply, eq->buffer, sizeof(Equ_Reply));
	/* realloc the tmp buffer */
	if (eq->length > m_length)
	{
		unsigned char *tmp;
		unsigned int n_length;

		tmp = eq->buffer;
		n_length = eq->length - m_length;

		eq->buffer = malloc(n_length);
		memcpy(eq->buffer, tmp + m_length, n_length);
		free(tmp);
	}
	else
	{
		free(eq->buffer);
		eq->buffer = NULL;
	}

	return 0;
}

static int _timeout_cb(void *data)
{
	printf("timer!!!\n");
	return 0;
}

static Equ_Error _server_send(Equanime *e, Equ_Message *m, void *data,
		double timeout, void **rdata)
{
	Equ_Error error;
	Ecore_Timer *timer;
	int ret;

	e->msg = m;

	DBG("Sending message of type %d and id %d", m->type, m->id);
	ret = ecore_con_server_send(e->svr, m, sizeof(Equ_Message));
	ret = ecore_con_server_send(e->svr, data, m->size);

	if (equ_message_reply_has(m->type) == EINA_FALSE)
		return EQU_ERR_NONE;

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
	free(e->msg);
	e->msg = NULL;
	e->reply = NULL;

	return error;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int equ_log = -1;

Equ_Error equ_message_server_send(Equanime *e, Equ_Message_Type type,
		void *data, double timeout, void **rdata)
{
	Equ_Message *m;
	void *body;

	m = equ_message_new(type);
	body = equ_message_encode(equ_message_name_get(m->type),
		data, &m->size);
	if (!body)
		return EQU_ERR_CODEC;

	return _server_send(e, m, body, timeout, rdata);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void equ_init(void)
{
	if (_init) return;

	_init++;
	eina_init();
	eet_init();
	ecore_init();
	ecore_con_init();
	equ_message_init();
	equ_log = eina_log_domain_register("equ", NULL);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void equ_shutdown(void)
{
	if (_init == 1)
	{
		eina_log_domain_unregister(equ_log);
		equ_message_shutdown();
		ecore_con_shutdown();
		ecore_shutdown();
		eet_shutdown();
		eina_shutdown();
	}
	_init--;
}

/**
 * Connects to an Equanime server
 * @param[in] port The port to use for the connection
 * @return An equanime instance which identifies a server connection
 */
EAPI Equanime * equ_new(int port)
{
	Equanime *eq;

	eq = calloc(1, sizeof(Equanime));
	ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA, _server_data, eq);
	/* try to connect to the server */
	eq->svr = ecore_con_server_connect(ECORE_CON_LOCAL_USER, EQUANIME_NAME,
			port, NULL);
	return eq;
}

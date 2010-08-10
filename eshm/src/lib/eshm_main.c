#include "Eshm.h"
#include "eshm_private.h"

/**
 * TODO
 * Add a buffer for the message received
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

typedef struct _Eshm
{
#if 0
	Ecore_Con_Server *svr;
#endif
	Eix_Server *svr;
} Eshm;

static Eshm _eshm;

static int _init = 0;

static int _timeout_cb(void *data)
{
	printf("timer!!!\n");
	return 0;
}

static int _server_data(void *data, int type, void *event)
{
	return ECORE_CALLBACK_RENEW;
}

#if 0
static Eshm_Error _server_send(Eshm_Message *m, void *data, double timeout, void **rdata)
{
	Eshm_Error error;
	Ecore_Timer *timer;
	int ret;

	_eshm.msg = m;

	DBG("Sending message. type = %d id = %d size = %d", m->type, m->id, m->size);
	ret = ecore_con_server_send(_eshm.svr, m, sizeof(Eshm_Message));
	ret = ecore_con_server_send(_eshm.svr, data, m->size);

	if (eshm_message_reply_has(m->type) == EINA_FALSE)
		return ESHM_ERR_NONE;

	/* TODO register a timeout callback */
	if (timeout)
		timer = ecore_timer_add(timeout, _timeout_cb, NULL);
	/* wait for the response */
	while (!_eshm.reply)
	{
		ecore_main_loop_iterate();
	}
	if (timeout)
		ecore_timer_del(timer);

	DBG("Finished lock reply. type %d", _eshm.reply->id);

	error = _eshm.reply->error;

	if (rdata)
		*rdata = _eshm.rbody;
	free(_eshm.reply);
	free(_eshm.msg);
	_eshm.msg = NULL;
	_eshm.reply = NULL;

	return error;
}
#endif

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int eshm_log_dom = -1;

/**
 * Sends a message to the server
 *
 * The message is encoded and then sent to the server, if the message needs
 * a reply this function will wait @timeout ms for it. The encoding and decoding
 * of the message is transparent.
 *
 * @param mhdr A completely message to send to the server
 * @param timeout Number of ms to wait for the reply in case of needed
 * @param rhdr The reply will be stored here
 * @return
 *
 * TODO the reply can be embedded on the header type itself?
 * TODO Encode the message with eet!!
 */
Eshm_Error eshm_message_server_send(Eshm_Message_Type type, void *data, double timeout, void **rdata)
{
	Eshm_Message *m;
	void *body;

	m = eshm_message_new(type);
	DBG("Encoding message. type = %d", type);
	body = eshm_message_encode(eshm_message_name_get(m->type), data, &m->size);
	if (!body)
		return ESHM_ERR_CODEC;

	return _server_send(m, body, timeout, rdata);
}

void eshm_common_init(void)
{
	ecore_init();
	ecore_con_init();
	eshm_message_init();
}
void eshm_common_shutdown(void)
{
	eshm_message_shutdown();
	ecore_con_shutdown();
	ecore_shutdown();
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
int ESHM_ERROR_ACCESS;
int ESHM_ERROR_EXIST;
int ESHM_ERROR_NEXIST;
int ESHM_ERROR_CODEC;
int ESHM_ERROR_TIMEOUT;
/**
 * Initialize the library
 * @return The number of times the library has been initialized
 */
EAPI int eshm_init(void)
{
	if (!_init)
	{
		eina_init();
		ecore_init();
		eix_init();
		eshm_log_dom = eina_log_domain_register("eshm", NULL);

		/* try to connect to the daemon */
		_eshm.svr = eix_server_connect(ESHMD_NAME, ESHMD_PORT);
		if (!_eshm.svr)
		{
			ERR("The server does not exist");
			eix_shutdown();
			ecore_shutdown();
			eina_log_domain_unregister(eshm_log_dom);
			eina_shutdown();
			return 0;
		}
		ecore_event_handler_add(EIX_SERVER_DATA, _server_data, NULL);
		ESHM_ERROR_ACCESS = eina_error_msg_register("Not enough permission for the request");
		ESHM_ERROR_EXIST = eina_error_msg_register("Segment already exists");
		ESHM_ERROR_NEXIST = eina_error_msg_register("Segment does not exist");
		ESHM_ERROR_CODEC = eina_error_msg_register("Encoding / Decoding failed");
		ESHM_ERROR_TIMEOUT = eina_error_msg_register("Timeout waiting for response");
	}
	return ++_init;
}
/**
 * Shutdowns the library
 */
EAPI void eshm_shutdown(void)
{
	if (!--_init)
	{
		eshm_message_shutdown();
		ecore_con_shutdown();
		ecore_shutdown();
		eina_log_domain_unregister(eshm_log_dom);
		eina_shutdown();
	}
}

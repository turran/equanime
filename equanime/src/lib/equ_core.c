#include "Equanime.h"
#include "equ_private.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equanime
{
	Eix_Server *svr;
	Equ_Message *msg; /* last message sent */
	Equ_Reply *reply; /* reply for the last message sent */
	void *rbody; /* decoded body of the reply message */
	unsigned char *buffer; /* buffer where the data received from server is stored */
	unsigned int length; /* length of the buffer */
};

static int _init = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int equ_log = -1;

int equ_message_server_send(Equanime *e, unsigned int type,
		void *data, double timeout, void **rdata)
{
	return eix_message_server_send(e->svr, type, data, timeout, rdata);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Initialize the Equanime library. You must call this function before any
 * other call to an Equanime function.
 */
EAPI void equ_init(void)
{
	if (_init) return;

	if (!eshm_init()) return;
	_init++;
	eina_init();
	eet_init();
	ecore_init();
	ecore_con_init();
	equ_message_init();
	equ_log = eina_log_domain_register("equ", NULL);
}
/**
 * Shutdowns the Equanime library. Once you have finished using Equanime
 * call this function to deinitialize everything
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
		eshm_shutdown();
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
	Eix_Server *svr;

	/* try to connect to the server */
	svr = eix_connect(EQUANIME_NAME, port);

	if (!svr) return NULL;

	eq = calloc(1, sizeof(Equanime));
	eq->svr = svr;
	//ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DEL, _server_del, eq);

	return eq;
}

/**
 * Flushes every message and waits until the server
 * has processed every message
 * @param[in] e Equanime The equanime connection
 */
EAPI void equ_sync(Equanime *e)
{
	Equ_Message_Sync m;
	Equ_Reply_Sync *r = NULL;
	int error;

	/* send the command to the server */
	error = equ_message_server_send(e, EIX_MESSAGE_SYNC, &m, 0, (void **)&r);
	if (error) return;
	/* allocate all the hosts and give them back to the user */
	free(r);
}

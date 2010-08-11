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
	Eix_Server *svr;
} Eshm;

static Eshm _eshm;

static int _init = 0;

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
 */
Eshm_Error eshm_message_server_send(int type, void *data, double timeout, void **rdata)
{
	printf("tryng to send message %d\n", type);
	eix_message_server_send(_eshm.svr, type, data, timeout, rdata);
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
		eshm_common_init();
		eshm_log_dom = eina_log_domain_register("eshm", NULL);

		/* try to connect to the daemon */
		_eshm.svr = eix_connect(ESHMD_NAME, ESHMD_PORT);
		eshm_common_server_setup(_eshm.svr);
		if (!_eshm.svr)
		{
			ERR("The server does not exist");
			eix_shutdown();
			ecore_shutdown();
			eina_log_domain_unregister(eshm_log_dom);
			eina_shutdown();
			return 0;
		}
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
		eina_log_domain_unregister(eshm_log_dom);
		eshm_common_shutdown();
	}
}

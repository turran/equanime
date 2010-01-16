#include "Equanime.h"
#include "equ_private.h"
#include "Ecore_Con.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init = 0;

static int _server_data(void *data, int type, void *event)
{
	printf("server data!!!\n");
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
	/* try to connect to the server */
	ecore_con_server_connect(ECORE_CON_LOCAL_USER, EQUANIME_NAME,
			EQUANIME_PORT, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_SERVER_DATA, _server_data, NULL);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void equ_shutdown(void)
{
	if (_init == 1)
	{
		ecore_con_shutdown();
		ecore_shutdown();
		eet_shutdown();
		eina_shutdown();
	}
	_init--;
}

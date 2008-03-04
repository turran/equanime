#include "Equanime.h"
#include "equanime_private.h"

Equanime_Controller *_c = NULL;

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void equanime_init(void)
{
	/* try to load every controller */
	mp25xxf_controller_init();
}

EAPI void equanime_shutdown(void)
{
	/* unload every controller */
	mp25xxf_controller_exit();
}

/**
 * 
 */
EAPI void equanime_controllers_get(void *cb, void *cb_data)
{
	
}

/**
 * 
 */
EAPI void equanime_controller_layers_get(Equanime_Controller *c, void *cb, void *cb_data)
{
	
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void equanime_controller_register(Equanime_Controller_Description *cd)
{
	Equanime_Controller *c;
	
	c = calloc(1, sizeof(Equanime_Controller));
	_c = c;
}


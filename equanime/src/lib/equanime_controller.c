#include "Equanime.h"
#include "equanime_private.h"

/* TODO instead of this hardcoded we should have a list of controllers */
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
Equanime_Controller * equanime_controller_register(Equanime_Controller_Description *cd)
{
	Equanime_Controller *c;
	
	c = calloc(1, sizeof(Equanime_Controller));
	_c = c;
	return c;
}

void equanime_controller_data_set(Equanime_Controller *ec, void *data)
{
	
}

void * equanime_controller_data_get(Equanime_Controller *ec)
{
	
}

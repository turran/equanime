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
EAPI void equanime_controllers_get(Equanime_Cb cb, void *cb_data)
{
	/* TODO for every controller call cb with cb_data */
	/* for now */
	cb(_c, cb_data);
}

/**
 * 
 */
EAPI void equanime_controller_layers_get(Equanime_Controller *c, Equanime_Cb cb, void *cb_data)
{
	Equanime_Layer *l;
	int i;
	
	l = c->layers;
	for (i = 0; i < c->desc->num_layers; i++)
	{
		cb(l, cb_data);
		l++;
	}
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
	ec->data = data;
}

void * equanime_controller_data_get(Equanime_Controller *ec)
{
	return ec->data;
}

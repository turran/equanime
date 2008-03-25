#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"

static Equanime_Controller *_controllers = NULL;

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void equanime_init(void)
{
	/* try to load every controller */
	mp25xxf_controller_module_init();
	/* then load every layer module */
	mp25xxf_rgb_module_init();
}

EAPI void equanime_shutdown(void)
{
	/* unload every module */
	/* first the controllers */
	mp25xxf_controller_module_exit();
	/* then the layers */
	mp25xxf_rgb_module_exit();
}

/**
 * 
 */
EAPI void equanime_controllers_get(Equanime_Cb cb, void *cb_data)
{
#if 0
	Equanime_Controller *c, **cs;
	int i;
		
	cs = _controllers;
	for (i = 0; i < _num_controllers; i++)
	{
		c = *cs;
		cb(c, cb_data);
		cs++;
	}
#endif
}

/**
 * 
 */
EAPI void equanime_controller_layers_get(Equanime_Controller *c, Equanime_Cb cb, void *cb_data)
{
	Equanime_Layer *l, **ls;
	int i;
	
	ls = c->layers;
	for (i = 0; i < c->num_layers; i++)
	{
		l = *ls;
		cb(l, cb_data);
		ls++;
	}
}

/**
 * 
 */
EAPI void equanime_controller_register(Equanime_Controller_Description *cd, Equanime_Controller_Functions *cf)
{
	Equanime_Controller *c;
	
	c = calloc(1, sizeof(Equanime_Controller));
	c->desc = cd;
	c->fncs = cf;
	/* call the probe function */
	if (!(c->fncs->probe(c)))
	{
		free(c);
		return;
	}
	/* add the controller to the list of controllers */
	_controllers = eina_inlist_append(_controllers, c);
}
/**
 * 
 */
EAPI void equanime_controller_unregister(Equanime_Controller_Description *cd)
{
	/* TODO Do nothing for now, we should remove the controller from the list
	 * of controllers and then free the controller itself */
}
/**
 * 
 */
EAPI void equanime_controller_data_set(Equanime_Controller *ec, void *data)
{
	ec->data = data;
}
/**
 * 
 */
EAPI void * equanime_controller_data_get(Equanime_Controller *ec)
{
	return ec->data;
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

void equanime_controller_layer_register(const char *name, Equanime_Layer *l)
{
	Equanime_Controller *c;
		
	/* find the controller with the same name */
	/* add the layer to the list of layers */
	/* increment the number of layers */
	l->controller = c;
	//c->desc->num_layers++;
}

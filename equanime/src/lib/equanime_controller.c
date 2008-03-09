#include "Equanime.h"
#include "equanime_private.h"

/* TODO instead of this hardcoded we should have a list of controllers */
Equanime_Controller **_controllers;
int _num_controllers;

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
	Equanime_Controller *c, **cs;
	int i;
		
	cs = _controllers;
	for (i = 0; i < _num_controllers; i++)
	{
		c = *cs;
		cb(c, cb_data);
		cs++;
	}
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
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * 
 */
void equanime_controller_register(Equanime_Controller_Description *cd)
{
	Equanime_Controller *c;
	
	c = calloc(1, sizeof(Equanime_Controller));
	c->desc = cd;
	/* call the probe function */
	if (!(c->desc->fncs.probe(c)))
	{
		free(c);
		return;
	}
	printf("Controller registered\n");
	/* add the controller to the list of controllers */
	_controllers = realloc(_controllers, sizeof(Equanime_Controller *) * (_num_controllers + 1));
	_controllers[_num_controllers] = c;
	_num_controllers++;
}

/**
 * 
 */
void equanime_controller_unregister(Equanime_Controller_Description *cd)
{
	/* TODO Do nothing for now, we should remove the controller from the list
	 * of controllers and then free the controller itself */
}

void equanime_controller_layer_register(const char *name, Equanime_Layer *l)
{
	
}

void equanime_controller_data_set(Equanime_Controller *ec, void *data)
{
	ec->data = data;
}

void * equanime_controller_data_get(Equanime_Controller *ec)
{
	return ec->data;
}

#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"
/**
 * A controller is in charge of controlling the global output, disabling and
 * enabling specific layers and change their priority.
 * 
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Equanime_Controller *_controllers = NULL;
static Equanime_Layer *_layers = NULL;
static int _init = 0;
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI void equanime_init(void)
{
	if (_init) return;
	
	_init++;
	equanime_module_load_all();
}
/**
 * 
 */
EAPI void equanime_shutdown(void)
{
	if (_init == 1)
	{
		equanime_module_unload_all();
	}
	_init--;
}
/**
 * 
 */
EAPI void equanime_controllers_get(Equanime_Cb cb, void *cb_data)
{
	Eina_Inlist *l;
	
	for (l = (Eina_Inlist *)_controllers; l; l = l->next)
	{
		Equanime_Controller *c = (Equanime_Controller *)l;
		cb(c, cb_data);
	}
}
/**
 * 
 */
EAPI void equanime_controller_layers_get(Equanime_Controller *c, Equanime_Cb cb, void *cb_data)
{
	Eina_Inlist *l;
		
	for (l = (Eina_Inlist *)_layers; l; l = l->next)
	{
		Equanime_Layer *y = (Equanime_Layer *)l;
		if (y->controller == c)
			cb(y, cb_data);
	}
}
/**
 * 
 */
EAPI int equanime_controller_register(Equanime_Controller_Description *cd, Equanime_Controller_Functions *cf)
{
	Equanime_Controller *c;
	
	c = calloc(1, sizeof(Equanime_Controller));
	c->desc = cd;
	c->fncs = cf;
	/* call the probe function */
	if (!(c->fncs->probe(c)))
	{
		free(c);
		return 0;
	}
	/* add the controller to the list of controllers */
	_controllers = eina_inlist_append(_controllers, c);
	return 1;
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
/**
 * 
 */
void equanime_controller_layer_register(const char *name, Equanime_Layer *l)
{
	Equanime_Controller *c;
	/* check that the name exists on the list of layers, if so register it */
	/* find the controller with the same name */
	/* add the layer to the list of layers */
	/* increment the number of layers */
	l->controller = c;
	//c->desc->num_layers++;
}
/**
 * 
 */
void equanime_controller_layer_unregister(void)
{
	
}

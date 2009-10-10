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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 *
 */
void equanime_controller_layer_register(Equanime_Controller *ec, Equanime_Layer *el)
{
	ec->num_layers++;
	_layers = eina_inlist_append(_layers, el);
}
/**
 *
 */
void equanime_controller_layer_unregister(Equanime_Layer *el)
{
	// itearate over the list of layers and get it
	//el->controller->num_layers--;
	//_layers = eina_inlist_remove(_layers, el);
}
/**
 *
 */
Equanime_Controller * equanime_controller_name_get_by(const char *name)
{
	Equanime_Controller *c;

	Eina_Inlist *l;

	for (l = (Eina_Inlist *)_controllers; l; l = l->next)
	{
		Equanime_Controller *c = (Equanime_Controller *)l;
		if (!strcmp(c->desc->name, name))
		{
			/* increment the number of layers */
			return c;
		}
	}
	return NULL;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
EAPI void equanime_controllers_get(Equanime_Cb cb, void *cb_data)
{
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)_controllers; l; l = l->next)
	{
		Equanime_Controller *c = (Equanime_Controller *)l;
		if (!cb(c, cb_data))
			return;
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
			if (!cb(y, cb_data))
				return;
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
/**
 *
 */
EAPI const Equanime_Controller_Description * equanime_controller_description_get(Equanime_Controller *ec)
{
	return ec->desc;

}


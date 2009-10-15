#include "Equanime.h"
#include "equanime_private.h"
/**
 * A controller is in charge of controlling the global output, disabling and
 * enabling specific layers and change their priority. Also setting the
 * standard output, the timmings, etc
 *
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Equ_Controller *_controllers = NULL;
static Equ_Layer *_layers = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 *
 */
void equ_controller_layer_register(Equ_Controller *ec, Equ_Layer *el)
{
	ec->num_layers++;
	_layers = eina_inlist_append(_layers, el);
}
/**
 *
 */
void equ_controller_layer_unregister(Equ_Layer *el)
{
	// itearate over the list of layers and get it
	//el->controller->num_layers--;
	//_layers = eina_inlist_remove(_layers, el);
}
/**
 *
 */
Equ_Controller * equ_controller_name_get_by(const char *name)
{
	Equ_Controller *c;

	Eina_Inlist *l;

	for (l = (Eina_Inlist *)_controllers; l; l = l->next)
	{
		Equ_Controller *c = (Equ_Controller *)l;
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
EAPI void equ_controllers_get(Equ_Cb cb, void *cb_data)
{
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)_controllers; l; l = l->next)
	{
		Equ_Controller *c = (Equ_Controller *)l;
		if (!cb(c, cb_data))
			return;
	}
}
/**
 *
 */
EAPI void equ_controller_layers_get(Equ_Controller *c, Equ_Cb cb, void *cb_data)
{
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)_layers; l; l = l->next)
	{
		Equ_Layer *y = (Equ_Layer *)l;
		if (y->controller == c)
			if (!cb(y, cb_data))
				return;
	}
}
/**
 *
 */
EAPI int equ_controller_register(Equ_Controller_Description *cd, Equ_Controller_Functions *cf)
{
	Equ_Controller *c;

	c = calloc(1, sizeof(Equ_Controller));
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
EAPI void equ_controller_unregister(Equ_Controller_Description *cd)
{
	/* TODO Do nothing for now, we should remove the controller from the list
	 * of controllers and then free the controller itself */
}
/**
 *
 */
EAPI void equ_controller_data_set(Equ_Controller *ec, void *data)
{
	ec->data = data;
}
/**
 *
 */
EAPI void * equ_controller_data_get(Equ_Controller *ec)
{
	return ec->data;
}
/**
 *
 */
EAPI const Equ_Controller_Description * equ_controller_description_get(Equ_Controller *ec)
{
	return ec->desc;

}


#include "Equanime.h"
#include "equanime_private.h"
/**
 * A controller is in charge of controlling the global output, disabling and
 * enabling specific layers and change their priority. Also setting the
 * standard output, the timings, etc
 *
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 *
 */
Equ_Layer * equ_controller_layer_register(Equ_Controller *ec,
		Equ_Layer_Backend *lb, const char *name, void *data)
{
	Equ_Layer *l;

	l = equ_layer_new(ec, lb, name, data);
	ec->layers = eina_list_append(ec->layers, l);
	return l;
}

void equ_controller_output_register(Equ_Controller *ec, Equ_Output_Backend *ob,
		const char *name, void *data)
{
	Equ_Output *o;

	o = equ_output_new(ec, ob, name, data);
	ec->outputs = eina_list_append(ec->outputs, o);
}

void equ_controller_input_register(Equ_Controller *ec, Equ_Input_Backend *ib,
		const char *name, void *data)
{
	Equ_Input *i;

	i = equ_input_new(ec, ib, name, data);
	ec->outputs = eina_list_append(ec->outputs, i);
}
/**
 *
 */
void equ_controller_layer_unregister(Equ_Layer *el)
{
}

/**
 *
 */
EAPI Equ_Controller * equ_controller_register(Equ_Controller_Backend *backend,
		const char *name, void *data)
{
	Equ_Controller *c;

	c = malloc(sizeof(Equ_Controller));
	c->backend = backend;
	c->data = data;
	c->name = name;

	return c;
}

/**
 *
 */
void equ_controller_unregister(Equ_Controller *c)
{
	/* TODO Do nothing for now, we should remove the controller from the list
	 * of controllers and then free the controller itself */
}
/*
 *
 */
void * equ_controller_data_get(Equ_Controller *c)
{
	return c->data;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
EAPI void equ_controllers_get(Equ_Cb cb, void *cb_data)
{

}
/**
 *
 */
EAPI void equ_controller_layers_get(Equ_Controller *c, Equ_Cb cb, void *cb_data)
{
}


#include "Equ_Server.h"
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
		const char *name, Equ_Layer_Backend *lb)
{
	Equ_Layer *l;

	l = equ_layer_new(ec, name, lb);
	ec->layers = eina_list_append(ec->layers, l);
	return l;
}

Equ_Output * equ_controller_output_register(Equ_Controller *ec,
		const char *name, Equ_Output_Backend *ob)
{
	Equ_Output *o;

	o = equ_output_new(ec, name, ob);
	ec->outputs = eina_list_append(ec->outputs, o);
	return o;
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
void equ_controller_layer_unregister(Equ_Layer *l)
{
	eina_list_remove(l->controller->layers, l);
}

void equ_controller_output_unregister(Equ_Output *o)
{
	eina_list_remove(o->controller->outputs, o);
}

/**
 *
 */
EAPI Equ_Controller * equ_controller_new(Equ_Host *h,
		const char *name, Equ_Controller_Backend *cb)
{
	Equ_Controller *c;

	c = calloc(1, sizeof(Equ_Controller));
	c->host = h;
	c->backend = cb;
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
EAPI const char * equ_controller_name_get(Equ_Controller *c)
{
	return c->name;
}
/**
 *
 */
EAPI void equ_controller_layers_get(Equ_Controller *c, Equ_Cb cb, void *cb_data)
{
	Equ_Layer *la;
	Eina_List *l;

	EINA_LIST_FOREACH(c->layers, l, la)
	{
		cb(la, cb_data);
	}
}

/**
 *
 */
EAPI void equ_controller_outputs_get(Equ_Controller *c, Equ_Cb cb, void *cb_data)
{
	Equ_Output *o;
	Eina_List *l;

	EINA_LIST_FOREACH(c->outputs, l, o)
	{
		cb(o, cb_data);
	}

}

/**
 *
 */
EAPI void equ_controller_inputs_get(Equ_Controller *c, Equ_Cb cb, void *cb_data)
{
	Equ_Input *i;
	Eina_List *l;

	EINA_LIST_FOREACH(c->inputs, l, i)
	{
		cb(i, cb_data);
	}

}
/**
 *
 */
EAPI Eina_Bool equ_controller_output_set(Equ_Controller *c, Equ_Output *o)
{
	if (o->controller != c)
		return EINA_FALSE;
	if (c->backend->output_set)
		return c->backend->output_set(c, o);
	return EINA_FALSE;
}

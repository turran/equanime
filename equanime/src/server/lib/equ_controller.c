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
EAPI Equ_Controller * equ_controller_new(Equ_Host *h, Equ_Controller_Backend *backend,
		const char *name, void *data)
{
	Equ_Controller *c;

	c = calloc(1, sizeof(Equ_Controller));
	c->host = h;
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

#include "Equ_Server.h"
#include "equ_server_private.h"
/**
 * A controller is in charge of controlling the global output, disabling and
 * enabling specific layers and change their priority. Also setting the
 * standard output, the timings, etc
 *
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/**
 *
 */
struct _Equ_Controller
{
	const Equ_Controller_Backend *backend;
	const char *name;
	void *data;
	Equ_Common_Id id;

	Equ_Host *host;
	Eina_List *layers;
	Eina_List *outputs;
	Eina_List *inputs;
};

Eina_Hash *_controllers = NULL;
static Equ_Common_Id _ids = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/**
 *
 */
Equ_Layer * equ_controller_layer_register(Equ_Controller *ec,
		const char *name, Equ_Layer_Backend *lb,
		Equ_Layer_Caps *caps, Equ_Layer_Status *status)
{
	Equ_Layer *l;

	l = equ_layer_new(ec, name, lb, caps, status);
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
void equ_controller_layer_unregister(Equ_Controller *c, Equ_Layer *l)
{
	c->layers = eina_list_remove(c->layers, l);
}

void equ_controller_output_unregister(Equ_Controller *c, Equ_Output *o)
{
	c->outputs = eina_list_remove(c->outputs, o);
}

/**
 *
 */
Equ_Controller * equ_controller_new(Equ_Host *h,
		const char *name, Equ_Controller_Backend *cb)
{
	Equ_Controller *c;

	if (!_controllers)
		_controllers = eina_hash_int32_new(NULL);

	c = calloc(1, sizeof(Equ_Controller));
	c->host = h;
	c->backend = cb;
	c->name = name;
	c->id = _ids++;
	eina_hash_add(_controllers, &c->id, c);

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

EAPI Equ_Common_Id equ_controller_id_get(Equ_Controller *c)
{
	return c->id;
}

EAPI Equ_Controller * equ_controller_get(Equ_Common_Id id)
{
	return eina_hash_find(_controllers, &id);
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
	if (equ_output_controller_get(o) != c)
		return EINA_FALSE;
	if (c->backend->output_set)
		return c->backend->output_set(c, o);
	return EINA_FALSE;
}

EAPI Equ_Host * equ_controller_host_get(Equ_Controller *c)
{
	return c->host;
}

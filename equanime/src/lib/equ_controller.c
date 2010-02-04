#include "Equanime.h"
#include "equ_private.h"
/**
 * A controller is in charge of controlling the global output, disabling and
 * enabling specific layers and change their priority. Also setting the
 * standard output, the timings, etc
 *
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Controller
{
	Equ_Common_Id id;
	const char *name;
	Equ_Host *host;
};

typedef struct _Equ_Controller_Layer_Get_Cb
{
	Equanime *e;
	Equ_Layer *layer;
	const char *name;
} Equ_Controller_Layer_Get_Cb;

static int _controller_layer_get_cb(void *data, void *cb_data)
{
	Equ_Layer *l = data;
	Equ_Controller_Layer_Get_Cb *cb = cb_data;

	if (!cb->name)
	{
		cb->layer = l;
		return EINA_FALSE;
	}

	if (!strcmp(equ_layer_name_get(cb->e, l), cb->name))
	{
		cb->layer = l;
		return EINA_FALSE;
	}
	equ_layer_delete(l);

	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
EAPI Equ_Controller * equ_controller_new(Equ_Host *h, Equ_Common_Id id,
		char *name)
{
	Equ_Controller *c;

	c = calloc(1, sizeof(Equ_Controller));
	c->host = h;
	c->id = id;
	c->name = strdup(name);

	return c;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_controller_delete(Equ_Controller *c)
{
	free(c->name);
	free(c);
}
/**
 *
 */
EAPI const char * equ_controller_name_get(Equanime *e, Equ_Controller *c)
{
	return c->name;
}
/**
 *
 */
EAPI void equ_controller_layers_get(Equanime *e, Equ_Controller *c, Equ_Cb cb, void *cb_data)
{
	Equ_Message_Layers_Get m;
	Equ_Reply_Layers_Get *r = NULL;
	Equ_Error error;
	Equ_Layer *l;
	int i;

	/* send the command to the server */
	m.controller_id = c->id;
	error = equ_message_server_send(e, EQU_MSG_TYPE_LAYERS_GET, &m, 0, (void **)&r);
	if (error) return;
	/* allocate all the hosts and give them back to the user */
	for (i = 0; i < r->layers_count; i++)
	{
		l = equ_layer_new(e, c, r->layers[i].id, r->layers[i].name);
		if (!cb(l, cb_data))
			break;
	}
	free(r);
}

EAPI Equ_Layer * equ_controller_layer_get(Equanime *e, Equ_Controller *c, const char *name)
{
	Equ_Controller_Layer_Get_Cb cb;

	cb.e = e;
	cb.name = name;
	cb.layer = NULL;

	equ_controller_layers_get(e, c, _controller_layer_get_cb, &cb);

	return cb.layer;
}

#if 0
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
	return EINA_FALSE;
}
#endif

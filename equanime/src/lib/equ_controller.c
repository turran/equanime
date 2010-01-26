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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
EAPI Equ_Controller * equ_controller_new(Equ_Host *h, Equ_Common_Id id)
{
	Equ_Controller *c;

	c = calloc(1, sizeof(Equ_Controller));
	c->host = h;
	c->id = id;

	return c;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Equ_Controller * equ_controller_get(Equanime *e, const char *hname,
		const char *name)
{

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
	for (i = 0; i < r->ids_count; i++)
	{
		l = equ_layer_new(c, r->ids[i]);
		if (!cb(l, cb_data))
			break;
	}
	free(r);
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

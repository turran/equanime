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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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
	return EINA_FALSE;
}

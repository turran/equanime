#include "Equ_Server.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Output * equ_output_new(Equ_Controller *c,
		const char *name, Equ_Output_Backend *ob)
{
	Equ_Output *o;

	o = malloc(sizeof(Equ_Output));
	o->controller = c;
	o->backend = ob;
	o->name = name;

	return o;
}

void * equ_output_data_get(Equ_Output *o)
{
	return o->data;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI const char * equ_output_name_get(Equ_Output *o)
{
	return o->name;
}

EAPI Equ_Mode * equ_output_mode_get(Equ_Output *o)
{
	return &o->mode;
}

EAPI Eina_Bool equ_output_mode_set(Equ_Output *o, Equ_Mode *m)
{
	if (o->backend->mode_set)
	{
		if (o->backend->mode_set(o, m))
		{
			o->mode = *m;
			printf("ok!!\n");

			return EINA_TRUE;
		}
	}
	return EINA_FALSE;
}

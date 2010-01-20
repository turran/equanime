#include "Equ_Server.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Output
{
	Equ_Output_Backend *backend;
	const char *name;
	void *data;

	Equ_Controller *controller;
	Equ_Mode mode;
};
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
EAPI void equ_output_unregister(Equ_Output *o)
{
	equ_controller_output_unregister(o->controller, o);
}

EAPI Equ_Controller * equ_output_controller_get(Equ_Output *o)
{
	return o->controller;
}

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

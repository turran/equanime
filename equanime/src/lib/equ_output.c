#include "Equanime.h"
#include "equanime_private.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Output * equ_output_new(Equ_Controller *c, Equ_Output_Backend *ob,
		const char *name, void *data)
{
	Equ_Output *o;

	o = malloc(sizeof(Equ_Output));
	o->backend = ob;
	o->name = name;
	o->data = data;

	return 0;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI const char * equ_output_name_get(Equ_Output *o)
{
	return o->name;
}

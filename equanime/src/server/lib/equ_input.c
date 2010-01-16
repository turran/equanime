#include "Equ_Server.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Input * equ_input_new(Equ_Controller *c, Equ_Input_Backend *ib,
		const char *name, void *data)
{
	Equ_Input *i;

	i = malloc(sizeof(Equ_Input));
	i->backend = ib;
	i->name = name;
	i->data = data;

	return i;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI const char * equ_input_name_get(Equ_Input *i)
{
	return i->name;
}

EAPI Equ_Mode * equ_input_mode_get(Equ_Input *i)
{
	return i->mode;
}

EAPI Eina_Bool equ_input_mode_set(Equ_Input *i, Equ_Mode *m)
{

}

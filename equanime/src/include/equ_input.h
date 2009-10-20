#ifndef _EQU_INPUT_H
#define _EQU_INPUT_H

typedef struct _Equ_Input Equ_Input; /**< Opaque handler */

EAPI Equ_Mode * equ_input_mode_get(Equ_Input *i);
EAPI Eina_Bool equ_input_mode_set(Equ_Input *i, Equ_Mode *m);

#endif

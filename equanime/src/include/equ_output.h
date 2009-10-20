#ifndef _EQU_OUTPUT_H
#define _EQU_OUTPUT_H

typedef struct _Equ_Output Equ_Output; /**< Opaque handler */

EAPI const char * equ_output_name_get(Equ_Output *o);
EAPI Equ_Mode * equ_output_mode_get(Equ_Output *o);
EAPI Eina_Bool equ_output_mode_set(Equ_Output *o, Equ_Mode *m);

#endif

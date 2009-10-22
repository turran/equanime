#ifndef _EQU_CONTROLLER_H
#define _EQU_CONTROLLER_H

/**
 * @defgroup Equanime_Controllers_Group Controllers
 * @{
 */

typedef struct _Equ_Controller Equ_Controller; /**< Opaque handler */

EAPI const char * equ_controller_name_get(Equ_Controller *c);
EAPI void equ_controller_layers_get(Equ_Controller *c, Equ_Cb cb, void *cb_data);
EAPI void equ_controller_outputs_get(Equ_Controller *c, Equ_Cb cb, void *cb_data);
EAPI void equ_controller_inputs_get(Equ_Controller *c, Equ_Cb cb, void *cb_data);

/** @} */

#endif

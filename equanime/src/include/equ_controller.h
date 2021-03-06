#ifndef _EQU_CONTROLLER_H
#define _EQU_CONTROLLER_H

/**
 * @ingroup Equanime_Client
 * @defgroup Equanime_Controllers_Group Controllers
 * @{
 */

EAPI const char * equ_controller_name_get(Equanime *e, Equ_Controller *c);
EAPI void equ_controller_layers_get(Equanime *e, Equ_Controller *c, Equ_Cb cb, void *cb_data);
EAPI Equ_Layer * equ_controller_layer_get(Equanime *e, Equ_Controller *c, const char *name);

EAPI void equ_controller_outputs_get(Equ_Controller *c, Equ_Cb cb, void *cb_data);
EAPI void equ_controller_inputs_get(Equ_Controller *c, Equ_Cb cb, void *cb_data);

/** @} */

#endif

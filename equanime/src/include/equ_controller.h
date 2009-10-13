#ifndef _EQU_CONTROLLER_H
#define _EQU_CONTROLLER_H

/**
 * @defgroup Equanime_Controllers_Group Controllers
 * @{
 */

/**
 *
 */
struct _Equ_Controller_Description
{
	const char *name; /** Controller name */
};

typedef struct _Equ_Controller Equ_Controller; /**< Opaque handler */
typedef struct _Equ_Controller_Description Equ_Controller_Description; /**< Opaque handler */

EAPI void equ_controllers_get(Equ_Cb cb, void *cb_data);
EAPI void equ_controller_layers_get(Equ_Controller *c, Equ_Cb cb, void *cb_data);
EAPI const Equ_Controller_Description * equ_controller_description_get(Equ_Controller *c);

/** @} */

#endif

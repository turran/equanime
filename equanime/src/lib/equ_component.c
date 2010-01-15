#include "Equanime.h"
#include "equ_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

typedef enum _Equ_Component_Flags
{
	EQU_COMPONENT_SCALES = 1 << EQU_COMPONENT_SCALER,
	EQU_COMPONENT_ROTATES = 1 << EQU_COMPONENT_ROTATOR,
	EQU_COMPONENT_CSCS = 1 << EQU_COMPONENT_CSC,
} Equ_Component_Flags;


struct _Equ_Component
{
	Equ_Component_Flags flags;
	Equ_Rotator r;
	Equ_Scaler s;
	Equ_Csc csc;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void * equ_component_interface_get(Equ_Component *c,
	Equ_Component_Type type)
{

}

EAPI Eina_Bool equ_component_setup(Equ_Component *c)
{

}

EAPI void equ_component_run(Equ_Component *c)
{

}


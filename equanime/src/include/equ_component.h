#ifndef _EQU_COMPONENT_H
#define _EQU_COMPONENT_H

typedef struct _Equ_Component Equ_Component;

typedef enum _Equ_Component_Type
{
	EQU_COMPONENT_SCALER,
	EQU_COMPONENT_ROTATOR,
	EQU_COMPONENT_CSC,
	EQU_COMPONENT_TYPES,
} Equ_Component_Type;

#endif

#ifndef _EQU_ENUMS_H
#define _EQU_ENUMS_H

typedef enum _Equ_Component_Type
{
	EQU_COMPONENT_SCALER,
	EQU_COMPONENT_ROTATOR,
	EQU_COMPONENT_CSC,
	EQU_COMPONENT_TYPES,
} Equ_Component_Type;

typedef enum _Equ_Format
{
	EQU_FORMAT_ARGB8888,
	EQU_FORMAT_RGB888,
	EQU_FORMAT_RGB565,
	EQU_FORMAT_Ab31,
	EQU_FORMAT_YUV422,
	EQU_FORMATS,
} Equ_Format;

/**
 *
 */
typedef enum
{
	EQU_SURFACE_ONSCREEN, /**< Visible graphics memory */
	EQU_SURFACE_OFFSCREEN, /**< Non visible graphics memory */
	EQU_SURFACE_VIRTUAL, /**< Virtual space */
} Equ_Surface_Type;


typedef enum _Equ_Angle
{
	EQU_ANGLE_0,
	EQU_ANGLE_90,
	EQU_ANGLE_180,
	EQU_ANGLE_270,
	EQU_ANGLES,
} Equ_Angle;

/**
 *
 */
typedef enum
{
	EQU_OUTPUT_LCD,
	EQU_OUTPUT_TV,
	EQU_OUTPUT_TYPES,
} Equ_Output_Type;


#endif

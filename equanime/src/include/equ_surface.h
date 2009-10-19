#ifndef _EQU_SURFACE_H
#define _EQU_SURFACE_H

/**
 * @defgroup Equanime_Surface_Group Surfaces
 * @{
 */

typedef enum _Equ_Format
{
	EQU_FORMAT_ARGB8888,
	EQU_FORMAT_RGB888,
	EQU_FORMAT_RGB565,
	EQU_FORMAT_Ab31,
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

typedef struct _Equ_Surface Equ_Surface; /**< Opaque handler */
EAPI Equ_Surface_Type equ_surface_type_get(const Equ_Surface *);

#endif

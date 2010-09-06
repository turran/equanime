#include <stdint.h>

#include "Eina.h"
#include "equ_enums.h"
#include "equ_common.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI size_t equ_format_size_get(Equ_Format fmt, uint32_t w, uint32_t h)
{
	switch (fmt)
	{
		case EQU_FORMAT_RGB888:
		return w * h * 3;
		break;

		case EQU_FORMAT_ARGB8888:
		return w * h * 4;
		break;

		case EQU_FORMAT_RGB565:
		return w * h * 2;
		break;
	}
}

EAPI void equ_format_components_masks(Equ_Format fmt,
		uint32_t *r, uint32_t *g, uint32_t *b, uint32_t *a,
		uint32_t *y, uint32_t *u, uint32_t *v)
{
	switch (fmt)
	{
		case EQU_FORMAT_RGB888:
		*r = 0xff0000;
		*g = 0x00ff00;
		*b = 0x0000ff;
		*a = 0;
		break;

		case EQU_FORMAT_ARGB8888:
		*a = 0xff000000;
		*r = 0x00ff0000;
		*g = 0x0000ff00;
		*b = 0x000000ff;
		break;

		case EQU_FORMAT_RGB565:
		*r = 0x0000f800;
		*g = 0x000007e0;
		*b = 0x0000001f;
		*a = 0;
		break;
	}
}

EAPI size_t equ_format_pitch_get(Equ_Format fmt, uint32_t width)
{
	return equ_format_size_get(fmt, width, 1);
}

EAPI int equ_format_depth_get(Equ_Format fmt)
{
	switch (fmt)
	{
		case EQU_FORMAT_RGB888:
		return 24;
		break;

		case EQU_FORMAT_ARGB8888:
		return 32;
		break;

		case EQU_FORMAT_RGB565:
		return 16;
		break;
	}
}

#ifndef _EQU_SURFACE_H
#define _EQU_SURFACE_H

/**
 * @ingroup Equanime_Client
 * @defgroup Equanime_Surface_Group Surfaces
 * @{
 */

typedef struct _Equ_Surface Equ_Surface; /**< Opaque handler */

EAPI Equ_Surface_Type equ_surface_type_get(Equanime *e, const Equ_Surface *s);
EAPI Equ_Format equ_surface_format_get(Equanime *e, Equ_Surface *s);
EAPI void equ_surface_size_get(Equanime *e, Equ_Surface *s,
		unsigned int *w, unsigned int *h);

EAPI void equ_surface_pixels_upload(Equanime *e, Equ_Surface *s,
		Equ_Surface_Data *data, Eina_Rectangle *r);
EAPI void equ_surface_pixels_download(Equanime *e, Equ_Surface *s,
		Equ_Surface_Data *data, Eina_Rectangle *r);

/** @} */

#endif

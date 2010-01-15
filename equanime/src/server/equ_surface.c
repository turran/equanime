#include "Equanime.h"
#include "equanime_private.h"
/**
 * A surface ...
 * 
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Equ_Surface * equanime_surface_new(void)
{
	Equ_Surface *s;
	
	s = calloc(1, sizeof(Equ_Surface));
	/* TODO handle the magic */
	return s;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI Equ_Format equ_surface_format_get(Equ_Surface *s)
{
	return s->fmt;
}

EAPI void equ_surface_size_get(Equ_Surface *s, unsigned int *w, unsigned int *h)
{
	if (w) *w = s->w;
	if (h) *h = s->h;
}

EAPI Equ_Surface_Type equanime_surface_type_get(const Equ_Surface *s)
{
	return s->type;
}

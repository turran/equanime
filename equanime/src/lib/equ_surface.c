#include "Equanime.h"
#include "equ_private.h"
/**
 * A surface ...
 *
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Surface
{
	Equ_Common_Id id;

	Equ_Surface_Type type;
	Equ_Format fmt;
	uint32_t w;
	uint32_t h;

	unsigned int pitch;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Surface * equ_surface_new(Equ_Common_Id id, uint32_t w, uint32_t h,
		Equ_Format fmt, Equ_Surface_Type type)
{
	Equ_Surface *s;

	s = calloc(1, sizeof(Equ_Surface));
	s->id = id;
	s->w = w;
	s->h = h;
	s->fmt = fmt;
	s->type = type;

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

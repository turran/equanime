#include "Equ_Server.h"
/**
 * A surface ...
 *
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Surface
{
	Equ_Pool *pool;
	uint32_t w;
	uint32_t h;
	Equ_Surface_Type type;
	Equ_Format fmt;
	void *data;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Surface * equ_surface_new(Equ_Pool *p, uint32_t w, uint32_t h,
		Equ_Format fmt, Equ_Surface_Type type, void *data)
{
	Equ_Surface *s;

	s = calloc(1, sizeof(Equ_Surface));
	s->pool = p;
	s->fmt = fmt;
	s->w = w;
	s->h = h;
	s->data = data;
	s->type = type;

	return s;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_surface_delete(Equ_Surface *s)
{
	equ_pool_free(s->pool, s->data);
	free(s);
}

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

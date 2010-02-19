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
	Equ_Common_Id id;
#if 0
	Equ_Pool *pool;
#else
	Equ_Host *host;
	void *hd;
#endif
	uint32_t w;
	uint32_t h;
	Equ_Surface_Type type;
	Equ_Surface_Data data;
};

Eina_Hash *_surfaces = NULL;
static Equ_Common_Id _ids = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#if 0
Equ_Surface * equ_surface_new(Equ_Pool *p, uint32_t w, uint32_t h,
		Equ_Format fmt, Equ_Surface_Type type)
#else
Equ_Surface * equ_surface_new(Equ_Host *host, void *data,
		uint32_t w, uint32_t h,
		Equ_Format fmt, Equ_Surface_Type type)
#endif
{
	Equ_Surface *s;
#if 0
	Equ_Surface_Data data;

	/* first try to allocate the surface data */
	if (type != EQU_SURFACE_LOCAL)
	{
		switch (fmt)
		{
			case EQU_FORMAT_RGB888:
			data.data.rgb888.plane0 = equ_pool_alloc(p, w * h * 4);
			if (!data.data.rgb888.plane0) return NULL;
			break;
		}
	}
#endif
	s = calloc(1, sizeof(Equ_Surface));
#if 0
	s->pool = p;
	s->data = data;
#else
	s->host = host;
	s->hd = data;
#endif
	s->w = w;
	s->h = h;
	s->type = type;
	s->data.fmt = fmt;
	s->id = _ids++;
	/* allocate the surface data from the pools */
	if (!_surfaces)
	{
		_surfaces = eina_hash_int32_new(NULL);
	}
	eina_hash_add(_surfaces, &s->id, s);

	return s;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_surface_delete(Equ_Surface *s)
{
	/* TODO free very data's plane */
	//equ_pool_free(s->pool, s->data);
	free(s);
}

EAPI Equ_Common_Id equ_surface_id_get(Equ_Surface *s)
{
	return s->id;
}

EAPI Equ_Format equ_surface_format_get(Equ_Surface *s)
{
	return s->data.fmt;
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

EAPI Equ_Surface * equ_surface_get(Equ_Common_Id id)
{
	return eina_hash_find(_surfaces, &id);
}

EAPI void * equ_surface_data_get(Equ_Surface *s)
{
	return s->hd;
}

EAPI void equ_surface_pixels_upload(Equ_Surface *s,
	Equ_Surface_Data *data, Eina_Rectangle *r)
{
	equ_host_surface_upload(s->host, s->hd, data, r);
}

EAPI void equ_surface_pixels_download(Equ_Surface *s,
	Equ_Surface_Data *data, Eina_Rectangle *rect)
{
	equ_host_surface_download(s->host, s->hd, data, rect);
}

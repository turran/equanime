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
	Equ_Common_Surface common;
	Equ_Host *host;
	void *data;
	Equ_Surface_Data sdata;
};


Eina_Hash *_surfaces = NULL;
static Equ_Common_Id _ids = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Surface * equ_surface_new(Equ_Host *host, Equ_Common_Surface *common,
		void *data)
{
	Equ_Surface *s;

	s = calloc(1, sizeof(Equ_Surface));
	s->host = host;
	s->data = data;
	s->common = *common;
	s->common.id = _ids++;
	if (common->shid)
		s->common.shid = strdup(common->shid);
	/* allocate the surface data from the pools */
	if (!_surfaces)
	{
		_surfaces = eina_hash_int32_new(NULL);
	}
	eina_hash_add(_surfaces, &s->common.id, s);

	return s;
}

void * equ_surface_data_get(Equ_Surface *s)
{
	return s->data;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

EAPI Equ_Common_Id equ_surface_id_get(Equ_Surface *s)
{
	return s->common.id;
}

EAPI Equ_Format equ_surface_format_get(Equ_Surface *s)
{
	return s->sdata.fmt;
}

EAPI void equ_surface_size_get(Equ_Surface *s, unsigned int *w, unsigned int *h)
{
	if (w) *w = s->common.w;
	if (h) *h = s->common.h;
}

EAPI Equ_Surface_Type equanime_surface_type_get(const Equ_Surface *s)
{
	return s->common.type;
}

EAPI Equ_Surface * equ_surface_get(Equ_Common_Id id)
{
	return eina_hash_find(_surfaces, &id);
}

EAPI const char * equ_surface_shid_get(Equ_Surface *s)
{
	return s->common.shid;
}

EAPI void equ_surface_pixels_upload(Equ_Surface *s,
	Equ_Surface_Data *data, Eina_Rectangle *r)
{
#if 0
	equ_host_surface_upload(s->host, s, data, r);
#endif
}

EAPI void equ_surface_pixels_download(Equ_Surface *s,
	Equ_Surface_Data *data, Eina_Rectangle *rect)
{
#if 0
	equ_host_surface_download(s->host, s, data, rect);
#endif
}

EAPI void equ_surface_delete(Equ_Surface *s)
{
	equ_host_surface_delete(s->host, s);
	eina_hash_del(_surfaces, &s->common.id, s);
	if (s->common.shid) free(s->common.shid);
	free(s);
}


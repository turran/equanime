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
	uint32_t w;
	uint32_t h;
	Equ_Surface_Data data;
	unsigned int pitch;
	Eshm_Segment *segment;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Surface * equ_surface_new(Equ_Common_Id id, uint32_t w, uint32_t h,
		Equ_Format fmt, Equ_Surface_Type type, char *shid)
{
	Eshm_Segment *segment;
	Equ_Surface *s;
	void *data = NULL;

	switch (type)
	{
		case EQU_SURFACE_SYSTEM:
		segment = eshm_segment_get(shid, 0, EINA_FALSE);
		printf("segment = %p %s\n", segment, shid);
		if (!segment) return NULL;
		data = eshm_segment_data_get(segment);
		break;

		case EQU_SURFACE_HOST:
		/* TODO handle the mmaped area */
		return NULL;
		break;
	}

	s = calloc(1, sizeof(Equ_Surface));
	s->id = id;
	s->w = w;
	s->h = h;
	s->type = type;
	s->data.fmt = fmt;
	/* TODO add more formats */
	switch (fmt)
	{
		case EQU_FORMAT_RGB888:
		s->data.data.rgb888.plane0 = data;
		break;
	}

	return s;
}

Equ_Common_Id equ_surface_id_get(Equ_Surface *s)
{
	return s->id;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Equ_Format equ_surface_format_get(Equanime *e, Equ_Surface *s)
{
	return s->data.fmt;
}

EAPI void equ_surface_size_get(Equanime *e, Equ_Surface *s, unsigned int *w, unsigned int *h)
{
	if (w) *w = s->w;
	if (h) *h = s->h;
}

EAPI Equ_Surface_Type equ_surface_type_get(Equanime *e, const Equ_Surface *s)
{
	return s->type;
}
#if 0
/**
 * Upload pixels into a surface
 * @param[in] e The Equanime connection
 * @param[in] s The surface to upload the pixels to
 * @param[in] data The surface pixels container
 * @param[in] r The rectangle area of the surface to upload the pixels to
 */
EAPI void equ_surface_pixels_upload(Equanime *e, Equ_Surface *s,
	Equ_Surface_Data *data, Eina_Rectangle *r)
{
	Equ_Message_Surface_Upload m;
	int error;
	int i;

	if (!data) return;
	if (s->data.fmt != data->fmt) return;

	/* TODO in case of a local | shared buffer just write directly */
	/* serialize the data into a single buffer */
	switch (s->data.fmt)
	{
		case EQU_FORMAT_ARGB8888:
		m.pixels = data->data.argb8888.plane0;
		m.pixels_size = r->w * r->h * 4;
		break;

		case EQU_FORMAT_RGB888:
		m.pixels = data->data.rgb888.plane0;
		m.pixels_size = r->w * r->h * 3;
		break;

		case EQU_FORMAT_RGB565:
		case EQU_FORMAT_Ab31:
		case EQU_FORMAT_YUV422:
		return;
	}
	m.surface_id = s->id;
	m.sx = r->x;
	m.sy = r->y;
	m.sw = r->w;
	m.sh = r->h;
	error = equ_message_server_send(e, EQU_MSG_SURFACE_UPLOAD, &m, 0, (void **)&r);
}

/**
 * Download the pixels from the surface
 * @param[in] e The Equanime connection
 * @param[in] s The surface to download the pixels from
 * @param[out] data The surface pixels container to put the downloaded pixels on
 * @param[in] rect The rectangle area of the surface to download the pixels from
 */
EAPI void equ_surface_pixels_download(Equanime *e, Equ_Surface *s,
	Equ_Surface_Data *data, Eina_Rectangle *rect)
{
	Equ_Message_Surface_Download m;
	Equ_Reply_Surface_Download *r = NULL;
	int error;

	m.surface_id = s->id;
	m.sx = rect->x;
	m.sy = rect->y;
	m.sw = rect->w;
	m.sh = rect->h;

	/* TODO in case of a local | shared buffer just read directly */
	error = equ_message_server_send(e, EQU_MSG_SURFACE_DOWNLOAD, &m, 0, (void **)&r);
	if (error) return;

	/* de-serialize the raw pixels into the surface data */
	data->fmt = s->data.fmt;
	switch (s->data.fmt)
	{
		case EQU_FORMAT_ARGB8888:
		data->data.argb8888.plane0 = r->pixels;
		break;

		case EQU_FORMAT_RGB888:
		data->data.rgb888.plane0 = r->pixels;
		break;

		case EQU_FORMAT_RGB565:
		case EQU_FORMAT_Ab31:
		case EQU_FORMAT_YUV422:
		return;
	}
	free(r);
}
#endif

/**
 * Gets the locally modifiable surface data.
 * This function is only valid if the surface is local or shared
 * @param[in] e The Equanime connection
 * @param[in] s The surface to get the data from
 * @param[out] d The surface data where the modifiable pointers are stored
 */
EAPI Eina_Bool equ_surface_data_get(Equanime *e, Equ_Surface *s, Equ_Surface_Data *d)
{
	if (!d)
		return EINA_FALSE;

	*d = s->data;
	return EINA_TRUE;
}

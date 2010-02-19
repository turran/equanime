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
	s->type = type;
	s->data.fmt = fmt;
	/* in case of local surface we should allocate the data ourselves */
	if (s->type == EQU_SURFACE_LOCAL)
	{
		switch (fmt)
		{
			case EQU_FORMAT_RGB888:
			s->data.data.rgb888.plane0 = calloc(w * h, sizeof(uint32_t));
			break;
		}
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
	Equ_Error error;
	int i;

	if (!data) return;
	if (s->data.fmt != data->fmt) return;

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
	error = equ_message_server_send(e, EQU_MSG_TYPE_SURFACE_UPLOAD, &m, 0, (void **)&r);
}

/**
 * Download the pixels from the surface
 * @param[in] e The Equanime connection
 * @param[in] s The surface to download the pixels from
 * @param[in,out] data The surface pixels container to put the downloaded pixels on
 * @param[in] rect The rectangle area of the surface to download the pixels from
 */
EAPI void equ_surface_pixels_download(Equanime *e, Equ_Surface *s,
	Equ_Surface_Data *data, Eina_Rectangle *rect)
{
	Equ_Message_Surface_Download m;
	Equ_Reply_Surface_Download *r = NULL;
	Equ_Error error;

	m.surface_id = s->id;
	m.sx = rect->x;
	m.sy = rect->y;
	m.sw = rect->w;
	m.sh = rect->h;

	error = equ_message_server_send(e, EQU_MSG_TYPE_SURFACE_DOWNLOAD, &m, 0, (void **)&r);
	if (error) return;

	/* de-serialize the raw pixels into the surface data */
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

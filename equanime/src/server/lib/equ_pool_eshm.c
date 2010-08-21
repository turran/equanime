#include "Equ_Server.h"
#include "equ_server_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _last;
static Eina_Hash *_segments;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * equ_surface_eshm_alloc(Equ_Common_Surface *common)
{
	Eshm_Segment *segment;
	void *data;
	char id[16];
	size_t bytes;

	bytes = equ_format_size_get(common->fmt, common->w, common->h);
	snprintf(id, 16, "equeshm%08x", _last++);
	segment = eshm_segment_new(id, bytes);
	if (!segment) return NULL;

	if (!_segments) _segments = eina_hash_pointer_new(NULL);
	data = eshm_segment_data_get(segment);
	eina_hash_add(_segments, data, segment);

	common->shid = strdup(id);

	return data;
}

void equ_surface_eshm_free(void *data)
{
	Eshm_Segment *segment;

	segment = eina_hash_find(_segments, data);
	//eshm_segment_free(segment);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

#include "Equ_Server.h"
#include "equ_server_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _last;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eshm_Segment * equ_surface_eshm_alloc(Equ_Common_Surface *common)
{
	Eshm_Segment *segment;
	void *data;
	char id[16];
	size_t bytes;

	bytes = equ_format_size_get(common->fmt, common->w, common->h);
	snprintf(id, 16, "equeshm%08x", _last++);
	segment = eshm_segment_new(id, bytes);
	if (!segment) return NULL;
	common->shid = strdup(id);

	return segment;
}

void equ_surface_eshm_free(Eshm_Segment *segment)
{
	eshm_segment_delete(segment);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

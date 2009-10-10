#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"
/**
 * A region ...
 * The API is almost the same as the layer API
 * 
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define CHECK_FLAG(r, f) if (!(r->desc->flags & f)) return;
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI Equanime_Region_Description * equanime_region_description_get(Equanime_Layer *l)
{
	//return l->desc;
	return NULL;
}

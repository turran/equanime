#include "Equanime.h"
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
EAPI Equ_Region_Description * equ_region_description_get(Equ_Layer *l)
{
	//return l->desc;
	return NULL;
}

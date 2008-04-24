#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"
/**
 * A surface ...
 * 
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI const Enesim_Surface * equanime_surface_surface_get(const Equanime_Surface *s)
{
	return s->es;
}

EAPI Equanime_Surface_Type equanime_surface_type_get(const Equanime_Surface *s)
{
	return s->type;
}

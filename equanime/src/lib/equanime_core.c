#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static int _init = 0;
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void equanime_init(void)
{
	if (_init) return;
	
	_init++;
	equanime_module_load_all();
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void equanime_shutdown(void)
{
	if (_init == 1)
	{
		equanime_module_unload_all();
	}
	_init--;
}
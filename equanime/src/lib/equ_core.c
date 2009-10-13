#include "Equanime.h"
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
EAPI void equ_init(void)
{
	if (_init) return;

	_init++;
	equ_module_load_all();
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void equ_shutdown(void)
{
	if (_init == 1)
	{
		equ_module_unload_all();
	}
	_init--;
}

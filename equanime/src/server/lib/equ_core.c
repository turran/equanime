#include "Equ_Server.h"
#include "equ_server_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init = 0;
Eina_Array *_modules = NULL;

static void _module_init(void)
{
	char *mpath;
	char *path;

	path = getenv("EQUANIME_DIR");
	if (!path)
	{
		/* FIXME fix the leak */
		path = strdup("/usr/local/lib/equanime");
	}
	_modules = eina_module_list_get(_modules, path, 1, NULL, NULL);
	eina_module_list_load(_modules);
}

static void _module_shutdown(void)
{
	eina_module_list_free(_modules);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int equ_log = -1;
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int equ_init(void)
{
	if (_init) return _init;
	if (!eshm_init()) return 0;

	_init++;
	equ_log = eina_log_domain_register("equ", NULL);
	equ_common_init();
	equ_hal_i2c_init();
	_module_init();
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void equ_shutdown(void)
{
	if (_init == 1)
	{
		_module_shutdown();
		equ_hal_i2c_shutdown();
		equ_common_shutdown();
	}
	_init--;
}

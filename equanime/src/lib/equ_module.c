#include "Equanime.h"
#include "equanime_private.h"

#include <dirent.h>
#include <string.h>
#include <dlfcn.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Module
{
	Eina_Inlist list;
	Eina_Module *module;
	int type;
} Module;

enum
{
	EQU_MODULE_CONTROLLER,
	EQU_MODULE_LAYER,
	EQU_MODULES
};

static Module *_modules = NULL;

typedef int (*Module_Init)(void);
typedef void (*Module_Shutdown)(void);

void _module_load_all_cb(Eina_Module *em, void *data)
{
	Module_Init dl_mopen;
	Module *m;

	/* if the module isnt loaded correctly dont add it
	 * to the list of modules */
	dl_mopen = eina_module_symbol_get(em, "module_init");
	if (!dl_mopen)
	{
		eina_module_unload(em);
		return;
	}
	if (!dl_mopen())
	{
		eina_module_unload(em);
		return;
	}

	/* everything went ok, add it to the list of modules */
	m = malloc(sizeof(Module));
	m->type = *(int *)data;
	m->module = em;
	_modules = eina_inlist_append(_modules, m);
}

static void _module_unload(int type)
{
	Eina_Inlist *l;

	for (l = (Eina_Inlist*)_modules; l; l = l->next)
	{
		Module *m = (Module *)l;
		if (m->type == type)
		{
			Module_Shutdown sd;

			sd = eina_module_symbol_get(m->module, "module_exit");
			sd();
			printf("removing %d\n", type);
			eina_module_unload(m->module);
		}
	}
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void equ_module_load_all(void)
{
	char *mpath;
	char *path;
	int type;


	path = getenv("EQUANIME_DIR");
	if (!path)
	{
		path = strdup("/usr/local/lib/equanime");
	}
	/* find the directory where equanime is libequanime.so/../equanime/ */
#if 0
	Dl_info equ_dl;
	if (dladdr(equ_module_load_all, &equ_dl))
	{
		printf("path = %s\n", equ_dl.dli_fname);
	}
#endif
	mpath = malloc(strlen(path) + strlen("controller") + 1);
	mpath = strcat(path, "/controller");

	/* try to load every controller */
	printf("%s\n", mpath);
	type = EQU_MODULE_CONTROLLER;
	//eina_module_load_all(mpath, &_module_load_all_cb, &type);
	/* try to load every layer */
	//_module_load("/usr/local/lib/equ/layer", EQU_MODULE_LAYER);
}

void equ_module_unload_all(void)
{
	/* unload every loaded module */
	/* first the layers */
	_module_unload(EQU_MODULE_LAYER);
	/* then the controllers */
	_module_unload(EQU_MODULE_CONTROLLER);
}

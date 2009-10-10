#include <dirent.h>
#include <string.h>
#include <dlfcn.h>

#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"
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
	EQUANIME_MODULE_CONTROLLER,
	EQUANIME_MODULE_LAYER,
	EQUANIME_MODULES
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
void equanime_module_load_all(void)
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
	Dl_info equanime_dl;
	if (dladdr(equanime_module_load_all, &equanime_dl))
	{
		printf("path = %s\n", equanime_dl.dli_fname);
	}
#endif
	mpath = malloc(strlen(path) + strlen("controller") + 1);
	mpath = strcat(path, "/controller");
	
	/* try to load every controller */
	printf("%s\n", mpath);
	type = EQUANIME_MODULE_CONTROLLER;
	eina_module_load_all(mpath, &_module_load_all_cb, &type);
	/* try to load every layer */
	//_module_load("/usr/local/lib/equanime/layer", EQUANIME_MODULE_LAYER);	
}

void equanime_module_unload_all(void)
{
	/* unload every loaded module */
	/* first the layers */
	_module_unload(EQUANIME_MODULE_LAYER);
	/* then the controllers */
	_module_unload(EQUANIME_MODULE_CONTROLLER);
}

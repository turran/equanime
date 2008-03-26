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
typedef struct _Equanime_Module
{
	Eina_Inlist list;
	int type;
	void *handle;
	
} Equanime_Module;

enum
{
	EQUANIME_MODULE_CONTROLLER,
	EQUANIME_MODULE_LAYER,
	EQUANIME_MODULES
};

static Equanime_Module *_modules = NULL;

typedef int (*Module_Init)(void);
typedef void (*Module_Shutdown)(void);

/* change this to only receive the type */
static void _module_load(const char *path, int type)
{
	struct dirent *de;
	DIR *d;
	
	d = opendir(path);
	if (!d) return;
	
	while (de = readdir(d))
	{
		int length;
		
		length = strlen(de->d_name);
		if (length < 4) /* x.so */
			continue;
		if (!strcmp(de->d_name + length - 3, ".so"))
		{
			char file[PATH_MAX];
			void *dl_handle;
			Module_Init dl_mopen;
			Equanime_Module *m;
			
			snprintf(file, PATH_MAX, "%s/%s", path, de->d_name);
			
			dl_handle = dlopen(file, RTLD_LAZY);
			if (!dl_handle) continue;
			
			/* if the module isnt loaded correctly dont add it 
			 * to the list of modules */
			
			dl_mopen = dlsym(dl_handle, "module_init");
			if (!dl_mopen)
			{
				dlclose(dl_handle);
				continue;
			}
			if (!dl_mopen())
			{
				dlclose(dl_handle);
				continue;
			}
			
			/* everything went ok, add it to the list of modules */
			m = malloc(sizeof(Equanime_Module));
			m->type = type;
			m->handle = dl_handle;
			_modules = eina_inlist_append(_modules, m);
		}
	}
	closedir(d);	
}

static void _module_unload(int type)
{
	Eina_Inlist *l;
	
	for (l = (Eina_Inlist*)_modules; l; l = l->next)
	{
		Equanime_Module *m = (Equanime_Module *)l;
		if (m->type == type)
		{
			Module_Shutdown sd;
			
			sd = dlsym(m->handle, "module_exit");
			sd();
			printf("removing %d\n", type);
			dlclose(m->handle);
		}
	}
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void equanime_module_load_all(void)
{
	/* try to load every controller */
	_module_load("/usr/local/lib/equanime/controller", EQUANIME_MODULE_CONTROLLER);
	/* try to load every layer */
	_module_load("/usr/local/lib/equanime/layer", EQUANIME_MODULE_LAYER);	
}

void equanime_module_unload_all(void)
{
	/* unload every loaded module */
	/* first the layers */
	_module_unload(EQUANIME_MODULE_LAYER);
	/* then the controllers */
	_module_unload(EQUANIME_MODULE_CONTROLLER);
}

#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Equanime_Module
{
	
} Equanime_Module;

static Equanime_Module *modules = NULL;

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void equanime_module_load_all(void)
{
	/* try to load every controller */
	/* then load every layer module */
	/* if the module isnt loaded correctly dont add it to the list of
	 * modules */	
}

void equanime_module_unload_all(void)
{
	/* unload every loaded module */
	/* first the controllers */
	/* then the layers */
}

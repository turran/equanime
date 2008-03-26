#include "Eina.h"
#include "Equanime.h"
#include "Equanime_Module.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _remove(Equanime_Layer *l)
{
	
}

static int _probe(Equanime_Layer *l)
{
	
}

static Equanime_Layer_Description mp25xxf_rgb_description = 
{
	.cname = "MagicEyes MP25XXF",
	.name = "RGB",
	.flags = EQUANIME_LAYER_VISIBILITY | EQUANIME_LAYER_POSITION | 
		EQUANIME_LAYER_SIZE | EQUANIME_LAYER_LEVEL,
};

static Equanime_Layer_Functions mp25xxf_rgb_functions =
{
	.probe = &_probe,
	.remove = &_remove,		
};
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
int module_init(void)
{
	/* register the new layer */
	return equanime_layer_register(&mp25xxf_rgb_description, &mp25xxf_rgb_functions);
}

void module_exit(void)
{
	/* unregister the layer */
	equanime_layer_unregister(&mp25xxf_rgb_description);
}

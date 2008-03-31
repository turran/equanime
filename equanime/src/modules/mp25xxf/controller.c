#include <stdlib.h>
#include <stdio.h>

#include "Eina.h"
#include "Equanime.h"
#include "Equanime_Module.h"

#include "mp25xxf.h"
/**
 * The size of the layers is determined by the display controller configuration
 * we should map/unmap the memory of each layer if the size changes, maybe
 * we need a driver that allcoates/deallocates dma memory regions in kernel?
 * 
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define DRIVER_NAME "mp25xxf_mlc"

enum
{
	MP25XXF_RGB,
	MP25XXF_LAYERS
};

typedef struct _Layer Layer;
typedef struct _Controller Controller;

/**
 * Layer definition
 */
struct _Layer
{
	void *addr;
	int id;
	Controller *c;
};

/**
 * Controller definition
 */
struct _Controller
{
	Equanime_Hal_Device *device;
	Layer *layers[MP25XXF_LAYERS];
	void *regs;
};


/*============================================================================*
 *                               Controller                                   * 
 *============================================================================*/
static int controller_probe(Equanime_Controller *ec)
{
	Controller *c;
		
	c = malloc(sizeof(Controller));
	/* check if the driver exists */
	c->device = equanime_hal_uio_open(DRIVER_NAME);
	if (!c->device)
	{
		free(c);
		return 0;
	}
	/* map the registers */
	c->regs = equanime_hal_uio_map(c->device, 0);

	equanime_controller_data_set(ec, c);
	return 1;
}

static void controller_remove(Equanime_Controller *ec)
{
	Controller *c;
			
	/* unregister the controller */
	c = equanime_controller_data_get(ec);
	/* close the device */
	equanime_hal_uio_close(c->device);
	free(c);
}

static Equanime_Controller_Description mp25xxf_description = 
{
	.name = "MagicEyes MP25XXF",
};

static Equanime_Controller_Functions mp25xxf_functions =
{
	.probe = &controller_probe,
	.remove = &controller_remove,		
};

/*============================================================================*
 *                                  Layer                                     * 
 *============================================================================*/
static int layer_probe(Equanime_Layer *el)
{
	Layer *l;
	Controller *c;
	Equanime_Layer_Description *ld;
	
	c = equanime_controller_data_get(equanime_layer_controller_get(el));
	
	l = malloc(sizeof(Layer));
	/* check the name and match it to the id */
	ld = equanime_layer_description_get(el);
	printf("probing\n");
	if (!strcmp(ld->name, "RGB"))
	{
		
		l->id = MP25XXF_RGB;
		/* map the memory */
		l->addr = equanime_hal_uio_map(c->device, 1);
		printf("l->addr = %x\n", l->addr);
	}
	else
	{
		/* WTF? */
		free(l);
		return 0;
	}
	equanime_layer_data_set(el, l);

	return 1;
}

static void layer_remove(Equanime_Layer *el)
{
	Layer *l;
	
	l = equanime_layer_data_get(el);
	l->c->layers[l->id] = NULL;
	free(l);
}

static void * layer_ptr_get(Equanime_Layer *el)
{
	Layer *l;
			
	l = equanime_layer_data_get(el);
	return l->addr;
}

static int layer_visibility_set(Equanime_Layer *el, int show)
{
	Layer *l;
	
	l = equanime_layer_data_get(el);
	/* TODO enable/disable all regions? */
	return 1;
}

static Equanime_Layer_Description mp25xxf_rgb_description = 
{
	.cname = "MagicEyes MP25XXF",
	.name = "RGB",
	.flags = EQUANIME_LAYER_VISIBILITY,
};

static Equanime_Layer_Functions mp25xxf_layer_functions =
{
	.probe = &layer_probe,
	.remove = &layer_remove,
	.ptr_get = &layer_ptr_get,
};
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
int module_init(void)
{
	printf("initializing\n");
	if (!equanime_controller_register(&mp25xxf_description, &mp25xxf_functions))
		goto err_controller;
	/* register the layers */
	if (!equanime_layer_register(&mp25xxf_rgb_description, &mp25xxf_layer_functions))
		goto err_layer0;
#if 0
	if (!equanime_layer_register(&mp25xxf_video1_description, &mp25xxf_layer_functions))
		goto err_layer1;
	if (!equanime_layer_register(&mp25xxf_rgb0_description, &mp25xxf_layer_functions))
		goto err_layer2;
	if (!equanime_layer_register(&mp25xxf_rgb1_description, &mp25xxf_layer_functions))
		goto err_layer3;
#endif
	return 1;

#if 0
err_layer3:
	equanime_layer_unregister(&mp25xxf_rgb0_description);
err_layer2:
	equanime_layer_unregister(&mp25xxf_video1_description);
err_layer1:
	equanime_layer_unregister(&mp25xxf_video0_description);
#endif
err_layer0:
	equanime_controller_unregister(&mp25xxf_description);
err_controller:
	return 0;

}

void module_exit(void)
{
	equanime_controller_unregister(&mp25xxf_description);
}

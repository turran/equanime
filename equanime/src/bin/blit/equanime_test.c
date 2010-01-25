#include <stdio.h>

#include "Eina.h"
#include "Enesim.h"
#include "Equanime.h"

/* This example should get a host
 * get a controller
 * get a layer
 * get a surface
 * and start blitting this surface into the layer every n seconds
 */

Equanime_Layer *rgb = NULL;
Equanime_Layer *video = NULL;

const char *layer_flags[] = {
	"VISIBILITY",
	"POSITION",
	"SIZE",
	"LEVEL",
};

int _layer_cb(Equanime_Layer *l, void *data)
{
	Equanime_Layer_Description *ld;
	
	ld = equanime_layer_description_get(l);
	/* check if it supports RGB, if so, setup the correct
	 * layer configuration */
	rgb = ld;
	 
	return 1;
}

int _controller_cb(Equanime_Controller *c, void *data)
{
	Equanime_Controller_Description *cd;
	
	cd = equanime_controller_description_get(c);
	equanime_controller_layers_get(c, (Equanime_Cb)_layer_cb, NULL);
	
	return 1;
}

static void _play(void)
{
	Equanime_Surface *es;
	Enesim_Surface *s;
	
	/* if we have an rgb layer */
	es = equanime_layer_surface_get(l);
	s = equanime_surface_enesim_surface_get(s);
	/* do some tests */
	/* if we have a yuv layer */
}

/**
 * List all controllers, layers, inputs and ouputs.
 */
int main(void)
{
	int num_controller = 0;
	
	equanime_init();
	equanime_controllers_get((Equanime_Cb)_controller_cb, &num_controller);
	/* lets play with the layers */
	_play();
	equanime_shutdown();
	return 0;
}

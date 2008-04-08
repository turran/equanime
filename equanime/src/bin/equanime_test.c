#include <stdio.h>

#include "Eina.h"
#include "Equanime.h"

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
	/* check if it supports RGB */
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
	/* if we have an rgb layer */
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
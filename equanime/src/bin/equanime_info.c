#include <stdio.h>

#include "Eina.h"
#include "Equanime.h"

static void _layer_desc_dump(Equanime_Layer_Description *ld)
{
	printf("\t- name = %s\n", ld->name);
}

int _layer_cb(Equanime_Layer *l, void *data)
{
	int *num = data;
	
	printf("Layer %d\n", *num);
	*num = *num + 1;
	_layer_desc_dump(equanime_layer_description_get(l));
	
	return 1;
}


static void _controller_desc_dump(Equanime_Controller_Description *cd)
{
	printf("\t- name = %s\n", cd->name);
}

int _controller_cb(Equanime_Controller *c, void *data)
{
	int *num = data;
	int num_layer = 0;
	
	printf("Controller %d\n", *num++);
	_controller_desc_dump(equanime_controller_description_get(c));
	equanime_controller_layers_get(c, (Equanime_Cb)_layer_cb, &num_layer);
	
	return 1;
}


/**
 * List all controllers, layers, inputs and ouputs.
 */
int main(void)
{
	int num_controller = 0;
	
	equanime_init();
	equanime_controllers_get((Equanime_Cb)_controller_cb, &num_controller);
	equanime_shutdown();
	return 0;
}

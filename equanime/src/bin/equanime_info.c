#include "Equanime.h"

void _layer_cb(Equanime_Layer *l, void *data)
{
	int *num = data;
	printf("Layer %dn", *num++);
}

void _controller_cb(Equanime_Controller *c, void *data)
{
	int *num = data;
	int num_layer = 0;
	printf("Controller %d\n", *num++);
	equanime_controller_layers_get(c, _layer_cb, &num_layer);
}


/**
 * List all controllers, layers, inputs and ouputs.
 */
int main(void)
{
	int num_controller = 0;
	
	equanime_init();
	equanime_controllers_get(_controller_cb, &num_controller);
	equanime_shutdown();
	return 0;
}

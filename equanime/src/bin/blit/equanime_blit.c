#include <stdio.h>
#include "Equanime.h"

Equanime *eq;

int main(void)
{
	int num_host;
	Equ_Host *h;
	Equ_Controller *c;
	Equ_Layer *l;
	Equ_Layer_Status status;
	Equ_Layer_Status caps;

	equ_init();
	eq = equ_new(0xea);
	/* get the default host */
	h = equ_host_get(eq, NULL);
	if (!h) return 1;
	/* get the default controller */
	c = equ_host_controller_get(eq, h, NULL);
	if (!c) return 1;
	/* get the default layer */
	l = equ_controller_layer_get(eq, c, NULL);
	if (!l) return 1;

	printf("host %s\n", equ_host_name_get(eq, h));
	printf("controller %s\n", equ_controller_name_get(eq, c));
	printf("layer %s\n", equ_layer_name_get(eq, l));

	/* get the format of the layer */
	//equ_layer_caps_get(eq, l, &caps);
	equ_layer_status_get(eq, l, &status);
	/* get a surface */
	/* put it on x,y */

	equ_shutdown();

	return 0;
}

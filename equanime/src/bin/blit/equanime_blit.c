#include <stdio.h>
#include "Equanime.h"

Equanime *eq;

static void _surface_info(Equ_Surface *s)
{
	Equ_Surface_Type t;
	int w, h;

	equ_surface_size_get(s, &w, &h);
	t = equ_surface_type_get(s);
	printf("surface = %p w = %d, h = %d type = %d\n", s, w, h, t);
}

int main(void)
{
	int num_host;
	Equ_Host *h;
	Equ_Controller *c;
	Equ_Layer *l;
	Equ_Layer_Status status;
	Equ_Layer_Status caps;
	Equ_Surface *s;
	Eina_Rectangle rect;

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
	equ_layer_status_get(eq, l, &status);
	/* get a surface */
	eina_rectangle_coords_from(&rect, 0, 0, 160, 120);
	s = equ_host_surface_get(eq, h, rect.w, rect.h, status.fmt, EQU_SURFACE_LOCAL);
	if (!s) goto end;

	_surface_info(s);
	/* put it on x,y */
	equ_layer_surface_put(eq, l, s, 0, 0, &rect);
	equ_sync(eq);
end:
	ecore_main_loop_begin();
	equ_shutdown();

	return 0;
}

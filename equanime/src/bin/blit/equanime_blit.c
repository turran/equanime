#include <stdio.h>
#include "Equanime.h"

Equanime *eq;

static void _surface_info(Equ_Surface *s)
{
	Equ_Surface_Type t;
	int w, h;

	equ_surface_size_get(eq, s, &w, &h);
	t = equ_surface_type_get(eq, s);
	printf("surface = %p w = %d, h = %d type = %d\n", s, w, h, t);
}

static void _surface_fill(Equ_Surface *s)
{
	Eina_Rectangle rect;
	Equ_Surface_Data data;

	rect.x = 0;
	rect.y = 0;
	rect.w = 0;
	rect.h = 0;
	/* create a simple pattern */
	/* upload the surface data */
	//equ_surface_pixels_upload(eq, s, &data, &rect);
}

static void _help(void)
{
	printf("equanime_blit [x y [w h]]\n");
}

int main(int argc, char **argv)
{
	int num_host;
	Equ_Host *h;
	Equ_Controller *c;
	Equ_Layer *l;
	Equ_Layer_Status status;
	Equ_Layer_Status caps;
	Equ_Surface *s;
	Eina_Rectangle rect;
	int x = 0, y = 0, width = 160, height  = 120;

	if (argc >= 3)
	{
		x = atoi(argv[1]);
		y = atoi(argv[2]);
		if (argc == 5)
		{
			width = atoi(argv[3]);
			height = atoi(argv[4]);
		}
		else if (argc > 5)
		{
			_help();
			return 0;
		}
	}
	else if (argc != 1)
	{
		_help();
		return 0;
	}

	equ_init();
	eq = equ_new(0xea);
	if (!eq) return 1;

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
	eina_rectangle_coords_from(&rect, 0, 0, width, height);
	//s = equ_host_surface_get(eq, h, rect.w, rect.h, status.fmt, EQU_SURFACE_LOCAL);
	s = equ_host_surface_get(eq, h, rect.w, rect.h, status.fmt, EQU_SURFACE_SHARED);
	if (!s) goto end;

	_surface_info(s);
	_surface_fill(s);
	/* put it on x,y */
	//equ_layer_surface_put(eq, l, s, x, y, &rect);
	equ_sync(eq);
end:
	ecore_main_loop_begin();
	equ_shutdown();

	return 0;
}

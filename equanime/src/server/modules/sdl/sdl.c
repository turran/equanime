#include "Equ_Server.h"
#include "SDL.h"

/* The SDL module has only one controller and
 * one output
 */
typedef struct _SDL
{
	SDL_Surface *surface;
	Equ_Controller *controller;
	Equ_Layer *layer;
	Equ_Output *output;
} SDL;

static Eina_Bool _output_mode_set(Equ_Output *o, Equ_Mode *m)
{
	return EINA_TRUE;
}

static Equ_Output_Backend _obackend = {
	.mode_set = _output_mode_set,
};

static Eina_Bool _controller_output_set(Equ_Controller *c, Equ_Output *o)
{
	return EINA_TRUE;
}

static Equ_Controller_Backend _cbackend = {
	.output_set = _controller_output_set,
};

static Equ_Layer_Backend _lbackend = {

};

static Eina_Bool _host_init(Equ_Host *h)
{
	SDL *sdl;
	Uint32 flags = 0;
	SDL_Surface *s;

	sdl = malloc(sizeof(SDL));
	SDL_Init(SDL_INIT_VIDEO);
	sdl->surface = SDL_SetVideoMode(320, 240, 32, flags);
	sdl->controller = equ_host_controller_register(h,
			"controller0", &_cbackend);
	sdl->output = equ_controller_output_register(sdl->controller,
			"output0", &_obackend);
	sdl->layer = equ_controller_layer_register(sdl->controller,
			"layer0", &_lbackend);
	equ_host_data_set(h, sdl);

	return EINA_TRUE;
}

static void _host_shutdown(Equ_Host *h)
{
	SDL *sdl;

	sdl = equ_host_data_get(h);
	equ_layer_unregister(sdl->layer);
	equ_output_unregister(sdl->output);
	equ_controller_unregister(sdl->controller);
	free(sdl);
	equ_host_unregister(h);
}

Equ_Host_Backend _hbackend = {
	.init = _host_init,
	.shutdown = _host_shutdown,
};

static Eina_Bool module_init(void)
{
	equ_host_register("sdl", &_hbackend);
	return EINA_TRUE;
}

static void module_shutdown(void)
{
}

EINA_MODULE_INIT(module_init);
EINA_MODULE_SHUTDOWN(module_shutdown);

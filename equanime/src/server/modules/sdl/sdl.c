#include "Equ_Server.h"
#include "SDL.h"

#include "Ecore.h"

/* The SDL module has only one controller and
 * one output
 */
typedef struct _SDL
{
	SDL_Surface *surface;
	Equ_Controller *controller;
	Equ_Layer *layer;
	Equ_Output *output;
	Equ_Server_Backend *server;
	Ecore_Timer *timer;
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
	/* FIXME put the layer size set callback */
};

static int _events_cb(void *data)
{
	SDL_Event event;
	SDL *sdl = data;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			sdl->server->quit();
			break;
		}
	}
	return 1;
}

static inline void _layer_setup(SDL *sdl)
{
	Equ_Layer_Caps caps = {
		.flags_mask = EQU_LAYER_SIZE,
	};
	Equ_Layer_Status status = {
		.w = 320,
		.h = 240,
		.fmt = EQU_FORMAT_RGB888,
	};

	sdl->layer = equ_controller_layer_register(sdl->controller,
			"layer0", &_lbackend, &caps, &status);
}

static Eina_Bool _host_init(Equ_Host *h, Equ_Server_Backend *sbackend)
{
	SDL *sdl;
	Uint32 flags = SDL_RESIZABLE;
	SDL_Surface *s;

	sdl = malloc(sizeof(SDL));
	SDL_Init(SDL_INIT_VIDEO);
	sdl->surface = SDL_SetVideoMode(320, 240, 32, flags);
	sdl->controller = equ_host_controller_register(h,
			"controller0", &_cbackend);
	sdl->output = equ_controller_output_register(sdl->controller,
			"output0", &_obackend);

	/* setup the timer callback to check for events */
	sdl->server = sbackend;
	sdl->timer = ecore_timer_add(0.005, _events_cb, sdl);
	_layer_setup(sdl);
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

#include "Equ_Server.h"
#include "equ_server_private.h"
#include "SDL.h"

#include "Ecore.h"

#define EQU_LOG_DOM _log_dom
static int _log_dom = -1;

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
	/* options */
	Eina_Bool resizable;
	int width;
	int height;
} SDL;

typedef struct _Surface
{
	SDL_Surface *surface;
	Eshm_Segment *segment;
} Surface;

static Equ_Option _options[] = {
	{ "0", "resizable", "Make the SDL window resizable"},
	{ "320", "width", "Width of the window"},
	{ "240", "height", "Height of the window"},
	NULL,
};

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


static inline void _info_to_status(SDL *sdl, Equ_Layer_Status *status)
{
	memset(status, 0, sizeof(Equ_Layer_Status));

	status->w = sdl->width;
	status->h = sdl->height;
	status->fmt = EQU_FORMAT_RGB888;
}

static Eina_Bool _events_cb(void *data)
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

			case SDL_VIDEORESIZE:
			{
				Equ_Layer_Status status;

				sdl->width = event.resize.w;
				sdl->height = event.resize.h;
				_info_to_status(sdl, &status);
				equ_layer_status_update(sdl->layer, &status);
			}
			break;
		}
	}
	return ECORE_CALLBACK_RENEW;
}

static void _layer_surface_put(Equ_Layer *l, Equ_Surface *s, int x, int y,
		Eina_Rectangle *rect)
{
	SDL *sdl;
	SDL_Surface *src;
	Equ_Host *h;
	SDL_Rect srect, drect;

	h = equ_layer_host_get(l);
	sdl = equ_host_data_get(h);
	src = equ_surface_data_get(s);
	srect.x = rect->x;
	srect.y = rect->y;
	srect.w = rect->w;
	srect.h = rect->h;

	drect.x = x;
	drect.y = y;
	drect.w = rect->w;
	drect.h = rect->h;

	SDL_BlitSurface(src, &srect, sdl->surface, &drect);
	SDL_UpdateRect(sdl->surface, drect.x, drect.y, drect.w, drect.h);
}

static Equ_Layer_Backend _lbackend = {
	/* FIXME put the layer size set callback */
	.surface_put = _layer_surface_put,
};

static inline void _layer_setup(SDL *sdl)
{
	Equ_Layer_Caps caps;
	Equ_Layer_Status status;

	memset(&caps, 0, sizeof(Equ_Layer_Caps));

	if (sdl->resizable)
		caps.flags_mask = EQU_LAYER_SIZE;

	_info_to_status(sdl, &status);

	sdl->layer = equ_controller_layer_register(sdl->controller,
			"layer0", &_lbackend, &caps, &status);
}

static Eina_Bool _host_init(Equ_Host *h, Equ_Server_Backend *sbackend,
		const char *options)
{
	SDL *sdl;
	Uint32 flags = 0;
	SDL_Surface *s;

	sdl = malloc(sizeof(SDL));
	/* set the default options */
	sdl->width = 320;
	sdl->height = 240;
	sdl->resizable = EINA_FALSE;
	/* parse the options */
	equ_option_parse(&_options[0], (char *)options, EQU_OPTION_BOOL,
			&sdl->resizable);
	if (sdl->resizable)
		flags |= SDL_RESIZABLE;
	/* initialize sdl */
	SDL_Init(SDL_INIT_VIDEO);
	sdl->surface = SDL_SetVideoMode(sdl->width, sdl->height, 32, flags);
	sdl->controller = equ_host_controller_register(h,
			"controller0", &_cbackend);
	sdl->output = equ_controller_output_register(sdl->controller,
			"output0", &_obackend);

	/* setup the timer callback to check for events */
	sdl->server = sbackend;
	sdl->timer = ecore_timer_add(0.005, _events_cb, sdl);
	/* layer setup */
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


static Equ_Option * _host_options(Equ_Host *h)
{
	return _options;
}

static void _host_surface_upload(Equ_Host *h, Equ_Surface *s, Equ_Surface_Data *data, Eina_Rectangle *rect)
{
	/* lock the surface */
	/* put the pixels at rect */
	/* unlock the surface */
}

static void _host_surface_download(Equ_Host *h, Equ_Surface *s, Equ_Surface_Data *data, Eina_Rectangle *rect)
{
	/* lock the surface */
	/* get the pixels from rect */
	/* unlock the surface */
}

static void * _host_surface_new(Equ_Host *h, Equ_Common_Surface *common)
{
	SDL *sdl;
	Uint32 rm, gm, bm, am;
	int depth;
	int pitch;
	Equ_Surface *s;
	void *data = NULL;
	void *shdata = NULL;
	size_t bytes;

	sdl = equ_host_data_get(h);
	depth = equ_format_depth_get(common->fmt);

	shdata = equ_surface_eshm_alloc(common);
	if (!shdata)
	{
		ERR("Cannot allocate %d", bytes);
		return NULL;
	}
	pitch = equ_format_pitch_get(common->fmt, common->w);
	equ_format_components_masks(common->fmt, &rm, &gm, &bm, &am, NULL, NULL, NULL);
	data = SDL_CreateRGBSurfaceFrom(shdata,
			common->w, common->h, depth, pitch,
			rm, gm, bm, am);

	return data;
}

static void _host_surface_delete(Equ_Host *h, Equ_Surface *s)
{
	SDL_Surface *surface = equ_surface_data_get(s);

	SDL_FreeSurface(surface);
}

Equ_Host_Backend _hbackend = {
	.init = _host_init,
	.shutdown = _host_shutdown,
	.options_get = _host_options,
	.surface_new = _host_surface_new,
	.surface_delete = _host_surface_delete,
	.surface_upload = _host_surface_upload,
	.surface_download = _host_surface_download,
};

static Eina_Bool module_init(void)
{
	_log_dom = eina_log_domain_register("Equ_SDL", NULL);
	DBG("Initializing");
	equ_host_register("sdl", &_hbackend);
	return EINA_TRUE;
}

static void module_shutdown(void)
{
	DBG("Shutting down");
}

EINA_MODULE_INIT(module_init);
EINA_MODULE_SHUTDOWN(module_shutdown);

#include "Equ_Server.h"

typedef struct _SDL
{
	Equ_Controller *c;
} SDL;

static SDL *sdl = NULL;

static Eina_Bool _output_set(Equ_Controller *c, Equ_Output *o)
{
	struct sdl *sdl = equ_controller_data_get(c);
	return EINA_TRUE;
}

static Equ_Controller_Backend _controller =
{
	.output_set = _output_set,
};

static Eina_Bool _output_mode_set(Equ_Output *o, Equ_Mode *m)
{
	/* TODO set register values for the ths7313? */
	return EINA_TRUE;
}

static Equ_Output_Backend _output =
{
	.mode_set = _output_mode_set,
};

static Eina_Bool module_init(void)
{
	Equ_Host *h;

	h = equ_host_register("sdl", NULL);

	return EINA_TRUE;
}

static void module_shutdown(void)
{
	//equ_controller_unregister(sdl);
}

EINA_MODULE_INIT(module_init);
EINA_MODULE_SHUTDOWN(module_shutdown);

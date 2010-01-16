#include "Equ_Server.h"
/**
 * A layer is often known as a plane on a video controller. The contorller then
 * blends all layers and sends the result through the video encoder signal
 *
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define CHECK_FLAG(l, f) if (!(l->flags & f)) return;
Equ_Region *_regions = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Layer * equ_layer_new(Equ_Controller *c, Equ_Layer_Backend *b,
		const char *name, void *data)
{
	Equ_Layer *l;

	l = malloc(sizeof(Equ_Layer));
	l->backend = b;
	l->controller = c;
	l->data = data;
	l->name = name;

	return l;
}

void * equ_layer_data_get(Equ_Layer *l)
{
	return l->data;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 *
 */
EAPI const char * equ_layer_name_get(Equ_Layer *l)
{
	return l->name;
}
/**
 *
 */
EAPI void equ_layer_surface_set(Equ_Layer *l, Equ_Surface *s)
{

}

/**
 * Get the layer surface.
 * TODO What happens if a layer changes resolution, size, format ?
 * we should check that the surface is released?
 */
EAPI Equ_Surface * equ_layer_surface_get(Equ_Layer *l)
{
	Equ_Surface *s;
#if 0
	if (!l->surface)
	{
		Enesim_Surface *es;
		Enesim_Surface_Data sdata;

		s = equ_surface_new();
		l->surface = s;
		/* switch for every format */
		switch (l->format)
		{
		/* Enesim_Surface_Format f, int w, int h, Enesim_Surface_Data *sdata */
		case ENESIM_SURFACE_ARGB8888:
			sdata.argb8888.plane0 = l->fncs->ptr_get(l);
			break;
		case ENESIM_SURFACE_ARGB8888_PRE:
			sdata.argb8888_pre.plane0 = l->fncs->ptr_get(l);
			break;
		case ENESIM_SURFACE_RGB565:
			sdata.rgb565.plane0 = l->fncs->ptr_get(l);
			sdata.rgb565.plane1 = malloc(sizeof(*sdata.rgb565.plane1) * l->w * l->h);
			break;
		case ENESIM_SURFACE_RGB888:
			sdata.rgb888.plane0 = l->fncs->ptr_get(l);
			sdata.rgb888.plane1 = malloc(sizeof(*sdata.rgb8888.plane1) * l->w * l->h);
			break;
		case ENESIM_SURFACE_A8:
			sdata.a8.plane0 = l->fncs->ptr_get(l);
			break;
		}
		es = enesim_surface_new(l->format, l->w, l->h, &sdata);
	}
	l->surface_ref++;
#endif

	return s;
}

/**
 *
 */
EAPI void equ_layer_regions_get(Equ_Layer *l, void *cb, void *cb_data)
{

}

/**
 *
 */
EAPI void equ_layer_size_get(Equ_Layer *l, int *w, int *h)
{
	if (!l->surface)
	{
		if (w) *w = 0;
		if (h) *h = 0;

		return;
	}
	equ_surface_size_get(l->surface, w, h);
}

/**
 *
 */
EAPI void equ_layer_position_get(Equ_Layer *l, int *x, int *y)
{
	if (x) *x = l->x;
	if (y) *x = l->y;
}
/**
 *
 *
 */
EAPI void equ_layer_position_set(Equ_Layer *l, int x, int y)
{
	CHECK_FLAG(l, EQU_LAYER_POSITION)
	if ((x == l->x) && (y == l->y))
		return;
	if (l->backend->position_set(l, x, y))
	{
		l->x = x;
		l->y = y;
	}
	l->x = x;
	l->y = y;
}
/**
 *
 */
EAPI void equ_layer_level_get(Equ_Layer *l, unsigned int *level)
{
	if (level) *level = l->level;
}
/**
 *
 */
EAPI void equ_layer_level_set(Equ_Layer *l, unsigned int level)
{
	CHECK_FLAG(l, EQU_LAYER_LEVEL)
	/* check if the layer with level == level is active or visible */
	/* if so hide it */
	/* call the function */
	/* update the value */
	l->level = level;
	/* if there was another layer with the new level
	 *  update that with the current */
}
/**
 *
 */
EAPI void equ_layer_level_up(Equ_Layer *l)
{
	/* call level set with l->level + 1 */
}
/**
 *
 */
EAPI void equ_layer_level_down(Equ_Layer *l)
{
	/* call level set with l->level - 1 */
}
/**
 *
 */
EAPI void equ_layer_hide(Equ_Layer *l)
{
	CHECK_FLAG(l, EQU_LAYER_VISIBILITY)
	if (l->hidden) return;
	if (l->backend->visibility_set(l, 0))
		l->hidden = 1;
}
/**
 *
 */
EAPI void equ_layer_show(Equ_Layer *l)
{
	CHECK_FLAG(l, EQU_LAYER_VISIBILITY)
	if (!l->hidden) return;
	if (l->backend->visibility_set(l, 1))
		l->hidden = 0;
}
/**
 *
 */
EAPI Eina_Bool equ_layer_is_visible(Equ_Layer *l)
{
	return l->hidden;
}

/**
 *
 */
EAPI Equ_Controller * equ_layer_controller_get(Equ_Layer *l)
{
	return l->controller;
}

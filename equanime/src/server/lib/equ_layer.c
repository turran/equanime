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

struct _Equ_Layer
{
	Equ_Controller *controller;
	const Equ_Layer_Backend *backend;
	void *data;

	Equ_Layer_Info info;
	Equ_Layer_Caps caps;
	Equ_Layer_Status status;
	/* FIXME remove this */
	int flags; /** Layer flags */
	const int *formats; /** Supported pixel formats */
	int x;
	int y;
	int level;
	unsigned char hidden;
	Equ_Surface *surface;
};

Eina_Hash *_layers = NULL;
static Equ_Region *_regions = NULL;
static Equ_Common_Id _ids = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Layer * equ_layer_new(Equ_Controller *c,
		const char *name, Equ_Layer_Backend *b)
{
	Equ_Layer *l;
	
	if (!_layers)
		_layers = eina_hash_int32_new(NULL);


	l = malloc(sizeof(Equ_Layer));
	l->backend = b;
	l->controller = c;
	l->info.name = name;
	l->info.id = _ids++;
	eina_hash_add(_layers, &l->info.id, l);

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
EAPI void equ_layer_unregister(Equ_Layer *l)
{
	equ_controller_layer_unregister(l->controller, l);
}
/**
 *
 */
EAPI Equ_Layer * equ_layer_get(Equ_Common_Id id)
{
	return eina_hash_find(_layers, &id);
}
/**
 *
 */
EAPI const char * equ_layer_name_get(Equ_Layer *l)
{
	return l->info.name;
}

EAPI Equ_Common_Id equ_layer_id_get(Equ_Layer *l)
{
	return l->info.id;
}

EAPI void equ_layer_status_get(Equ_Layer *l, Equ_Layer_Status *status)
{
	if (!status)
		return;
	*status = l->status;
}

EAPI void equ_layer_caps_get(Equ_Layer *l, Equ_Layer_Caps *caps)
{
	if (!caps)
		return;
	*caps = l->caps;
}

EAPI void equ_layer_format_set(Equ_Layer *l, Equ_Format fmt)
{
	/* check the capabilities */
	/* call the backend implementation */
}


EAPI void equ_layer_size_set(Equ_Layer *l, uint32_t w, uint32_t h)
{


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

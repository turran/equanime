#include "Equanime.h"
#include "equ_private.h"
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
	Equ_Layer_Info info;
	Equ_Layer_Caps caps;
	Equ_Layer_Status status;
	Equ_Controller *controller;

	/* FIXME check from here */
	int flags; /** Layer flags */
	const int *formats; /** Supported pixel formats */
	int x;
	int y;
	int level;
	unsigned char hidden;
	Equ_Surface *surface;
};

static Equ_Region *_regions = NULL;
static Equ_Common_Id _ids = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Layer * equ_layer_new(Equanime *e, Equ_Controller *c, Equ_Common_Id id,
		const char *name)
{
	Equ_Layer *l;

	l = calloc(1, sizeof(Equ_Layer));
	l->controller = c;
	l->info.id = id;
	l->info.name = strdup(name);

	return l;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_layer_delete(Equ_Layer *l)
{
	free(l->info.name);
	free(l);
}
/**
 * Get the name of a layer
 * @param[in] e The Equanime connection
 * @param[in] l The layer to get the name from
 * @return The name of the layer
 */
EAPI const char * equ_layer_name_get(Equanime *e, Equ_Layer *l)
{
	return l->info.name;
}

/**
 * Get the capabilities of a layer
 * @param[in] e The Equanime connection
 * @param[in] l The Layer to get the capabilities from
 * @param[in,out] caps 
 */
EAPI void equ_layer_caps_get(Equanime *e, Equ_Layer *l, Equ_Layer_Caps *caps)
{
	Equ_Message_Layer_Caps_Get m;
	Equ_Reply_Layer_Caps_Get *r = NULL;
	Equ_Error error;

	if (!caps) return;

	/* send the command to the server */
	m.layer_id = l->info.id;
	error = equ_message_server_send(e, EQU_MSG_TYPE_LAYER_CAPS_GET, &m, 0, (void **)&r);
	if (error) return;
	*caps = r->caps;

	free(r);
}

/**
 * @param[in] e The Equanime connection
 * @param[in] l The Layer to get the status from
 * @param[in,out] status
 */
EAPI void equ_layer_status_get(Equanime *e, Equ_Layer *l, Equ_Layer_Status *status)
{
	Equ_Message_Layer_Caps_Get m;
	Equ_Reply_Layer_Status_Get *r = NULL;
	Equ_Error error;

	if (!status) return;

	/* send the command to the server */
	m.layer_id = l->info.id;
	error = equ_message_server_send(e, EQU_MSG_TYPE_LAYER_STATUS_GET, &m, 0, (void **)&r);
	if (error) return;
	*status = r->status;

	free(r);
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
	l->hidden = 1;
}
/**
 *
 */
EAPI void equ_layer_show(Equ_Layer *l)
{
	CHECK_FLAG(l, EQU_LAYER_VISIBILITY)
	if (!l->hidden) return;
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

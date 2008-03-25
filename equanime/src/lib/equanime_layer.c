#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"

#define CHECK_FLAG(l, f) if (!(l->desc->flags & f)) return;

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI void equanime_layer_description_get(Equanime_Layer *l, Equanime_Layer_Description *ld)
{
	
}
/**
 * 
 */
EAPI void equanime_layer_regions_get(Equanime_Layer *l, void *cb, void *cb_data)
{
	
}
/**
 * 
 * 
 */
EAPI void equanime_layer_size_set(Equanime_Layer *l, int w, int h)
{
	CHECK_FLAG(l, EQUANIME_LAYER_SIZE)
}

/**
 * 
 * 
 */
EAPI void equanime_layer_position_set(Equanime_Layer *l, int x, int y)
{
	CHECK_FLAG(l, EQUANIME_LAYER_POSITION)
	/* call the function */
	/* update the value */
	l->x = x;
	l->y = y;
}
/**
 * 
 * 
 */
EAPI void equanime_layer_geometry_get(Equanime_Layer *l, int *x, int *y, int *w, int *h)
{
	if (x) *x = l->x;
	if (y) *y = l->y;
	if (w) *w = l->w;
	if (h) *h = l->h;
}
/**
 * 
 */
EAPI void equanime_layer_format_set(Equanime_Layer *l, void *f)
{
	
	
}
/**
 *  
 */
EAPI void equanime_layer_level_get(Equanime_Layer *l, unsigned int *level)
{
	if (level) *level = l->level;
}
/**
 *  
 */
EAPI void equanime_layer_level_set(Equanime_Layer *l, unsigned int level)
{
	CHECK_FLAG(l, EQUANIME_LAYER_LEVEL)
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
EAPI void equanime_layer_level_up(Equanime_Layer *l)
{
	/* call level set with l->level + 1 */
}
/**
 * 
 */
EAPI void equanime_layer_level_down(Equanime_Layer *l)
{
	/* call level set with l->level - 1 */
}
/**
 * 
 */
EAPI void equanime_layer_hide(Equanime_Layer *l)
{
	CHECK_FLAG(l, EQUANIME_LAYER_VISIBILITY)
}
/**
 * 
 */
EAPI void equanime_layer_show(Equanime_Layer *l)
{
	CHECK_FLAG(l, EQUANIME_LAYER_VISIBILITY)
}
/**
 * 
 */
EAPI void equanime_layer_visibility_get(Equanime_Layer *l, unsigned char *hidden)
{
	if (hidden) *hidden = l->hidden;
}

/**
 * 
 */
EAPI Equanime_Controller * equanime_layer_controller_get(Equanime_Layer *l)
{
	return l->controller;
}

/**
 * 
 */
EAPI void * equanime_layer_ptr_get(Equanime_Layer *l)
{
	return l->ptr;
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 *
 */
void equanime_layer_register(Equanime_Layer_Description *ld, Equanime_Layer_Functions *lf)
{
	Equanime_Layer *l;
	
	l = malloc(sizeof(Equanime_Layer));
	equanime_controller_layer_register(ld->cname, l);
	l->desc = ld;
}
/**
 * 
 */
void equanime_layer_unregister(Equanime_Layer_Description *ld)
{
	Equanime_Layer *l;
	
	/* TODO remove the layer from the controller */
	l = equanime_controller_layer_unregister(ld->cname);
	free(l);
}

/**
 * 
 */
void equanime_layer_data_set(Equanime_Layer *l, void *data)
{
	l->data = data;
}

/**
 * 
 */
void * equanime_layer_data_get(Equanime_Layer *l)
{
	return l->data;
}

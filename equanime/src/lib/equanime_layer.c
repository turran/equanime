#include "Equanime.h"
#include "equanime_private.h"


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
	
}
/**
 * 
 * 
 */
EAPI void equanime_layer_position_set(Equanime_Layer *l, int x, int y)
{
	
}
/**
 * 
 * 
 */
EAPI void equanime_layer_geometry_get(Equanime_Layer *l, int *x, int *y, int *w, int *h)
{
	
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
	
}
/**
 *  
 */
EAPI void equanime_layer_level_set(Equanime_Layer *l, unsigned int level)
{
	
}
/**
 * 
 */
EAPI void equanime_layer_level_up(Equanime_Layer *l)
{
	
}
/**
 * 
 */
EAPI void equanime_layer_level_down(Equanime_Layer *l)
{
	
}
/**
 * 
 */
EAPI void equanime_layer_hide(Equanime_Layer *l)
{
	
}
/**
 * 
 */
EAPI void equanime_layer_show(Equanime_Layer *l)
{
	
}
/**
 * 
 */
EAPI void equanime_layer_visibility_get(Equanime_Layer *l, unsigned char *hidden)
{
	
}

/**
 * 
 */
EAPI Equanime_Controller * equanime_layer_controller_get(Equanime_Layer *l)
{
	return l->controller;
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 *
 */
void equanime_layer_register(Equanime_Layer_Description *ld)
{
	Equanime_Layer *l;
	
	//l = calloc(1, sizeof(Equanime_Layer));
	/* find the controller with the same name as the description cname */
	
	//l->controller = c;
	//l->desc = ld;
	/* add a new layer to the controller */
	//c->layers = realloc(c->layers, sizeof(Equanime_Layer) * c->desc->num_layers + 1);
	//c->layers[c->desc->num_layers] = l;
	//c->desc->num_layers++;
	
}
/**
 * 
 */
void equanime_layer_unregister(Equanime_Layer_Description *ld)
{
	
}

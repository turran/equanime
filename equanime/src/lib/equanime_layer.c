#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"
#include "equanime_private.h"
/**
 * A layer ...
 * 
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define CHECK_FLAG(l, f) if (!(l->desc->flags & f)) return;
Equanime_Region *_regions = NULL;
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI const Equanime_Layer_Description * equanime_layer_description_get(Equanime_Layer *l)
{
	return l->desc;
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
	if ((w == l->w) && (h == l->h))
		return;
	if (l->fncs->size_set(l, w, h))
	{
		l->w = w;
		l->h = h;
	}
}

/**
 * 
 * 
 */
EAPI void equanime_layer_position_set(Equanime_Layer *l, int x, int y)
{
	CHECK_FLAG(l, EQUANIME_LAYER_POSITION)
	if ((x == l->x) && (y == l->y))
		return;
	if (l->fncs->position_set(l, x, y))
	{
		l->x = x;
		l->y = y;
	}
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
EAPI void equanime_layer_format_set(Equanime_Layer *l, Enesim_Surface_Format fmt)
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
	if (l->hidden) return;
	if (l->fncs->visibility_set(l, 0))
			l->hidden = 1;
}
/**
 * 
 */
EAPI void equanime_layer_show(Equanime_Layer *l)
{
	CHECK_FLAG(l, EQUANIME_LAYER_VISIBILITY)
	if (!l->hidden) return;
	if (l->fncs->visibility_set(l, 1))
		l->hidden = 0;
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
 * Get the layer surface.
 * TODO What happens if a layer changes resolution, size, format ?
 * we should check that the surface is released?
 */
EAPI const Equanime_Surface * equanime_layer_surface_get(Equanime_Layer *l)
{
	Equanime_Surface *s;
	
	if (!l->surface)
	{
		Enesim_Surface *es;
		Enesim_Surface_Data sdata;
		
		s = equanime_surface_new();
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
#if 0
		case ENESIM_SURFACE_RGB888:
			sdata.rgb888.plane0 = l->fncs->ptr_get(l);
			sdata.rgb888.plane1 = malloc(sizeof(*sdata.rgb8888.plane1) * l->w * l->h);
			break;
		case ENESIM_SURFACE_A8:
			sdata.a8.plane0 = l->fncs->ptr_get(l);
			break;
#endif
		}
		es = enesim_surface_new(l->format, l->w, l->h, &sdata);
	}
	l->surface_ref++;
	
	return s;
}
/**
 * 
 */
EAPI void equanime_layer_surface_release(Equanime_Layer *l)
{
	if (l->surface_ref)
		l->surface_ref--;
}
	
/**
 *
 */
EAPI int equanime_layer_register(Equanime_Layer_Description *ld, Equanime_Layer_Functions *lf)
{
	Equanime_Layer *l;
	Equanime_Controller *c;
	
	c = equanime_controller_name_get_by(ld->cname);
	if (!c)
	{
		return 0;
	}
	
	l = malloc(sizeof(Equanime_Layer));
	l->desc = ld;
	l->fncs = lf;
	l->controller = c;
	/* call the probe function */
	printf("mmm\n");
	if (!(l->fncs->probe(l)))
	{
		free(l);
		return 0;
	}
	equanime_controller_layer_register(c, l);
	
	return 1;
}
/**
 * 
 */
EAPI void equanime_layer_unregister(Equanime_Layer_Description *ld)
{
	Equanime_Layer *l;
	
	/* TODO remove the layer from the controller */
	//l = equanime_controller_layer_unregister(ld->cname);
	//free(l);
}

/**
 * 
 */
EAPI void equanime_layer_data_set(Equanime_Layer *l, void *data)
{
	l->data = data;
}

/**
 * 
 */
EAPI void * equanime_layer_data_get(Equanime_Layer *l)
{
	return l->data;
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

#ifndef _EQU_LAYER_H
#define _EQU_LAYER_H

/**
 * @defgroup Equanime_Layer_Group Layers
 * @{
 */

typedef struct _Equ_Layer Equ_Layer; /**< Opaque handler */

EAPI void equ_layer_regions_get(Equ_Layer *l, void *cb, void *cb_data);

EAPI void equ_layer_size_get(Equ_Layer *l, int *w, int *h);

EAPI void equ_layer_position_set(Equ_Layer *l, int x, int y);
EAPI void equ_layer_position_get(Equ_Layer *l, int *x, int *y);

EAPI void equ_layer_level_get(Equ_Layer *l, unsigned int *level);
EAPI void equ_layer_level_set(Equ_Layer *l, unsigned int level);
EAPI void equ_layer_level_up(Equ_Layer *l);
EAPI void equ_layer_level_down(Equ_Layer *l);

EAPI void equ_layer_hide(Equ_Layer *l);
EAPI void equ_layer_show(Equ_Layer *l);
EAPI Eina_Bool equ_layer_is_visible(Equ_Layer *l);

EAPI Equ_Surface * equ_layer_surface_get(Equ_Layer *l);
EAPI void equ_layer_surface_set(Equ_Layer *l, Equ_Surface *s);

EAPI Equ_Controller * equ_layer_controller_get(Equ_Layer *l);


/** @} */

#endif

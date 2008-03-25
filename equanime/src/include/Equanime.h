#ifndef EQUANIME_H_
#define EQUANIME_H_

/**
 * 
 */

typedef struct _Equanime_Layer Equanime_Layer;
typedef struct _Equanime_Layer_Description Equanime_Layer_Description;

struct _Equanime_Layer_Description
{
	const char *cname; /** Controller name */
	const char *name; /** Layer name */
	int flags; /** Layer flags */ 
};

struct _Equanime_Controller_Description
{
	const char *name;
};

/**
 * TODO define possible layer options, like:
 */

enum
{
	EQUANIME_LAYER_VISIBILITY 	= (1 << 0),
	EQUANIME_LAYER_POSITION 	= (1 << 1),
	EQUANIME_LAYER_SIZE	 	= (1 << 2),
	EQUANIME_LAYER_LEVEL		= (1 << 3),
};



typedef struct _Equanime_Region Equanime_Region;

typedef struct _Equanime_Controller Equanime_Controller;
typedef struct _Equanime_Controller_Description Equanime_Controller_Description;


typedef void (*Equanime_Cb)(void *data, void *user_data);

EAPI void equanime_init(void);
EAPI void equanime_shutdown(void);
EAPI void equanime_controllers_get(Equanime_Cb cb, void *cb_data);
EAPI void equanime_controller_layers_get(Equanime_Controller *c, Equanime_Cb cb, void *cb_data);

EAPI Equanime_Controller * equanime_layer_controller_get(Equanime_Layer *l);

/* TODO pixel formats */

#endif /*EQUANIME_H_*/

#ifndef EQUANIME_H_
#define EQUANIME_H_

/**
 * 
 */

typedef struct _Equanime_Region Equanime_Region; /**< Opaque handler */
typedef struct _Equanime_Controller Equanime_Controller; /**< Opaque handler */
typedef struct _Equanime_Controller_Description Equanime_Controller_Description; /**< Opaque handler */
typedef struct _Equanime_Layer Equanime_Layer; /**< Opaque handler */
typedef struct _Equanime_Layer_Description Equanime_Layer_Description; /**< Opaque handler */

#define EQUANIME_LAYERS_MAX 10

/**
 * 
 */
struct _Equanime_Layer_Description
{
	const char *cname; /** Controller name */
	const char *name; /** Layer name */
	int flags; /** Layer flags */ 
	const int *formats; /** Supported pixel formats */
};

/**
 * 
 */
struct _Equanime_Controller_Description
{
	const char *name; /** Controller name */
	const char *lnames[EQUANIME_LAYERS_MAX]; /** Layer names */
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

typedef void (*Equanime_Cb)(void *data, void *user_data);

EAPI void equanime_init(void);
EAPI void equanime_shutdown(void);
EAPI void equanime_controllers_get(Equanime_Cb cb, void *cb_data);
EAPI void equanime_controller_layers_get(Equanime_Controller *c, Equanime_Cb cb, void *cb_data);

EAPI Equanime_Layer_Description * equanime_layer_description_get(Equanime_Layer *l);
EAPI Equanime_Controller * equanime_layer_controller_get(Equanime_Layer *l);

/* TODO pixel formats (FOURCC similar to what videodev2.h has */

#endif /*EQUANIME_H_*/

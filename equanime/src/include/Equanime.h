#ifndef EQUANIME_H_
#define EQUANIME_H_

/**
 * Equanime
 * 
 * 
 * TODO
 * - Support for outputs and inputs. The controller will have one output
 * in case it can only display the layer information on one crt/tv/panel/whatever
 * The inputs are useful for layers that can receive data from several sources,
 * like a video ccd block, etc.
 * - Define color-keying
 * - Define alpha blending between layers
 */

typedef struct _Equanime_Region Equanime_Region; /**< Opaque handler */
typedef struct _Equanime_Controller Equanime_Controller; /**< Opaque handler */
typedef struct _Equanime_Controller_Description Equanime_Controller_Description; /**< Opaque handler */
typedef struct _Equanime_Layer Equanime_Layer; /**< Opaque handler */
typedef struct _Equanime_Layer_Description Equanime_Layer_Description; /**< Opaque handler */

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

typedef int (*Equanime_Cb)(void *data, void *user_data);

EAPI void equanime_init(void);
EAPI void equanime_shutdown(void);
EAPI void equanime_controllers_get(Equanime_Cb cb, void *cb_data);
EAPI void equanime_controller_layers_get(Equanime_Controller *c, Equanime_Cb cb, void *cb_data);
EAPI Equanime_Controller_Description * equanime_controller_description_get(Equanime_Controller *c);

EAPI Equanime_Layer_Description * equanime_layer_description_get(Equanime_Layer *l);
EAPI Equanime_Controller * equanime_layer_controller_get(Equanime_Layer *l);

/* TODO pixel formats (FOURCC similar to what videodev2.h has */
#define FOURCC(a, b, c, d)          \
	(((unsigned int)(a) << 0) | \
	((unsigned int)(b) << 8)  | \
	((unsigned int)(c) << 16) | \
	((unsigned int)(d) << 24))

#endif /*EQUANIME_H_*/

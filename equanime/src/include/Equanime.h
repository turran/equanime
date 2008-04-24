#ifndef EQUANIME_H_
#define EQUANIME_H_

/**
 * @mainpage Equanime
 * @section intro Introduction
 * Equanime aims to be a simple hardware based graphics library for embedded 
 * devices.
 * It might be very similar to what DirectFB is, but without many policies implemented
 * inside. For example equanime won't have any concept of window manager or surface
 * allocator, not even input devices; just a graphics abstraction layer. This was
 * what I expected from DirectFB years ago when it first started, but then it
 * followed the same X11 errors: monolithic (which are now "solved" on Xorg).
 * 
 * Drivers:
 * - mp25xxf: MagicEyes (www.mesdigital.com) based SoC. Note that you *need* 
 * the code.google.com/p/gp2x-linux26 kernel :)
 * - dm320: Texas Instruments DM320 based SoC.
 * - dummy: UIO dummy driver.
 * 
 * @file
 * @brief Equanime API
 * @defgroup Equanime_Group API
 * @{
 * 
 * @todo
 * - Support for outputs and inputs. The controller will have one output
 * in case it can only display the layer information on one crt/tv/panel/whatever
 * The inputs are useful for layers that can receive data from several sources,
 * like a video ccd block, etc.
 * - Define color-keying
 * - Define alpha blending between layers
 * - Define the concept of regions, layers with no regions in hw means one region
 *   in sw with the size and properties of the layer but can't change anything?
 */

typedef struct _Equanime_Region Equanime_Region; /**< Opaque handler */
typedef struct _Equanime_Region_Description Equanime_Region_Description; /**< Opaque handler */
typedef struct _Equanime_Output Equanime_Output; /**< Opaque handler */
typedef struct _Equanime_Input Equanime_Input; /**< Opaque handler */

/**
 * TODO define possible layer options, like:
 */
typedef enum
{
	EQUANIME_LAYER_VISIBILITY 	= (1 << 0),
	EQUANIME_LAYER_POSITION 	= (1 << 1),
	EQUANIME_LAYER_SIZE	 	= (1 << 2),
	EQUANIME_LAYER_LEVEL		= (1 << 3),
	EQUANIME_LAYER_COLORKEY		= (1 << 4),
	EQUANIME_LAYER_BLEND		= (1 << 5),
} Equanime_Layer_Flags;
/**
 * 
 */
typedef enum
{
	EQUANIME_REGION_VISIBILITY 	= (1 << 0),
	EQUANIME_REGION_POSITION 	= (1 << 1),
	EQUANIME_REGION_SIZE	 	= (1 << 2),
	EQUANIME_REGION_LEVEL		= (1 << 3),
	EQUANIME_REGION_COLORKEY	= (1 << 4),
	EQUANIME_REGION_BLEND		= (1 << 5),
} Equanime_Region_Flags;
/**
 * 
 */
typedef enum
{
	EQUANIME_OUTPUT_LCD,
	EQUANIME_OUTPUT_TV,
	EQUANIME_OUTPUT_TYPES,
} Equanime_Output_Type;


/**
 * 
 */
struct _Equanime_Region_Description
{
	const char *cname; /** Controller name */
	const char *name; /** Layer name */
	int flags; /** Layer flags */ 
};
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

typedef int (*Equanime_Cb)(void *data, void *user_data); /**< */
/**
 * @defgroup Equanime_Core_Group Core
 * @{
 */
EAPI void equanime_init(void);
EAPI void equanime_shutdown(void);
/**
 * @} 
 * @defgroup Equanime_Controllers_Group Controllers
 * @{
 */

typedef struct _Equanime_Controller Equanime_Controller; /**< Opaque handler */
typedef struct _Equanime_Controller_Description Equanime_Controller_Description; /**< Opaque handler */

EAPI void equanime_controllers_get(Equanime_Cb cb, void *cb_data);
EAPI void equanime_controller_layers_get(Equanime_Controller *c, Equanime_Cb cb, void *cb_data);
EAPI const Equanime_Controller_Description * equanime_controller_description_get(Equanime_Controller *c);

/**
 * @}
 * @defgroup Equanime_Surface_Group Surfaces
 * @{
 */

/**
 * 
 */
typedef enum
{
	EQUANIME_SURFACE_ONSCREEN, /**< Visible graphics memory */
	EQUANIME_SURFACE_OFFSCREEN, /**< Non visible graphics memory */
	EQUANIME_SURFACE_VIRTUAL, /**< Virtual space */
} Equanime_Surface_Type;

typedef struct _Equanime_Surface Equanime_Surface; /**< Opaque handler */
EAPI const Enesim_Surface * equanime_surface_surface_get(const Equanime_Surface *);
EAPI Equanime_Surface_Type equanime_surface_type_get(const Equanime_Surface *);

/**
 * @}
 * @defgroup Equanime_Layer_Group Layers
 * @{
 */

typedef struct _Equanime_Layer Equanime_Layer; /**< Opaque handler */
typedef struct _Equanime_Layer_Description Equanime_Layer_Description; /**< Opaque handler */

EAPI const Equanime_Layer_Description * equanime_layer_description_get(Equanime_Layer *l);
EAPI void equanime_layer_regions_get(Equanime_Layer *l, void *cb, void *cb_data);
EAPI void equanime_layer_size_set(Equanime_Layer *l, int w, int h);
EAPI void equanime_layer_position_set(Equanime_Layer *l, int x, int y);
EAPI void equanime_layer_geometry_get(Equanime_Layer *l, int *x, int *y, int *w, int *h);
EAPI void equanime_layer_format_set(Equanime_Layer *l, Enesim_Surface_Format sfmt);
EAPI void equanime_layer_level_get(Equanime_Layer *l, unsigned int *level);
EAPI void equanime_layer_level_set(Equanime_Layer *l, unsigned int level);
EAPI void equanime_layer_level_up(Equanime_Layer *l);
EAPI void equanime_layer_level_down(Equanime_Layer *l);
EAPI void equanime_layer_hide(Equanime_Layer *l);
EAPI void equanime_layer_show(Equanime_Layer *l);
EAPI void equanime_layer_visibility_get(Equanime_Layer *l, unsigned char *hidden);
EAPI Equanime_Controller * equanime_layer_controller_get(Equanime_Layer *l);
EAPI const Equanime_Surface * equanime_layer_surface_get(Equanime_Layer *l);
/**
 * @}
 * @}
 */
#endif /*EQUANIME_H_*/

#include <stdlib.h>

#include "Eina.h"
#include "Equanime.h"
#include "Equanime_Module.h"

#include "dm320_regs.h"

/**
 * This driver implements the OSD hardware block of the Texas Instruments DM320.
 * The UIO driver should export four mmaped areas, one for each plane.
 * Restrictions: 
 * + Only one RGB layer can be on 16bpp depth at a time
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define CONTROLLER_NAME "Texas Instruments DM320"

#define DRIVER_NAME "dm320_osd"

enum
{
	DM320_VIDEO0,
	DM320_VIDEO1,
	DM320_RGB0,
	DM320_RGB1,
	DM320_LAYERS
};

typedef struct _Layer Layer;
typedef struct _Controller Controller;

/**
 * Layer definition
 */
struct _Layer
{
	void *addr;
	int id;
	Controller *c;
};

/**
 * Controller definition
 */
struct _Controller
{
	Equanime_Hal_Device *device;
	Layer *layers[DM320_LAYERS];
	void *regs;
};

/* old code */
#if 0
static void
dm320_fb_plane_enable(int plane, int enable)
{
	switch (plane)
	{
		case DM320_FB_PLANE_YUV0:
		if (enable)
			VIDWINMD |= VIDWINMD_ACT0;
		else
			VIDWINMD &= ~VIDWINMD_ACT0;
		break;
		
		case DM320_FB_PLANE_YUV1:
		if (enable)
			VIDWINMD |= VIDWINMD_ACT1;
		else
			VIDWINMD &= ~VIDWINMD_ACT1;
		break;
		
		case DM320_FB_PLANE_RGB0:
		if (enable)
			OSDWIN0MD |= OSDWINXMD_ACT;
		else
			OSDWIN0MD &= ~OSDWINXMD_ACT;
		break;

		case DM320_FB_PLANE_RGB1:
		if (enable)
			OSDWIN1MD |= OSDWINXMD_ACT;
		else
			OSDWIN1MD &= ~OSDWINXMD_ACT;
		break;
	}
}

/* FIXME 
 * is this logic necessary outside the check_var? */
static void 
dm320_fb_plane_bpp_check(int plane, int *bpp)
{
	/* round up */
	if (*bpp <= 1)
		*bpp = 1;
	else if (*bpp <= 2)
		*bpp = 2;
	else if (*bpp <= 4)
		*bpp = 4;
	else if (*bpp <= 8)
		*bpp = 8;
	else 
		*bpp = 16;
	/* video planes only support 4:2:2 (16 bpp) */
	if ((plane < DM320_FB_PLANE_RGB0) && (*bpp != 16))
		*bpp = 16;
	/* rgb planes, only one can be in 16 bpp */
	else if ((plane > DM320_FB_PLANE_YUV1) && (*bpp == 16))
	{
		/* if the first plane is 16 bpp set up the second 
		 * in 8 bpp */
		if (plane == DM320_FB_PLANE_RGB1)
		{

			if (planes[DM320_FB_PLANE_RGB0] && 
			planes[DM320_FB_PLANE_RGB0]->info->var.bits_per_pixel == 16)
				*bpp = 8;
		}

		/* if the second plane is 16 bpp set up the second
		 * with 8 bpp */
		if (plane == DM320_FB_PLANE_RGB0)
		{
			if (planes[DM320_FB_PLANE_RGB1] && 
			planes[DM320_FB_PLANE_RGB1]->info->var.bits_per_pixel == 16)
				*bpp = 8;
		}
	}
}

/* only one rgb plane in 16 bpp */
static void
dm320_fb_plane_bpp_set(int plane, int bpp)
{
	if (plane < DM320_FB_PLANE_RGB0) return;
	if (planes[plane]->bpp == bpp) return;
	/* disable old mode */
	if (planes[plane]->bpp == 16)
	{
		if (plane == DM320_FB_PLANE_RGB0)
			OSDWIN0MD &= ~OSDWINXMD_RGBE;
		else
			OSDWIN1MD &= ~OSDWINXMD_RGBE;
	}
	/* enable new mode */
	switch (plane)
	{
		case DM320_FB_PLANE_RGB0:
		switch (bpp)
		{
			case 1:
			OSDWIN0MD = (OSDWIN0MD & ~OSDWINXMD_BMW) | OSDWINXMD_BMW_1BIT;
			break;
			case 2:
			OSDWIN0MD = (OSDWIN0MD & ~OSDWINXMD_BMW) | OSDWINXMD_BMW_2BIT;
			break;
			case 4:
			OSDWIN0MD = (OSDWIN0MD & ~OSDWINXMD_BMW) | OSDWINXMD_BMW_4BIT;
			break;

			case 8:
			OSDWIN0MD = (OSDWIN0MD & ~OSDWINXMD_BMW) | OSDWINXMD_BMW_8BIT;
			break;

			case 16:
			OSDWIN0MD |= OSDWINXMD_RGBE;
			break;

		}
		break;

		case DM320_FB_PLANE_RGB1:
		switch (bpp)
		{
			case 1:
			OSDWIN1MD = (OSDWIN1MD & ~OSDWINXMD_BMW) | OSDWINXMD_BMW_1BIT;
			break;
			case 2:
			OSDWIN1MD = (OSDWIN1MD & ~OSDWINXMD_BMW) | OSDWINXMD_BMW_2BIT;
			break;
			case 4:
			OSDWIN1MD = (OSDWIN1MD & ~OSDWINXMD_BMW) | OSDWINXMD_BMW_4BIT;
			break;

			case 8:
			OSDWIN1MD = (OSDWIN1MD & ~OSDWINXMD_BMW) | OSDWINXMD_BMW_8BIT;
			break;

			case 16:
			OSDWIN1MD |= OSDWINXMD_RGBE;
			break;

		}
		break;
	}
	planes[plane]->bpp = bpp;
}


/* Zooming, 
 * if you have 2x zoom for a 640x480 display window size
 * the device reads a 320x240 from memory and displays it
 * with a factor of two (horizontal and/or vertical)
 */

static int
dm320_fb_plane_vzoom_set(int plane, unsigned int zoom)
{
	if (zoom > DM320_FB_ZOOM_4X)
		return -EINVAL;
	switch (plane)
	{
		case DM320_FB_PLANE_YUV0:
		break;
		
		case DM320_FB_PLANE_YUV1:
		break;
		
		case DM320_FB_PLANE_RGB0:
		OSDWIN0MD = (OSDWIN0MD & ~OSDWINXMD_OVZ) | OSDWINXMD_OVZv(zoom);
		break;

		case DM320_FB_PLANE_RGB1:
		OSDWIN1MD = (OSDWIN1MD & ~OSDWINXMD_OVZ) | OSDWINXMD_OVZv(zoom);
		break;
	}
	return 0;
}

static int
dm320_fb_plane_hzoom_set(int plane, unsigned int zoom)
{
	if (zoom > DM320_FB_ZOOM_4X)
		return -EINVAL;
	switch (plane)
	{
		case DM320_FB_PLANE_YUV0:
		break;
		
		case DM320_FB_PLANE_YUV1:
		break;
		
		case DM320_FB_PLANE_RGB0:
		OSDWIN0MD = (OSDWIN0MD & ~OSDWINXMD_OHZ) | OSDWINXMD_OHZv(zoom);
		break;

		case DM320_FB_PLANE_RGB1:
		OSDWIN1MD = (OSDWIN1MD & ~OSDWINXMD_OHZ) | OSDWINXMD_OHZv(zoom);
		break;
	}
	return 0;
}

/* 
 * frame mode
 * field mode
 */
static int
dm320_fb_plane_fmode_set(int plane, unsigned int mode)
{
	if (mode > DM320_FB_MODE_FRAME)
		return -EINVAL;

	switch (plane)
	{
		case DM320_FB_PLANE_YUV0:
		if (mode)
			VIDWINMD |= VIDWINMD_VFF0;
		else
			VIDWINMD &= ~VIDWINMD_VFF0;
		break;
		
		case DM320_FB_PLANE_YUV1:
		if (mode)
			VIDWINMD |= VIDWINMD_VFF1;
		else
			VIDWINMD &= ~VIDWINMD_VFF1;
		break;
		
		case DM320_FB_PLANE_RGB0:
		if (mode)
			OSDWIN0MD |= OSDWINXMD_OFF;
		else
			OSDWIN0MD &= ~OSDWINXMD_OFF;
		break;

		case DM320_FB_PLANE_RGB1:
		if (mode)
			OSDWIN1MD |= OSDWINXMD_OFF;
		else
			OSDWIN1MD &= ~OSDWINXMD_OFF;
		break;
	}
	return 0;
}

/* set the rgb 1 plane in to attribute mode */
static void
dm320_fb_plane_attr_set(int plane, int on)
{
	if (plane != DM320_FB_PLANE_RGB1)
		return;
	if (on > 1) on = 1;
	else if (on < 0) on = 0;

	OSDATTRMD = ((OSDATTRMD & ~OSDATTRMD_OASW) | OSDATTRMD_OASWv(on));
}

/* 
 * offset
 */
static void
dm320_fb_plane_offset_set(int plane, unsigned int offset)
{
	u32 addr_32 = offset/32;
	switch (plane)
	{
		case DM320_FB_PLANE_YUV0:
		VIDWIN0OFST = offset;
		break;
		
		case DM320_FB_PLANE_YUV1:
		VIDWIN1OFST = offset;
		break;
		
		case DM320_FB_PLANE_RGB0:
		OSDWIN0OFST = offset;
		break;

		case DM320_FB_PLANE_RGB1:
		OSDWIN1OFST = offset;
		break;
	}
}

/* 
 * x = pixels
 * y = lines (pixels * 2)
 * position relative to the base x and base y
 * coordinates 
 */
static int
dm320_fb_plane_position_set(int plane, unsigned int x, unsigned int y)
{
	if (x >= DM320_FB_MAX_X || y >= DM320_FB_MAX_Y)
		return -EINVAL;
	switch (plane)
	{
		case DM320_FB_PLANE_YUV0:
		VIDWIN0XP = x;
		VIDWIN0YP = y;
		break;
		
		case DM320_FB_PLANE_YUV1:
		VIDWIN1XP = x;
		VIDWIN1YP = y;
		break;
		
		case DM320_FB_PLANE_RGB0:
		OSDWIN0XP = x;
		OSDWIN0YP = y;
		break;

		case DM320_FB_PLANE_RGB1:
		OSDWIN1XP = x;
		OSDWIN1YP = y;
		break;
	}
	return 0;
}


static int
dm320_fb_plane_size_set(int plane, int width, int height)
{
	if (width > DM320_FB_MAX_WIDTH || height > DM320_FB_MAX_HEIGHT) 
		return 0;
	switch (plane)
	{
		case DM320_FB_PLANE_YUV0:
		VIDWIN0XL = width;
		VIDWIN0YL = height;
		break;
		
		case DM320_FB_PLANE_YUV1:
		VIDWIN1XL = width;
		VIDWIN1YL = height;
		break;
		
		case DM320_FB_PLANE_RGB0:
		OSDWIN0XL = width;
		OSDWIN0YL = height;
		break;

		case DM320_FB_PLANE_RGB1:
		OSDWIN1XL = width;
		OSDWIN1YL = height;
		break;
	}
	return 1;
}
#endif

/*============================================================================*
 *                               Controller                                   * 
 *============================================================================*/
static int controller_probe(Equanime_Controller *ec)
{
	Controller *c;
		
	c = malloc(sizeof(Controller));
	/* check if the driver exists */
	c->device = equanime_hal_uio_open(DRIVER_NAME);
	if (!c->device)
	{
		free(c);
		return 0;
	}
	equanime_controller_data_set(ec, c);
	
	return 1;
}

static void controller_remove(Equanime_Controller *ec)
{
	Controller *c;
			
	/* unregister the controller */
	c = equanime_controller_data_get(ec);
	/* close the device */
	equanime_hal_uio_close(c->device);
	free(c);
}

static Equanime_Controller_Description dm320_description = 
{
	.name = CONTROLLER_NAME,
};

static Equanime_Controller_Functions dm320_functions =
{
	.probe = &controller_probe,
	.remove = &controller_remove,		
};
/*============================================================================*
 *                                  Layer                                     * 
 *============================================================================*/
static int layer_probe(Equanime_Layer *el)
{
	Layer *l;
	Controller *c;
	Equanime_Layer_Description *eld;
	
	c = equanime_controller_data_get(equanime_layer_controller_get(el));
	
	l = malloc(sizeof(Layer));
	/* check the name and match it to the id */
	eld = equanime_layer_description_get(el);
	if (!strcmp(eld->name, "VIDEO0"))
		l->id = DM320_VIDEO0;
	else if (!strcmp(eld->name, "VIDEO1"))
		l->id = DM320_VIDEO1;
	else if (!strcmp(eld->name, "RGB0"))
		l->id = DM320_RGB0;
	if (!strcmp(eld->name, "RGB1"))
		l->id = DM320_RGB1;
	else
	{
		/* WTF? */
		free(l);
		return 0;
	}
	equanime_layer_data_set(el, l);

	return 1;
}

static void layer_remove(Equanime_Layer *el)
{
	Layer *l;
	
	l = equanime_layer_data_get(el);
	l->c->layers[l->id] = NULL;
	free(l);
}

/* 
 * width pixels (max 4096 = 2^12)
 * height lines (max 2048 = 2^11)
 */
static void layer_size_set(Equanime_Layer *el)
{
	Layer *l;
		
	l = equanime_layer_data_get(el);
	switch (l->id)
	{
		case DM320_VIDEO0:
		//VIDWIN0XL = width;
		//VIDWIN0YL = height;
		break;
		
		case DM320_VIDEO1:
		//VIDWIN1XL = width;
		//VIDWIN1YL = height;
		break;
		
		case DM320_RGB0:
		//OSDWIN0XL = width;
		//OSDWIN0YL = height;
		break;

		case DM320_RGB1:
		//OSDWIN1XL = width;
		//OSDWIN1YL = height;
		break;
	}
}

static void layer_position_set(Equanime_Layer *el)
{
	Layer *l;
	
	l = equanime_layer_data_get(el);
	switch (l->id)
	{
		case DM320_VIDEO0:
		//VIDWIN0XP = x;
		//VIDWIN0YP = y;
		break;
			
		case DM320_VIDEO1:
		//VIDWIN1XP = x;
		//VIDWIN1YP = y;
		break;
			
		case DM320_RGB0:
		//OSDWIN0XP = x;
		//OSDWIN0YP = y;
		break;

		case DM320_RGB1:
		//OSDWIN1XP = x;
		//OSDWIN1YP = y;
		break;
	}
}

static void layer_format_set(Equanime_Layer *l, int f)
{
	
}

static Equanime_Layer_Description dm320_video0_description = 
{
	.cname = CONTROLLER_NAME,
	.name = "VIDEO0",
	.flags = EQUANIME_LAYER_VISIBILITY |
		EQUANIME_LAYER_POSITION | 
		EQUANIME_LAYER_SIZE |
		EQUANIME_LAYER_LEVEL,
};

static Equanime_Layer_Description dm320_video1_description = 
{
	.cname = CONTROLLER_NAME,
	.name = "VIDEO1",
	.flags = EQUANIME_LAYER_VISIBILITY |
		EQUANIME_LAYER_POSITION | 
		EQUANIME_LAYER_SIZE |
		EQUANIME_LAYER_LEVEL,
};

static Equanime_Layer_Description dm320_rgb0_description = 
{
	.cname = CONTROLLER_NAME,
	.name = "RGB0",
	.flags = EQUANIME_LAYER_VISIBILITY |
		EQUANIME_LAYER_POSITION | 
		EQUANIME_LAYER_SIZE |
		EQUANIME_LAYER_LEVEL,
};

static Equanime_Layer_Description dm320_rgb1_description = 
{
	.cname = CONTROLLER_NAME,
	.name = "RGB1",
	.flags = EQUANIME_LAYER_VISIBILITY |
		EQUANIME_LAYER_POSITION | 
		EQUANIME_LAYER_SIZE |
		EQUANIME_LAYER_LEVEL,
};

static Equanime_Layer_Functions dm320_layer_functions =
{
	.probe = &layer_probe,
	.remove = &layer_remove,		
};

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
int module_init(void)
{
	/* register the controller */
	if (!equanime_controller_register(&dm320_description, &dm320_functions))
		goto err_controller;
	/* register the layers */
	if (!equanime_layer_register(&dm320_video0_description, &dm320_layer_functions))
		goto err_layer0;
	if (!equanime_layer_register(&dm320_video1_description, &dm320_layer_functions))
		goto err_layer1;
	if (!equanime_layer_register(&dm320_rgb0_description, &dm320_layer_functions))
		goto err_layer2;
	if (!equanime_layer_register(&dm320_rgb1_description, &dm320_layer_functions))
		goto err_layer3;
	return 1;
	
err_layer3:
	equanime_layer_unregister(&dm320_rgb0_description);
err_layer2:
	equanime_layer_unregister(&dm320_video1_description);
err_layer1:
	equanime_layer_unregister(&dm320_video0_description);
err_layer0:
	equanime_controller_unregister(&dm320_description);
err_controller:
	return 0;
}

void module_exit(void)
{
	equanime_layer_unregister(&dm320_video0_description);
	equanime_layer_unregister(&dm320_video1_description);
	equanime_layer_unregister(&dm320_rgb0_description);
	equanime_layer_unregister(&dm320_rgb1_description);
	equanime_controller_unregister(&dm320_description);
}

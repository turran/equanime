#include "Equanime.h"
#include "equanime_private.h"

#include "dm6446_regs.h"

/* 
 * Constraints:
 * Both the OSD windows and VIDWIN1 should be fully contained inside VIDWIN0. This means that both
 * the Y and X position of either the OSD windows or VIDWIN1 should be greater (not equal) to the Y
 * and X position of VIDWIN0.
 * If the vertical resize filter is enabled for either of the video windows, the maximum horizontal window
 * dimension cannot be greater than 720 currently. This is due to the limitation in the size of the line
 * memory.
 * It is not possible to use both of the CLUT ROMs at the same time. However, a window can use RAM
 * while another chooses ROM.
 * The 24-bit RGB input mode is only valid for one of the two video windows (programmable) and does
 * not apply to the OSD windows.
 * Video window 0: Use video window 0 for HD display.
 * Video window 1: Do not use video window 1 (turn off video window 1 when doing HD display).
 * OSD windows 0 and 1: You may use the OSD windows but their combined total bandwidth must be
 * less than 25 Mbytes/second.
 * The bandwidth for the OSD windows depends on the size of the windows and also the bytes per pixel
 * associated with the input data format. The equation is:
 * [(x0 × y0) × b0 + (x1 × y1) × b1] × 60 < 25 Mbytes/second
 * Where:
 * x0 = horizontal size of OSD window 0 in units of pixels
 * y0 = vertical size of OSD window 0 in units of pixels
 * b0 = bytes per pixel (depends on data input format selected for the OSD window)
 * x1 = horizontal size of OSD window 1 in units of pixels
 * y1 = vertical size of OSD window 1 in units of pixels
 * b1 = bytes per pixel (depends on data input format selected for the OSD window)
 * 60 = frame rate for HD video
 */

typedef struct _Layer Layer;

typedef enum _Layer_Id
{
	LAYER_OSD0,
	LAYER_OSD1,
	LAYER_VID0,
	LAYER_VID1,
	LAYERS,
} Layer_Id;

struct _Layer
{
	Layer_Id id;
	Layer *all;
	Equ_Layer *layer;
	struct dm6446 *dm6446;
};

static Eina_Bool _all_size_check(Layer *l, unsigned int w, unsigned int h)
{

}

static int _osd_bandwith_get(unsigned int w, unsigned int h, int bpp)
{

}

static Eina_Bool _osd_bandwidth(Layer *l, unsigned int w, unsigned int h, Equ_Format fmt)
{
	Equ_Layer *other;
	unsigned int cw, ch, ow, oh;
	Eina_Bool cen, oen;
	int obw = 0;
	Equ_Surface *s;

	if (l->id == LAYER_OSD0)
	{
		Layer *tmp = l->all + LAYER_OSD1;
		other = tmp->layer;
	}
	else
	{
		Layer *tmp = l->all + LAYER_OSD0;
		other = tmp->layer;
	}

	if (equ_layer_is_visible(other))
	{
		Equ_Format fmt;

		equ_layer_geometry_get(other, NULL, NULL, &ow, &oh);
		/* TODO get bpp from format */
		//fmt = equ_layer_format_get(other);
		obw += _osd_bandwith_get(ow, oh, 16);
	}
current:
	/* TODO get bpp from format */
	obw += _osd_bandwith_get(ow, oh, 16);
}

static Eina_Bool _vid_format_check(Layer *l, Equ_Format fmt)
{
	if ((fmt != EQU_FORMAT_RGB888) ||
			(fmt != EQU_FORMAT_YUV422))
		return EINA_FALSE;

	return EINA_TRUE;
}

static Eina_Bool _osd0_size_set(Equ_Layer *l, unsigned int w, unsigned int h)
{

}

static Eina_Bool _osd0_position_set(Equ_Layer *l, int x, int y)
{

}

static Eina_Bool _osd0_visibility_set(Equ_Layer *l, Eina_Bool show)
{
	if (show)
		l->osd->osdwin0md |= show;
	else
		l->osd->osdwin0md &= ~show;
}

static Eina_Bool _osd0_surface_set(Equ_Layer *l, Equ_Surface *s)
{

}

static Equ_Layer_Backend _layer_osd0 = {
	.size_set = _osd0_size_set,
	.position_set = _osd0_position_set,
	.visibility_set = _osd0_visibility_set,
	.surface_set = _osd0_surface_set,
};

static Equ_Layer_Backend _layer_osd1 = {

};

static Equ_Layer_Backend _layer_vid0 = {

};

static Equ_Layer_Backend _layer_vid1 = {

};

/* TODO receive the registers and the interrupts? */
Eina_Bool dm6446_controller_init(Equ_Controller *c, struct dm6446 *dm6446)
{
	Layer *l, *all;

	/* register the layers */
	/* OSD0 */
	l = all = malloc(sizeof(Layer) * LAYERS);
	l->id = LAYER_OSD0;
	l->dm6446 = dm6446;
	l->all = all;
	l->layer = equ_controller_layer_register(c, &_layer_osd0, l);
	/* OSD1 */
	l++;
	l->id = LAYER_OSD1;
	l->dm6446 = dm6446;
	l->all = all;
	l->layer = equ_controller_layer_register(c, &_layer_osd1, l);
	/* VID0 */
	l++;
	l->id = LAYER_VID0;
	l->dm6446 = dm6446;
	l->all = all;
	l->layer = equ_controller_layer_register(c, &_layer_vid0, l);
	/* VID1 */
	l++;
	l->id = LAYER_VID0;
	l->dm6446 = dm6446;
	l->all = all;
	l->layer = equ_controller_layer_register(c, &_layer_vid1, l);
}

void dm6446_controller_shutdown(Equ_Controller *c)
{
	/* TODO get the first layer and free the array of data */
}


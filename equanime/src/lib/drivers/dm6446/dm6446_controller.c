#include <stddef.h>

#include "Equanime.h"
#include "equanime_private.h"

#include "dm6446_regs.h"

/*
 * Constraints:
 * If the vertical resize filter is enabled for either of the video windows, the maximum horizontal window
 * dimension cannot be greater than 720 currently. This is due to the limitation in the size of the line
 * memory.
 * It is not possible to use both of the CLUT ROMs at the same time. However, a window can use RAM
 * while another chooses ROM.
 * Video window 0: Use video window 0 for HD display.
 * Video window 1: Do not use video window 1 (turn off video window 1 when doing HD display).
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
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

/*
 * Both the OSD windows and VIDWIN1 should be fully contained inside VIDWIN0. This means that both
 * the Y and X position of either the OSD windows or VIDWIN1 should be greater (not equal) to the Y
 * and X position of VIDWIN0.
 */
static Eina_Bool _layer_size_check(Layer *l, int x, int y,
		unsigned int w, unsigned int h)
{

}

/* get the bandwidth, bpp is on bytes not, bits */
static inline int _osd_bandwith_get(unsigned int w, unsigned int h, int bpp)
{
	return w * h * bpp;
}

/*
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
static Eina_Bool _osd_bandwidth(Layer *l, unsigned int w, unsigned int h,
		Equ_Format fmt)
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

		equ_layer_size_get(other, &ow, &oh);
		/* TODO get bpp from format */
		//fmt = equ_layer_format_get(other);
		obw += _osd_bandwith_get(ow, oh, 2);
	}
current:
	/* TODO get bpp from format */
	obw += _osd_bandwith_get(ow, oh, 2);
}

static Eina_Bool _position_check(Layer *l, Equ_Layer *el, int x, int y)
{
	unsigned int w, h;

	equ_layer_size_get(el, &w, &h);
	if (!_layer_size_check(l, x, y, w, h))
		return EINA_FALSE;

	return EINA_TRUE;
}

/* Check that we can enable the layer */
static Eina_Bool _visibility_check(Layer *l, Equ_Layer *el, Eina_Bool show)
{
	unsigned int w, h;
	int x, y;

	equ_layer_size_get(el, &w, &h);
	equ_layer_position_get(el, &x, &y);
	if (!_layer_size_check(l, x, y, w, h))
		return EINA_FALSE;
	return EINA_TRUE;
}

/* Check that we set the surface format and size */
static Eina_Bool _osd_surface_check(Layer *l, Equ_Layer *el, Equ_Surface *s,
		unsigned int *w, unsigned int *h)
{
	Equ_Format fmt;
	int x, y;

	fmt = equ_surface_format_get(s);
	equ_surface_size_get(s, w, h);
	if (!_osd_bandwidth(l, *w, *h, fmt))
		return EINA_FALSE;

	equ_layer_position_get(el, &x, &y);

	if (!_layer_size_check(l, x, y, *w, *h))
		return EINA_FALSE;

	return EINA_TRUE;
}

static Eina_Bool _vid_format_check(Layer *l, Equ_Format fmt)
{
	/*
	 * The 24-bit RGB input mode is only valid for one of the two video windows (programmable) and does
	 * not apply to the OSD windows.
	 */
	if (fmt == EQU_FORMAT_RGB888)
	{
		Equ_Layer *other;
		Equ_Surface *s;


		if (l->id == LAYER_VID0)
		{
			Layer *tmp = l->all + LAYER_VID1;
			other = tmp->layer;
		}
		else
		{
			Layer *tmp = l->all + LAYER_VID0;
			other = tmp->layer;
		}
		if (!equ_layer_is_visible(other))
			return EINA_TRUE;

		s = equ_layer_surface_get(other);
		if (equ_surface_format_get(s) == EQU_FORMAT_RGB888)
			return EINA_FALSE;
	}
	else if (fmt == EQU_FORMAT_YUV422)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}


static Eina_Bool _osd0_position_set(Equ_Layer *el, int x, int y)
{
	Layer *l = equ_layer_data_get(el);
	struct dm6446 *dm6446 = l->dm6446;

	/* common checks */
	if (!_position_check(l, el, x, y))
		return EINA_FALSE;
	/* set the position */
	dm6446->osd->osdwin0xp = x;
	dm6446->osd->osdwin0yp = y;
}

static Eina_Bool _osd0_visibility_set(Equ_Layer *el, Eina_Bool show)
{
	Layer *l = equ_layer_data_get(el);
	struct dm6446 *dm6446 = l->dm6446;

	if (!_visibility_check(l, el, show))
		return EINA_FALSE;
	/* enable/disable it */
	if (show)
		dm6446->osd->osdwin0md |= show;
	else
		dm6446->osd->osdwin0md &= ~show;

	return EINA_TRUE;
}

static Eina_Bool _osd0_surface_set(Equ_Layer *el, Equ_Surface *s)
{
	Layer *l = equ_layer_data_get(el);
	Equ_Format fmt;
	struct dm6446 *dm6446 = l->dm6446;
	unsigned int w, h;

	/* common checks */
	fmt = equ_surface_format_get(s);
	if (fmt != EQU_FORMAT_RGB565)
		return EINA_FALSE;

	if (!_osd_surface_check(l, el, s, &w, &h))
		return EINA_FALSE;

	/* actually setup the surface */
	dm6446->osd->osdwin0xl = w;
	dm6446->osd->osdwin0yl = h;

	return EINA_TRUE;
}

static Equ_Layer_Backend _layer_osd0 = {
	.position_set = _osd0_position_set,
	.visibility_set = _osd0_visibility_set,
	.surface_set = _osd0_surface_set,
};


static Eina_Bool _osd1_visibility_set(Equ_Layer *el, Eina_Bool show)
{
	Layer *l = equ_layer_data_get(el);
	struct dm6446 *dm6446 = l->dm6446;

	if (!_visibility_check(l, el, show))
		return EINA_FALSE;
	/* enable/disable it */
	if (show)
		dm6446->osd->osdwin1md |= show;
	else
		dm6446->osd->osdwin1md &= ~show;

	return EINA_TRUE;
}

static Eina_Bool _osd1_position_set(Equ_Layer *el, int x, int y)
{
	Layer *l = equ_layer_data_get(el);
	struct dm6446 *dm6446 = l->dm6446;

	/* common checks */
	if (!_position_check(l, el, x, y))
		return EINA_FALSE;
	/* set the position */
	dm6446->osd->osdwin1xp = x;
	dm6446->osd->osdwin1yp = y;
}

static Eina_Bool _osd1_surface_set(Equ_Layer *el, Equ_Surface *s)
{
	Layer *l = equ_layer_data_get(el);
	Equ_Format fmt;
	struct dm6446 *dm6446 = l->dm6446;
	unsigned int w, h;

	/* common checks */
	fmt = equ_surface_format_get(s);
	if ((fmt != EQU_FORMAT_RGB565) ||
			(fmt != EQU_FORMAT_Ab31))
		return EINA_FALSE;

	if (!_osd_surface_check(l, el, s, &w, &h))
		return EINA_FALSE;

	/* actually setup the surface */
	dm6446->osd->osdwin1xl = w;
	dm6446->osd->osdwin1yl = h;

	return EINA_TRUE;
}

static Equ_Layer_Backend _layer_osd1 = {
	.position_set = _osd1_position_set,
	.surface_set = _osd1_surface_set,
	.visibility_set = _osd1_visibility_set,
};

static Equ_Layer_Backend _layer_vid0 = {

};

static Equ_Layer_Backend _layer_vid1 = {

};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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
	l->layer = equ_controller_layer_register(c, &_layer_osd0, "osd0", l);
	/* OSD1 */
	l++;
	l->id = LAYER_OSD1;
	l->dm6446 = dm6446;
	l->all = all;
	l->layer = equ_controller_layer_register(c, &_layer_osd1, "osd1", l);
	/* VID0 */
	l++;
	l->id = LAYER_VID0;
	l->dm6446 = dm6446;
	l->all = all;
	l->layer = equ_controller_layer_register(c, &_layer_vid0, "vid0", l);
	/* VID1 */
	l++;
	l->id = LAYER_VID0;
	l->dm6446 = dm6446;
	l->all = all;
	l->layer = equ_controller_layer_register(c, &_layer_vid1, "vid1", l);

	printf("%08x %08x\n", dm6446->venc->dacsel, dm6446->venc->dactst);

	/* disable VCLK output pin enable */
#if 0
	dm6446->osd->mode = 0x200;
	dm6446->venc->osdclk0 = 1;
	dm6446->venc->osdclk1 = 2;
	dm6446->venc->vidctl = 0x1101;
	dm6446->venc->syncctl = 0;
	dm6446->venc->dclkctl = 0;
	dm6446->venc->drgbx1 = 0x57c;
	dm6446->venc->lcdout = 0;
	dm6446->venc->cmpnt = 0x100;
#endif
	/* set color bars mode */
	dm6446->venc->vdpro = (1 << 8);
	/* enable the clocks */
	dm6446->vpss->clkctl = 0x18;
}

void dm6446_controller_shutdown(Equ_Controller *c)
{
	/* TODO get the first layer and free the array of data */
}

void dm6446_venc_mode_set(struct dm6446 *dm6446, Equ_Mode *m,
		Eina_Bool internal)
{
#if 0
	/* set the window field / frame mode */
	(VENC_YCCCTL, 0x0);

	(VENC_VSTARTA, extmode->vstarta);
	(OSD_BASEPX, extmode->basex);
	(OSD_BASEPY, extmode->basey);
#endif
	/* Set the internal DAC (only NTSC, PAL, 525P, 625) */
	if (internal)
	{
		printf("YCCTL = %d\n", offsetof(struct dm6446_venc_regs, ycctl));
		printf("VMOD = %d\n", offsetof(struct dm6446_venc_regs, vmod));
		dm6446->venc->vmod |= 1 << 12;
		dm6446->venc->ycctl |= 1;
		printf("internal0 = %08x\n", dm6446->venc->vmod);
		if (m->std == EQU_STANDARD_NTSC)
		{
			dm6446->venc->vmod |= 1 << 1;
			/* NTSC/PAL Timing */
			dm6446->venc->vmod &= ~(1 << 4);
			/* master mode */
			dm6446->venc->vmod &= ~(1 << 5);
			/* NTSC */
			dm6446->venc->vmod &= ~(1 << 6);
			/* SDTV */
			dm6446->venc->vmod &= ~(1 << 8);
			/* standard interlace */
			dm6446->venc->vmod &= ~(1 << 10);
		}
		else if (m->std == EQU_STANDARD_PAL)
		{
			dm6446->venc->vmod |= 1 << 1;
			/* NTSC/PAL Timing */
			dm6446->venc->vmod &= ~(1 << 4);
			/* master mode */
			dm6446->venc->vmod &= ~(1 << 5);
			/* NTSC */
			dm6446->venc->vmod |= (1 << 6);
			/* SDTV */
			dm6446->venc->vmod &= ~(1 << 8);
			/* standard interlace */
			dm6446->venc->vmod &= ~(1 << 10);
		}
		printf("internal1 = %08x\n", dm6446->venc->vmod);
#if 0
		/* Set REC656 Mode */
		(VENC_YCCCTL, 0x1);
		/* Enable output mode and NTSC */
		(VENC_VMOD, 0x1003);
#endif
	}
	else
	{
		dm6446->venc->hspls = m->t.hsync_len;
		dm6446->venc->vspls = m->t.vsync_len;
		dm6446->venc->hint = m->xres + m->t.left_margin + m->t.right_margin;
		dm6446->venc->hstart = m->t.left_margin;
		dm6446->venc->hvalid = m->xres;
		dm6446->venc->vint = m->yres + m->t.upper_margin + m->t.lower_margin;
		dm6446->venc->vstart = m->t.upper_margin;
		dm6446->venc->vvalid = m->vmode == EQU_VIDEO_MODE_INTERLACED ? m->yres / 2 : m->yres;
	}
}

void dm6446_venc_dac_set(struct dm6446 *dm6446, dm6446_dout dac0,
		dm6446_dout dac1, dm6446_dout dac2, dm6446_dout dac3)
{
	printf("Setting dacs to %d %d %d %d\n", dac0, dac1, dac2, dac3);
	dm6446->venc->dacsel = dac0 | dac1 << 4 | dac2 << 8 | dac3 << 12;
}

void dm6446_venc_dac_enable(struct dm6446 *dm6446, Eina_Bool dac0,
		Eina_Bool dac1, Eina_Bool dac2, Eina_Bool dac3)
{
	dm6446->venc->dactst = !dac0 << 12 | !dac1 << 13 | !dac2 << 14 | !dac3 << 15;
}

void dm6446_venc_enable(struct dm6446 *dm6446, Eina_Bool enable)
{
	if (enable)
		dm6446->venc->vmod |= 1;
	else
		dm6446->venc->vmod &= ~1;

}

void dm6446_venc_vout_set(struct dm6446 *dm6446, dm6446_vout vout)
{
	uint32_t vmod;
	uint32_t ycctl;

	vmod = dm6446->venc->vmod & ~(0xf << 12);
	ycctl = dm6446->venc->ycctl & ~(0x3 << 2);
	switch (vout)
	{
		case DM6446_VOUT_16BIT_YCBCR:
		case DM6446_VOUT_16BIT_YCRCB:
		ycctl |= vout << 2;
		break;

		case DM6446_VOUT_8BIT_CBYCRY:
		case DM6446_VOUT_8BIT_YCRYCB:
		case DM6446_VOUT_8BIT_CRYCBY:
		case DM6446_VOUT_8BIT_YCBYCR:
		vmod |= 1 << 12;
		ycctl |= (vout - DM6446_VOUT_8BIT_CBYCRY) << 2;
		break;

		case DM6446_VOUT_PARALLEL:
		vmod |= 2 << 12;
		break;

		default:
		break;
	}
	dm6446->venc->vmod = vmod;
	dm6446->venc->ycctl = ycctl;
}

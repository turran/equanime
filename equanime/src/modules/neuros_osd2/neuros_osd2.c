#include "Equanime.h"
#include "equanime_private.h"
#include "dm6446_regs.h"

/**
 * This module adds support for the neuros osd2 device
 * Outputs:
 * composite (internal)
 * component (ths8200)
 * hdmi (sil9034)
 * Inputs:
 * composite
 * component
 *
 * Here we should create a controller, set the functions
 * and call the generic dm6446 when needed and the different
 * attached components when needed
 * Create the host ourselves too
 */
struct nt_osd2
{
	Equ_Controller *c;
	struct dm6446 dm6446;
};

static struct nt_osd2 osd2;

static Eina_Bool _output_set(Equ_Controller *c, Equ_Output *o)
{
	/* TODO in case of composite output, setup the internal DAC too */
}

static Equ_Controller_Backend _controller =
{
	.output_set = _output_set,
};

static Eina_Bool _composite_timing_set(Equ_Output *o, Equ_Timing *t)
{
	/* only sdtv (NTSC, PAL) */
	if ((t->std != EQU_STANDARD_PAL) ||
			(t->std != EQU_STANDARD_NTSC))
		return EINA_FALSE;
	/* set register values for the ths7313 */
	return EINA_TRUE;
}

static Equ_Output_Backend _composite_output =
{
	.timing_set = _composite_timing_set,
};

static Eina_Bool _component_timing_set(Equ_Output *o, Equ_Timing *t)
{
	/* only hdtv (480p, 720p, 1080i) */
	if ((t->std != EQU_STANDARD_480P) ||
			(t->std != EQU_STANDARD_720P) ||
			(t->std != EQU_STANDARD_1080I))
		return EINA_FALSE;
	/* set the register values for the ths8200 */
	return EINA_TRUE;
}


static Equ_Output_Backend _component_output =
{
	.timing_set = _component_timing_set,
};

/* initialize the dm6446 controller, layers, regions, etc */
static Eina_Bool _dm6446_setup(Equ_Controller *c, Equanime_Hal_Device *dev, struct dm6446 *dm6446)
{
	dm6446->osd = equanime_hal_uio_map(dev, 1);
	if (!dm6446->osd)
		goto osd_err;

	dm6446->venc = equanime_hal_uio_map(dev, 2);
	if (!dm6446->venc)
		goto venc_err;

	if (!dm6446_controller_init(c, dm6446))
		goto dm6446_err;

	return EINA_TRUE;

dm6446_err:
	equanime_hal_uio_unmap(dev, 2);
venc_err:
	equanime_hal_uio_unmap(dev, 1);
osd_err:
	return EINA_FALSE;
}

/* register the outputs, inputs, etc */
static Eina_Bool _io_setup(Equ_Controller *c)
{
	Equ_Hal_I2C *ths8200, *ths7313;

	/* get the companion devices */
	/* ths8200 (0x20) */
	ths8200 = equ_hal_i2c_get(0x20);
	/* ths7313 (0x2c) */
	ths7313 = equ_hal_i2c_get(0x2c);

	equ_controller_output_register(c, &_component_output, ths8200);
	equ_controller_output_register(c, &_composite_output, ths7313);
}

static Eina_Bool module_init(void)
{
	Equanime_Hal_Device *hd;
	void *mem;

	printf("Initializing Neuros OSD2 Board\n");
	/* get the UIO devices */
	hd = equanime_hal_uio_open("dm6446_media");
	if (!hd)
		goto end;

	/* TODO setup the memory */
	mem = equanime_hal_uio_map(hd, 0);

	osd2.c = equ_controller_register(&_controller, &osd2);
	/* TODO check !osd2.c */
	if (!_dm6446_setup(osd2.c, hd, &osd2.dm6446))
	{

	}
	_io_setup(osd2.c);

	return EINA_TRUE;

controller_clean:
	//equ_controller_unregister(osd_controller);
media_clean:
	/* equ_hal_i2c_delete(ths8200); */
	equanime_hal_uio_close(hd);
end:
	return EINA_FALSE;
}

static void module_shutdown(void)
{
	//equ_controller_unregister(osd2);
}

EINA_MODULE_INIT(module_init);
EINA_MODULE_SHUTDOWN(module_shutdown);

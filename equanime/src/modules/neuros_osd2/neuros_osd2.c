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

struct dm6446 dm6446;

Eina_Bool module_init(void)
{
	Equanime_Hal_Device *hd;
	Equ_Hal_I2C *ths8200;
	void *mem;

	printf("Initializing Neuros OSD2 Board\n");
	/* get the companion devices */
	/* TODO ths7313 (0x2c) */
	/* TODO ths8200 (0x20) */
	ths8200 = equ_hal_i2c_get(0x20);
	/* something = ths8200_init(ths8200); */
	/* set this as one output of dm6446 */

	/* get the UIO devices */
	hd = equanime_hal_uio_open("dm6446_media");
	if (!hd)
		goto end;
	/* TODO setup the memory */
	mem = equanime_hal_uio_map(hd, 0);
	/* initialize the dm6446 controller, layers, input, output, etc */
	dm6446.osd = equanime_hal_uio_map(hd, 1);
	dm6446.venc = equanime_hal_uio_map(hd, 2);
	if (!dm6446_controller_init(&dm6446))
		goto media_clean;


	return EINA_TRUE;

media_clean:
	/* equ_hal_i2c_delete(ths8200); */
	equanime_hal_uio_close(hd);
end:
	return EINA_FALSE;
}

void module_shutdown(void)
{

}

EINA_MODULE_INIT(module_init);
EINA_MODULE_SHUTDOWN(module_shutdown);

#include "Equanime.h"
#include "equanime_private.h"
#include "dm6446_regs.h"

struct dm6446 dm6446;

Eina_Bool module_init(void)
{
	Equanime_Hal_Device *hd;
	Equanime_Hal_Device *hdc;
	void *mem;

	printf("Initializing Neuros OSD2 Board\n");
	/* get the UIO devices */
	hd = equanime_hal_uio_open("equanime-host");
	if (!hd)
		goto end;
	mem = equanime_hal_uio_map(hd, 0);

	hdc = equanime_hal_uio_open("dm6446-media");
	if (!hdc)
		goto host_clean;

	/* TODO setup the memory */

	/* initialize the dm6446 controller, layers, input, output, etc */
	dm6446.osd = equanime_hal_uio_map(hdc, 0);
	dm6446.venc = equanime_hal_uio_map(hdc, 1);
	if (!dm6446_controller_init(&dm6446))
		goto media_clean

	return EINA_TRUE;

media_clean:
	equanime_hal_uio_unmap(hdc, 0);
	equanime_hal_uio_unmap(hdc, 1);
	equanime_hal_uio_close(hdc);
host_clean:
	equanime_hal_uio_unmap(hd, 0);
	equanime_hal_uio_close(hd);
end:
	return EINA_FALSE;
}

void module_shutdown(void)
{

}

EINA_MODULE_INIT(module_init);
EINA_MODULE_SHUTDOWN(module_shutdown);

#include "Equanime.h"
#include "equanime_private.h"
#include "dm6446_regs.h"

struct dm6446 dm6446;

Eina_Bool module_init(void)
{
	Equanime_Hal_Device *hd;
	Equanime_Hal_Device *hdc;

	printf("Initializing Neuros OSD2 Board\n");
	/* get the host and setup the memory pool */
	hd = equanime_hal_uio_open("equanime-host");
	if (!hd)
		return EINA_FALSE;
	/* get the controller device */
	hdc = equanime_hal_uio_open("dm6446-media");
	if (!hdc)
	{
		equanime_hal_uio_close(hd);
		return EINA_FALSE;
	}
#if 0
	dm6446.venc = map[0];
	dm6446.osd = map[1];
	etc
#endif
	/* initialize the dm6446 controller, layers, input, output, etc */
	if (!dm6446_controller_init(&dm6446))
	{
		equanime_hal_uio_close(hdc);
		equanime_hal_uio_close(hd);
		return EINA_FALSE;
	}
}

void module_shutdown(void)
{

}

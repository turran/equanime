#include "Equ_Server.h"

#if 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define CONTROLLER_NAME "Dummy UIO"
#define DRIVER_NAME "uio_dummy" // useful for testing the uio interface
/*============================================================================*
 *                               Controller                                   *
 *============================================================================*/
static int controller_probe(Equ_Controller *ec)
{
	Equanime_Hal_Device *device;
	/* check if the driver exists */
	device = equanime_hal_uio_open(DRIVER_NAME);
	if (!device)
		return 0;
	equanime_controller_data_set(ec, device);

	return 1;
}

static void controller_remove(Equ_Controller *ec)
{
	Equanime_Hal_Device *device;

	/* unregister the controller */
	device = equanime_controller_data_get(ec);
	/* close the device */
	equanime_hal_uio_close(device);
}

static Equ_Controller_Description dummy_description =
{
	.name = CONTROLLER_NAME,
};

static Equ_Controller_Functions dummy_functions =
{
	.probe = &controller_probe,
	.remove = &controller_remove,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#endif
Eina_Bool module_init(void)
{
	printf("Dummy Module init\n");
	return EINA_TRUE;
}

void module_shutdown(void)
{
	printf("Dummy Module exit\n");
}

EINA_MODULE_INIT(module_init);
EINA_MODULE_SHUTDOWN(module_shutdown);

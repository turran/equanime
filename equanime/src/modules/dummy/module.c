#include "Eina.h"
#include "Equanime.h"
#include "Equanime_Module.h"




/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
#define CONTROLLER_NAME "Dummy UIO"
#define DRIVER_NAME "uio_dummy" // useful for testing the uio interface

/*============================================================================*
 *                               Controller                                   * 
 *============================================================================*/
static int controller_probe(Equanime_Controller *ec)
{
	Equanime_Hal_Device *device;
	/* check if the driver exists */
	device = equanime_hal_uio_open(DRIVER_NAME);
	if (!device)
		return 0;
	equanime_controller_data_set(ec, device);
	
	return 1;
}

static void controller_remove(Equanime_Controller *ec)
{
	Equanime_Hal_Device *device;
	
	/* unregister the controller */
	device = equanime_controller_data_get(ec);
	/* close the device */
	equanime_hal_uio_close(device);
}

static Equanime_Controller_Description dummy_description = 
{
	.name = CONTROLLER_NAME,
};

static Equanime_Controller_Functions dummy_functions =
{
	.probe = &controller_probe,
	.remove = &controller_remove,		
};

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
int module_init(void)
{
	/* register the controller */
	if (!equanime_controller_register(&dummy_description, &dummy_functions))
		return 0;
	return 1;
}

void module_exit(void)
{
	equanime_controller_unregister(&dummy_description);
}


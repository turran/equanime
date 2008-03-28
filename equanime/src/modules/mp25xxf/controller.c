#include "Eina.h"
#include "Equanime.h"
#include "Equanime_Module.h"

#include "mp25xxf.h"

#define DRIVER_NAME "mp25xxf_mlc"
//#define DRIVER_NAME "uio_dummy" // useful for testing the uio interface

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _remove(Equanime_Controller *c)
{
	Equanime_Hal_Device *device;
			
	/* unregister the controller */
	device = equanime_controller_data_get(c);
	
	/* close the device */
	equanime_hal_uio_close(device);
}

static int _probe(Equanime_Controller *c)
{
	Equanime_Hal_Device *device;
		
	/* check if the driver exists */
	device = equanime_hal_uio_open(DRIVER_NAME);
	if (!device) return 0;
		
	equanime_controller_data_set(c, device);
	return 1;
}

static Equanime_Controller_Description mp25xxf_description = 
{
	.name = "MagicEyes MP25XXF",
};

static Equanime_Controller_Functions mp25xxf_functions =
{
	.probe = &_probe,
	.remove = &_remove,		
};
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
int module_init(void)
{
	return equanime_controller_register(&mp25xxf_description, &mp25xxf_functions);
}

void module_exit(void)
{
	equanime_controller_unregister(&mp25xxf_description);
}

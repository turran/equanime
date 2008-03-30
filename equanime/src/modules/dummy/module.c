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
 *                                 Global                                     * 
 *============================================================================*/
int module_init(void)
{
	/* register the controller */
	if (!equanime_controller_register(&dm320_description, &dm320_functions))
		return 0;
	return 0;
}

void module_exit(void)
{
	equanime_controller_unregister(&dm320_description);
}


#include "Equanime.h"
#include "equanime_private.h"
#include "mp25xxf.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static Equanime_Controller_Description mp25xxf_description = 
{
	.name = "MagicEyes MP25XXF",
	.fncs = NULL,
};

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

void mp25xxf_controller_init(void)
{
	Equanime_Controller *ec;
	Equanime_Hal_Device *device;
	
	/* check if the driver exists */
	device = equanime_hal_uio_open("mp25xxf_mlc");
	if (!device) return;
	
	/* register the new controller */
	ec = equanime_controller_register(&mp25xxf_description));
	if (!ec) return;
	
	equanime_controller_data_set(ec, device);
	/* setup every layer */
	mp25xxf_rgb_init();
}

void mp25xxf_controller_exit(void)
{
	Equanime_Hal_Device *device;
	
	/* shutdown the layers */
	mp25xxf_rgb_exit();
	/* unregister the controller */
	//device = equanime_controller_data_get()
	//equanime_controller_unregister(&mp25xxf_description);
	/* close the device */
	equanime_hal_uio_close(mp25xxf_device);
}
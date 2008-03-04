#include "Equanime.h"
#include "equanime_private.h"
#include "mp25xxf.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Equanime_Controller_Description mp25xxf_description = 
{
	.name = "MagicEyes MP25XXF",
};

void mp25xxf_controller_init(void)
{
	/* check if the driver exists */
	/* register the new controller */
	equanime_controller_register(&mp25xxf_description);
	/* setup every layer */
	mp25xxf_rgb_init();

}

void mp25xxf_controller_exit(void)
{
	/* unregister the controller */
	/* shutdown the layers */
	mp25xxf_rgb_exit();
}
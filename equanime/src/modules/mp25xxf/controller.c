#include "Equanime.h"
#include "equanime_private.h"

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
}

void mp25xxf_controller_exit(void)
{
	/* unregister the controller */
}
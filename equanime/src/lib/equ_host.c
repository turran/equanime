#include "Equanime.h"
#include "equanime_private.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Host
{
	Equ_Host_Backend *backend;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void equ_host_register(Equ_Host_Backend *hb)
{

}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Equ_Surface * equ_host_surface_get(Equ_Host *h, uint32_t w, uint32_t h,
		Equ_Format fmt)
{


}

EAPI Equ_Controller * equ_host_controller_get(Equ_Host *h)
{

}

EAPI Equ_Component * equ_host_component_get(Equ_Host *h)
{

}

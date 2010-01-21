#include "Equanime.h"
#include "equ_private.h"
/**
 * A host is just the main element that owns a controller, different
 * components, surface, allocators, etc
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Host
{
	char *name;
	int id;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/* TODO add a way to know the alignment */
EAPI Equ_Surface * equ_host_surface_get(Equ_Host *host, uint32_t w, uint32_t h,
		Equ_Format fmt)
{
	Equ_Surface *s;
	void *ptr;

	/* TODO allocate the data */
	//s = equ_surface_new(h, w, h, fmt);

	return s;
}

EAPI void equ_host_controllers_get(Equ_Host *h, Equ_Cb cb, void *cb_data)
{
	Equ_Controller *c;

}

EAPI void equ_host_components_get(Equ_Host *h, Equ_Cb cb, void *cb_data)
{
	Equ_Component *c;
}

/**
 * Get all the available hosts registered
 * @param e The Equanime connection
 * @param cb The callback function to call whenever the data is received
 * @param cb_data The data to pass to the callback function
 */
EAPI void equ_hosts_get(Equanime *e, Equ_Cb cb, void *cb_data)
{
	Equ_Host *h;
}

EAPI const char * equ_host_name_get(Equanime *e, Equ_Host *h)
{
	return h->name;
}

EAPI void equ_host_delete(Equ_Host *h)
{
	free(h);
}

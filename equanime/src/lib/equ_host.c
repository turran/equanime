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
 * @parami[in] e The Equanime connection
 * @param[in] cb The callback function to call whenever the data is received
 * @param[in] cb_data The data to pass to the callback function
 */
EAPI void equ_hosts_get(Equanime *e, Equ_Cb cb, void *cb_data)
{
	Equ_Message_Hosts_Get m;
	Equ_Reply_Hosts_Get *r;
	Equ_Error error;
	int i;

	/* send the command to the server */
	error = equ_message_server_send(e, EQU_MSG_TYPE_HOSTS_GET, &m, 0, (void **)&r);
	if (error) return;
	/* allocate all the hosts and give them back to the user */
	for (i = 0; i < r->hosts_count; i++)
	{
		Equ_Host *h;

		h = malloc(sizeof(Equ_Host));
		printf("host %d %p\n", r->hosts[i].id, r->hosts[i].name);
		h->name = strdup(r->hosts[i].name);
		h->id = r->hosts[i].id;

		if (!cb(h, cb_data))
			break;
	}
	free(r);
}

EAPI const char * equ_host_name_get(Equanime *e, Equ_Host *h)
{
	return h->name;
}

/**
 * Deletes a host
 * @param[in] h The host to delete
 */
EAPI void equ_host_delete(Equ_Host *h)
{
	free(h->name);
	free(h);
}

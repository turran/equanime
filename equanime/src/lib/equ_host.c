#include <string.h>

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
	Equ_Common_Id id;
	const char *name;
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

EAPI Equ_Controller * equ_host_controller_get(Equ_Host *h, const char *name)
{

}

EAPI void equ_host_components_get(Equ_Host *h, Equ_Cb cb, void *cb_data)
{
	Equ_Component *c;
}

/**
 * Get a host from its name
 * @param[in] e The Equanime connection
 * @param[in] name The host name
 * @return The host
 */
EAPI Equ_Host * equ_host_get(Equanime *e, const char *name)
{
	Equ_Host *h;
	Equ_Message_Host_Get m;
	Equ_Reply_Host_Get *r = NULL;
	Equ_Error error;
	int i;

	m.name = name;
	error = equ_message_server_send(e, EQU_MSG_TYPE_HOST_GET, &m, 0, (void **)&r);
	if (error) return NULL;

	h = malloc(sizeof(Equ_Host));
	h->name = strdup(name);
	h->id = r->id;

	free(r);

	return h;
}

/**
 * Get all the available hosts registered
 * @param[in] e The Equanime connection
 * @param[in] cb The callback function to call whenever the data is received
 * @param[in] cb_data The data to pass to the callback function
 */
EAPI void equ_hosts_get(Equanime *e, Equ_Cb cb, void *cb_data)
{
	Equ_Message_Hosts_Get m;
	Equ_Reply_Hosts_Get *r = NULL;
	Equ_Error error;
	Equ_Host *h;
	int i;

	/* send the command to the server */
	error = equ_message_server_send(e, EQU_MSG_TYPE_HOSTS_GET, &m, 0, (void **)&r);
	if (error) return;
	/* allocate all the hosts and give them back to the user */
	for (i = 0; i < r->ids_count; i++)
	{
		h = calloc(1, sizeof(Equ_Host));
		h->id = r->ids[i];
		if (!cb(h, cb_data))
			break;
	}
	free(r);
}

/**
 * Get all the controllers relative to a host
 * @param[in] e The Equanime connection
 * @param[in] h The host to get the controllers from
 * @param[in] cb The callback function to call whenever the data is received
 * @param[in] cb_data The data to pass to the callback function
 */
EAPI void equ_host_controllers_get(Equanime *e, Equ_Host *h, Equ_Cb cb,
		void *cb_data)
{
	Equ_Message_Controllers_Get m;
	Equ_Reply_Controllers_Get *r = NULL;
	Equ_Error error;
	Equ_Controller *c;
	int i;

	/* send the command to the server */
	m.host_id = h->id;
	error = equ_message_server_send(e, EQU_MSG_TYPE_CONTROLLERS_GET, &m, 0, (void **)&r);
	if (error) return;
	/* allocate all the hosts and give them back to the user */
	for (i = 0; i < r->ids_count; i++)
	{
		c = equ_controller_new(h, r->ids[i]);
		if (!cb(c, cb_data))
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
	if (h->name)
		free(h->name);
	free(h);
}

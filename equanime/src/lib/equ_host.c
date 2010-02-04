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

typedef struct _Equ_Host_Get_Cb
{
	Equ_Host *host;
	const char *name;
} Equ_Host_Get_Cb;

typedef struct _Equ_Host_Controller_Get_Cb
{
	Equ_Controller *controller;
	const char *name;
	Equanime *e;
} Equ_Host_Controller_Get_Cb;

static int _host_get_cb(void *data, void *cb_data)
{
	Equ_Host *h = data;
	Equ_Host_Get_Cb *cb = cb_data;

	if (!cb->name)
	{
		cb->host = h;
		return EINA_FALSE;
	}

	if (!strcmp(h->name, cb->name))
	{
		cb->host = h;
		return EINA_FALSE;
	}
	equ_host_delete(h);

	return EINA_TRUE;
}

static int _host_controller_get_cb(void *data, void *cb_data)
{
	Equ_Controller *c = data;
	Equ_Host_Controller_Get_Cb *cb = cb_data;

	if (!cb->name)
	{
		cb->controller = c;
		return EINA_FALSE;
	}

	if (!strcmp(equ_controller_name_get(cb->e, c), cb->name))
	{
		cb->controller = c;
		return EINA_FALSE;
	}
	equ_controller_delete(c);

	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Get a surface from the host
 * @param[in] e The Equanime connection
 * @param[in] host The host to get the surface from
 * @param[in] w The width
 * @param[in] h The height
 * @param[in] fmt The format of the surface
 * @param[in] type The surface type
 */
EAPI Equ_Surface * equ_host_surface_get(Equanime *e, Equ_Host *host, uint32_t w,
		uint32_t h, Equ_Format fmt, Equ_Surface_Type type)
{
	Equ_Message_Surface_Get m;
	Equ_Reply_Surface_Get *r = NULL;
	Equ_Error error;
	Equ_Surface *s;

	m.host_id = host->id;
	m.w = w;
	m.h = h;
	m.fmt = fmt;
	m.type = type;

	error = equ_message_server_send(e, EQU_MSG_TYPE_SURFACE_GET, &m, 0, (void **)&r);
	if (error) return NULL;

	s = equ_surface_new(r->id, w, h, fmt, type);

	free(r);

	return s;
}

EAPI void equ_host_components_get(Equ_Host *h, Equ_Cb cb, void *cb_data)
{
	Equ_Component *c;
}

/**
 * Get a host from its name
 * @param[in] e The Equanime connection
 * @param[in] name The host name, if NULL it will return the first host found
 * @return The host
 */
EAPI Equ_Host * equ_host_get(Equanime *e, const char *name)
{
	Equ_Host_Get_Cb cb;

	cb.name = name;
	cb.host = NULL;
	equ_hosts_get(e, _host_get_cb, &cb);

	return cb.host;
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
	for (i = 0; i < r->hosts_count; i++)
	{
		h = calloc(1, sizeof(Equ_Host));
		h->id = r->hosts[i].id;
		h->name = strdup(r->hosts[i].name);
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
	for (i = 0; i < r->controllers_count; i++)
	{
		c = equ_controller_new(h, r->controllers[i].id, r->controllers[i].name);
		if (!cb(c, cb_data))
			break;
	}
	free(r);
}

/**
 * Get a controller from its name
 * @param[in] e The Equanime connection
 * @param[in] h The host to get the controller from
 * @param[in] name The controller name, if NULL it will return the first controller found
 * @return The controller
 */
EAPI Equ_Controller * equ_host_controller_get(Equanime *e, Equ_Host *h, const char *name)
{
	Equ_Host_Controller_Get_Cb cb;

	cb.name = name;
	cb.controller = NULL;
	cb.e = e;

	equ_host_controllers_get(e, h, _host_controller_get_cb, &cb);

	return cb.controller;
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

#include "Equanime.h"
#include "equanime_private.h"
/**
 * A host is just the main element that owns a controller, different
 * components, surface, allocators, etc
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Host
{
	const char *name;
	void *data;

	/* TODO add the memory areas */
	Eina_List *controllers;
	Eina_List *components;
};

Eina_List *_hosts = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Host * equ_host_register(const char *name, void *data)
{
	Equ_Host *h;

	h = calloc(1, sizeof(Equ_Host));
	h->name = name;

	_hosts = eina_list_append(_hosts, h);

	return h;
}

Equ_Controller * equ_host_controller_register(Equ_Host *h,
		Equ_Controller_Backend *backend, const char *name, void *data)
{
	Equ_Controller *c;

	c = equ_controller_new(h, backend, name, data);
	h->controllers = eina_list_append(h->controllers, c);

	return c;
}
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
	Eina_List *l;

	EINA_LIST_FOREACH(h->controllers, l, c)
	{
		cb(c, cb_data);
	}
}

EAPI void equ_host_components_get(Equ_Host *h, Equ_Cb cb, void *cb_data)
{
	Equ_Component *c;
	Eina_List *l;

	EINA_LIST_FOREACH(h->components, l, c)
	{
		cb(c, cb_data);
	}
}

EAPI void equ_hosts_get(Equ_Cb cb, void *cb_data)
{
	Equ_Host *h;
	Eina_List *l;

	EINA_LIST_FOREACH(_hosts, l, h)
	{
		cb(h, cb_data);
	}
}

EAPI const char * equ_host_name_get(Equ_Host *h)
{
	return h->name;
}

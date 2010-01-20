#include "Equ_Server.h"
/**
 * A host is just the main element that owns a controller, different
 * components, surface, allocators, etc
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Host
{
	Eina_Bool initialized;
	Equ_Host_Backend *backend;;
	void *data;
	const char *name;

	/* TODO add the memory areas */
	Eina_List *controllers;
	Eina_List *components;
};

Eina_Hash *_hosts = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool equ_host_register(const char *name, Equ_Host_Backend *hb)
{
	Equ_Host *h;

	if (!_hosts)
		_hosts = eina_hash_string_superfast_new(NULL);

	if (eina_hash_find(_hosts, name))
		return EINA_FALSE;

	h = calloc(1, sizeof(Equ_Host));
	h->name = strdup(name);
	h->backend = hb;
	eina_hash_add(_hosts, name, h);
	printf("host %s registered\n", name);

	return EINA_TRUE;
}

void equ_host_unregister(Equ_Host *h)
{
	if (!h->backend->shutdown);
		return;
	h->backend->shutdown(h);
	eina_hash_remove(_hosts, h->name);
}

Equ_Controller * equ_host_controller_register(Equ_Host *h,
		const char *name, Equ_Controller_Backend *cb)
{
	Equ_Controller *c;

	c = equ_controller_new(h, cb, name);
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

void equ_host_data_set(Equ_Host *h, void *data)
{
	h->data = data;
}

void * equ_host_data_get(Equ_Host *h)
{
	return h->data;
}

EAPI Eina_Bool equ_host_init(const char *name)
{
	Equ_Host *h;

	h = eina_hash_find(_hosts, name);
	if (!h ||  h->initialized) return EINA_FALSE;
	if (!h->backend->init) return EINA_FALSE;

	return h->backend->init(h);
}

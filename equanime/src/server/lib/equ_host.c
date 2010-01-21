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

	Eina_List *controllers;
	Eina_List *components;
	Eina_List *pools;
};

Eina_Hash *_hosts = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eina_Bool equ_host_register(const char *name, Equ_Host_Backend *hb)
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

EAPI void equ_host_unregister(Equ_Host *h)
{
	if (!h->backend->shutdown);
		return;
	h->backend->shutdown(h);
	eina_hash_remove(_hosts, h->name);
}

EAPI Equ_Controller * equ_host_controller_register(Equ_Host *h,
		const char *name, Equ_Controller_Backend *cb)
{
	Equ_Controller *c;

	c = equ_controller_new(h, cb, name);
	h->controllers = eina_list_append(h->controllers, c);

	return c;
}

EAPI Equ_Pool * equ_host_pool_register(Equ_Host *h,
		const char *name, uint32_t tmask, Equ_Pool_Backend *pb)
{
	Equ_Pool *p;

	p = equ_pool_new(h, pb, name, tmask);
	h->pools = eina_list_append(h->pools, p);
}

/* TODO add a way to know the alignment */
EAPI Equ_Surface * equ_host_surface_get(Equ_Host *host, uint32_t w, uint32_t h,
		Equ_Format fmt, Equ_Surface_Type type)
{
	Equ_Pool *p;
	Eina_List *l;
	Equ_Surface *s;
	void *data = NULL;

	EINA_LIST_FOREACH(host->pools, l, p)
	{
		if (data = equ_pool_alloc(p, w * h * fmt, type))
			break;
	}
	if (!data)
		return NULL;

	s = equ_surface_new(p, w, h, fmt, type, data);
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
	
#if 0
	Equ_Host *h;
	Eina_List *l;

	EINA_LIST_FOREACH(_hosts, l, h)
	{
		cb(h, cb_data);
	}
#endif
}

EAPI const char * equ_host_name_get(Equ_Host *h)
{
	return h->name;
}

EAPI void equ_host_data_set(Equ_Host *h, void *data)
{
	h->data = data;
}

EAPI void * equ_host_data_get(Equ_Host *h)
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

#include "Equ_Server.h"
#include "equ_server_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Pool
{
	const char *name;
	Equ_Pool_Backend *backend;
	uint32_t stype;
	void *data;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Equ_Pool * equ_pool_new(Equ_Controller *c, Equ_Input_Backend *pb,
		const char *name, uint32_t stype)
{
	Equ_Pool *p;

	p = malloc(sizeof(Equ_Pool));
	p->backend = pb;
	p->name = name;
	p->stype = stype;

	return p;
}

EAPI void * equ_pool_alloc(Equ_Pool *p, size_t bytes)
{
	return p->backend->alloc(p, bytes);
}

EAPI void equ_pool_free(Equ_Pool *p, void *data)
{
	p->backend->free(p, data);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_pool_data_set(Equ_Pool *p, void *data)
{
	p->data = data;
}

EAPI void * equ_pool_data_get(Equ_Pool *p, void *data)
{
	return p->data;
}

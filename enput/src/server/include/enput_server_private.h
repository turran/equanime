#ifndef _EQU_SERVER_PRIVATE_H
#define _EQU_SERVER_PRIVATE_H

#include <string.h>
#include "Eshm.h"

#define ERR(...) EINA_LOG_DOM_ERR(EQU_LOG_DOM, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(EQU_LOG_DOM, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(EQU_LOG_DOM, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(EQU_LOG_DOM, __VA_ARGS__)

/* equ_core.c */
extern int equ_log;

void equ_controller_layer_unregister(Equ_Controller *c, Equ_Layer *l);
void equ_controller_output_unregister(Equ_Controller *c, Equ_Output *o);

/* equ_pool.c */
Equ_Pool * equ_pool_new(Equ_Host *h, Equ_Pool_Backend *pb,
		const char *name, uint32_t stype);
void * equ_pool_alloc(Equ_Pool *p, size_t bytes);
void equ_pool_free(Equ_Pool *p, void *data);
void equ_pool_data_set(Equ_Pool *p, void *data);
void * equ_pool_data_get(Equ_Pool *p);

#endif

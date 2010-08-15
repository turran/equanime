#ifndef _EQU_SERVER_PRIVATE_H
#define _EQU_SERVER_PRIVATE_H

#include <string.h>
#include "Eshm.h"

#define ERR(...) EINA_LOG_DOM_ERR(EQU_LOG_DOM, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(EQU_LOG_DOM, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(EQU_LOG_DOM, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(EQU_LOG_DOM, __VA_ARGS__)

void equ_controller_layer_unregister(Equ_Controller *c, Equ_Layer *l);
void equ_controller_output_unregister(Equ_Controller *c, Equ_Output *o);

#endif

#ifndef _EQU_SERVER_PRIVATE_H
#define _EQU_SERVER_PRIVATE_H

#include <string.h>
#include "Eshm.h"

void equ_controller_layer_unregister(Equ_Controller *c, Equ_Layer *l);
void equ_controller_output_unregister(Equ_Controller *c, Equ_Output *o);

#endif

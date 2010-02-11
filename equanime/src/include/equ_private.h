#ifndef _EQU_PRIVATE_H
#define _EQU_PRIVATE_H

#include <string.h>
#include <stdlib.h>

#include "equ_common.h"

#define ERR(...) EINA_LOG_DOM_ERR(equ_log, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(equ_log, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(equ_log, __VA_ARGS__)

extern equ_log;
Equ_Error equ_message_server_send(Equanime *e, Equ_Message_Type type,
		void *data, double timeout, void **rdata);
/**
 *
 */
struct _Equ_Rotator
{
	Equ_Angle angle;
};

/**
 *
 */
struct _Equ_Scaler
{
	uint32_t sw, sh, dw, dh;
};

struct _Equ_Csc
{
	Equ_Format sf;
	Equ_Format df;
};

/**
 *
 */
struct _Equ_Region
{
	Equ_Layer *layer;
	void *data;

	int x;
	int y;
	int w;
	int h;
	int level;
	unsigned char hidden;
};
/**
 *
 */
struct _Equ_Output
{
	const char *name;
	void *data;

	Equ_Controller *controller;
	Equ_Mode mode;
};
/**
 *
 */
struct _Equ_Input
{
	const char *name;
	void *data;

	Equ_Mode *mode;
};


Equ_Layer * equ_layer_new(Equanime *e, Equ_Controller *c, Equ_Common_Id id, const char * name);
Equ_Surface * equ_surface_new(Equ_Common_Id id, uint32_t w, uint32_t h, Equ_Format fmt,
		Equ_Surface_Type type);
Equ_Controller * equ_controller_new(Equ_Host *h, Equ_Common_Id id, char *name);

#endif /*_EQUANIME_PRIVATE_H*/

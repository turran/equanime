#ifndef _EQU_PRIVATE_H
#define _EQU_PRIVATE_H

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

#endif /*_EQUANIME_PRIVATE_H*/

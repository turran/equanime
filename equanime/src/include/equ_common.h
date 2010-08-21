#ifndef _EQU_COMMON_H
#define _EQU_COMMON_H

#include "Eix.h"

#define EQUANIME_NAME "equanime"
#define EQUANIME_PORT 0xea

#define EQU_MSGS_NUM (EQU_MSG_LAST - EIX_REPLY_LAST)
#define EQU_INDEX(n) (n - EIX_REPLY_LAST)
/*
 * TODO
 * - Add a way to send messages from the server to the client that are *not* replies
 */

typedef int Equ_Common_Id;

typedef enum _Equ_Error
{
	EQU_ERR_NEXIST = EIX_ERRORS,
	EQU_ERRORS,
} Equ_Error;

/* common data definitions */
typedef struct _Equ_Common_Host
{
	Equ_Common_Id id;
	char *name;
} Equ_Common_Host;

typedef struct _Equ_Common_Controller
{
	Equ_Common_Id id;
	char *name;
} Equ_Common_Controller;

typedef struct _Equ_Layer_Info
{
	Equ_Common_Id id;
	char *name;
} Equ_Layer_Info;

typedef struct _Equ_Common_Surface
{
	Equ_Common_Id id;
	uint32_t w;
	uint32_t h;
	char *shid;
	Equ_Surface_Type type;
	Equ_Format fmt;
} Equ_Common_Surface;

typedef struct _Equ_Layer_Caps
{
	uint32_t fmt_mask;
	uint32_t flags_mask;
} Equ_Layer_Caps;

typedef struct _Equ_Layer_Status
{
	int32_t x;
	int32_t y;
	uint32_t w;
	uint32_t h;
	Equ_Format fmt;
} Equ_Layer_Status;

typedef struct _Equ_Surface_Data
{
	Equ_Format fmt;
	union {
		struct {
			uint32_t *plane0;
		} rgb888;
		struct {
			uint32_t *plane0;
		} argb8888;
	} data;
} Equ_Surface_Data;

typedef enum
{
	EQU_DATA_HOST,
	EQU_DATA_CONTROLLER,
	EQU_DATA_LAYER,
	EQU_DATA_LAYER_CAPS,
	EQU_DATA_LAYER_STATUS,
	EQU_DATAS
} Equ_Data;

typedef enum _Equ_Message
{
	EQU_MSG_HOSTS_GET = EIX_REPLY_LAST,
	EQU_MSG_CONTROLLERS_GET,
	EQU_MSG_LAYERS_GET,
	EQU_MSG_LAYER_CAPS_GET,
	EQU_MSG_LAYER_STATUS_GET,
	EQU_MSG_SURFACE_GET,
	EQU_MSG_SURFACE_PUT,
	EQU_MSG_SURFACE_UPLOAD,
	EQU_MSG_SURFACE_DOWNLOAD,
	EQU_MSG_LAST,
} Equ_Message;

typedef enum _Equ_Reply
{
	EQU_REPLY_HOSTS_GET = EQU_MSG_LAST,
	EQU_REPLY_CONTROLLERS_GET,
	EQU_REPLY_LAYERS_GET,
	EQU_REPLY_LAYER_CAPS_GET,
	EQU_REPLY_LAYER_STATUS_GET,
	EQU_REPLY_SURFACE_GET,
	EQU_REPLY_SURFACE_DOWNLOAD,
	EQU_REPLY_LAST,
} Equ_Reply;

typedef struct _Equ_Message_Sync
{
} Equ_Message_Sync;

typedef struct _Equ_Message_Hosts_Get
{
} Equ_Message_Hosts_Get;

typedef struct _Equ_Message_Controllers_Get
{
	Equ_Common_Id host_id;
} Equ_Message_Controllers_Get;

typedef struct _Equ_Message_Layers_Get
{
	Equ_Common_Id controller_id;
} Equ_Message_Layers_Get;

typedef struct _Equ_Message_Layer_Caps_Get
{
	Equ_Common_Id layer_id;
} Equ_Message_Layer_Caps_Get;

typedef struct _Equ_Message_Layer_Status_Get
{
	Equ_Common_Id layer_id;
} Equ_Message_Layer_Status_Get;

typedef struct _Equ_Message_Surface_Get
{
	Equ_Common_Id host_id;
	unsigned int w;
	unsigned int h;
	Equ_Format fmt;
	Equ_Surface_Type type;
} Equ_Message_Surface_Get;

typedef struct _Equ_Message_Surface_Put
{
	Equ_Common_Id surface_id;
	Equ_Common_Id layer_id;
	int dx;
	int dy;
	int cx;
	int cy;
	int cw;
	int ch;
} Equ_Message_Surface_Put;

typedef struct _Equ_Message_Surface_Upload
{
	Equ_Common_Id surface_id;
	int sx;
	int sy;
	int sw;
	int sh;
	void *pixels;
	int pixels_size;
} Equ_Message_Surface_Upload;

typedef struct _Equ_Message_Surface_Download
{
	Equ_Common_Id surface_id;
	int sx;
	int sy;
	int sw;
	int sh;
} Equ_Message_Surface_Download;

typedef struct _Equ_Reply_Sync
{
} Equ_Reply_Sync;

typedef struct _Equ_Reply_Hosts_Get
{
	int hosts_count;
	Equ_Common_Host *hosts;
} Equ_Reply_Hosts_Get;

typedef struct _Equ_Reply_Controllers_Get
{
	int controllers_count;
	Equ_Common_Controller *controllers;
} Equ_Reply_Controllers_Get;

typedef struct _Equ_Reply_Layers_Get
{
	int layers_count;
	Equ_Layer_Info *layers;
} Equ_Reply_Layers_Get;

typedef struct _Equ_Reply_Layer_Caps_Get
{
	Equ_Layer_Caps caps;
} Equ_Reply_Layer_Caps_Get;

typedef struct _Equ_Reply_Layer_Status_Get
{
	Equ_Layer_Status status;
} Equ_Reply_Layer_Status_Get;

typedef struct _Equ_Reply_Surface_Get
{
	Equ_Common_Id id;
	char *sh_id;
} Equ_Reply_Surface_Get;

typedef struct _Equ_Reply_Surface_Download
{
	void *pixels;
	int pixels_size;
} Equ_Reply_Surface_Download;

void equ_message_init(void);
void equ_message_shutdown(void);

#endif /*_EQU_COMMON_H*/

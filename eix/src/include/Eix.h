/* EIX Message and protocol library
 * Copyright (C) 2010 Jorge Luis Zapata
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef EIX_H_
#define EIX_H_

/**
 * @mainpage Eix
 * @image html enesim.png
 * @section intro Introduction
 * @section features Features
 * @section dependencies Dependencies
 * - Eina
 * @file
 * @brief Eix API
 */

#include "Eina.h"
#include "Eet.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Eix_Client Eix_Client;
typedef struct _Eix_Server Eix_Server;

typedef int (*Eix_Server_Process)(Eix_Client *c, unsigned int id, void *msg,
		void **reply);

EAPI extern int EIX_EVENT_SERVER_ADD;
EAPI extern int EIX_EVENT_SERVER_DEL;
EAPI extern int EIX_EVENT_CLIENT_ADD;
EAPI extern int EIX_EVENT_CLIENT_DEL;

typedef struct _Eix_Event_Server_Add
{
	Eix_Server *server;
} Eix_Event_Server_Add;

typedef struct _Eix_Event_Server_Del
{
	Eix_Server *server;
} Eix_Event_Server_Del;

typedef struct _Eix_Event_Client_Add
{
	Eix_Client *client;
} Eix_Event_Client_Add;

typedef struct _Eix_Event_Client_Del
{
	Eix_Client *client;
} Eix_Event_Client_Del;

typedef enum _Eix_Message_Id
{
	EIX_MESSAGE_SYNC,
	EIX_MESSAGE_LAST,
} Eix_Message_Id;

typedef enum _Eix_Reply_Id
{
	EIX_REPLY_SYNC = EIX_MESSAGE_LAST,
	EIX_REPLY_LAST,
} Eix_Reply_Id;

/*
 * this errors are well known error number to retrieve the correct
 * error from the message, it is used only internally, for API errors
 * use TODO
 */
typedef enum
{
	EIX_ERR_NONE,
	EIX_ERR_NEXIST,  /* a reixest for some element that doesnt exist */
	EIX_ERR_CODEC,   /* bad encoding or decoding */
	EIX_ERR_TIMEOUT, /* timeout error */
	EIX_ERRORS,
} Eix_Error;

typedef struct _Eix_Message_Sync
{
} Eix_Message_Sync;

typedef struct _Eix_Reply_Sync
{
} Eix_Reply_Sync;

EAPI void eix_init(void);
EAPI void eix_shutdown(void);
EAPI Eix_Server * eix_connect(const char *name, int port);
EAPI Eix_Server * eix_new(const char *name, int port,
		Eix_Server_Process process);
EAPI void eix_sync(Eix_Server *e);
EAPI void eix_server_message_add(Eix_Server *e,
		unsigned int id, Eet_Data_Descriptor *edd,
		unsigned int reply_id);
EAPI Eix_Error eix_message_server_send(Eix_Server *es, int type,
		void *data, double timeout, void **rdata);

#ifdef __cplusplus
}
#endif

#endif /*_EIX_H*/

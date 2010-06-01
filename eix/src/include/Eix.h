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

#define EIX_NAME "eixanime"
#define EIX_PORT 0xea

/* protocol structures */
typedef enum
{
	EIX_MSG_NO_REPLY,
	EIX_MSG_REPLY,
} Eix_Message_Reply;

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

/* A reply should always be the same value as the message this reply replies to
 * plus one
 */
typedef enum _Eix_Message_Name
{
	EIX_MSG_NAME_SYNC,
	EIX_MSG_NAME_SYNCR,
	EIX_MSG_NAMES
} Eix_Message_Name;

typedef enum _Eix_Message_Type
{
	EIX_MSG_TYPE_SYNC              = ((EIX_MSG_NAME_SYNC << 1) | EIX_MSG_REPLY),
	EIX_MSG_TYPE_SYNCR             = ((EIX_MSG_NAME_SYNC << 1) | EIX_MSG_NO_REPLY),
} Eix_Message_Type;

/*
 * A message is composed of:
 * +----+------+------+-----------------
 * | id | type | size |
 * +----+------+------+-----------------
 * <-----header------>.<-----body------>
 */

typedef struct _Eix_Message
{
	unsigned int id; /* id of the message */
	Eix_Message_Type type; /* type of message */
	unsigned int size; /* size of the body */
} Eix_Message;

typedef struct _Eix_Message_Sync
{
} Eix_Message_Sync;

/*
 * A reply is composed of:
 * +----+-------+------+-----------------
 * | id | error | size |
 * +----+-------+------+-----------------
 * <------header------>.<-----body------>
 */

typedef struct _Eix_Reply
{
	unsigned int id; /* id of the message this reply replies to */
	unsigned int error; /* in case of any error set by the daemon */
	unsigned int size; /* size of the body */
} Eix_Reply;

typedef struct _Eix_Reply_Sync
{
} Eix_Reply_Sync;

static inline Eix_Message_Name eix_message_name_get(Eix_Message_Type t)
{
	return (t & ~1) >> 1;
}

static inline Eina_Bool eix_message_reply_has(Eix_Message_Type t)
{
	if (t & EIX_MSG_REPLY)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}

/**
 * Given a message type return the name of the reply. Note that the reply's
 * name is always the message plus one.
 */
static inline Eina_Bool eix_message_reply_name_get(Eix_Message_Type t, Eix_Message_Name *n)
{
	if (eix_message_reply_has(t) == EINA_FALSE)
		return EINA_FALSE;
	*n = eix_message_name_get(t) + 1;
	return EINA_TRUE;
}

EAPI void eix_init(void);
EAPI void eix_shutdown(void);

void * eix_message_encode(Eix_Message_Name name, const void *data, int *size);
void * eix_message_decode(Eix_Message_Name name, const void *data, int size);

Eix_Message * eix_message_new(Eix_Message_Type type);

#ifdef __cplusplus
}
#endif

#endif /*_EIX_H*/

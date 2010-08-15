#ifndef ESHM_PRIVATE_H_
#define ESHM_PRIVATE_H_

#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "Eix.h"

#define ERR(...) EINA_LOG_DOM_ERR(eshm_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(eshm_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(eshm_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(eshm_log_dom, __VA_ARGS__)

extern int eshm_log_dom;

#define ESHMD_NAME "eshmd"
#define ESHMD_PORT 0x1b

/*
 * this errors are well known error number to retrieve the correct
 * error from the message, it is used only internally, for API errors
 * use TODO
 */
typedef enum
{
	ESHM_ERR_ACCESS = EIX_ERRORS,  /* permission errors */
	ESHM_ERR_EXIST,   /* a request for new segment that already exists */
	ESHM_ERR_NEXIST,  /* a request for a segment that no longer exists */
	ESHM_ERR_CODEC,   /* bad encoding or decoding */
	ESHM_ERR_TIMEOUT, /* timeout error */
	ESHM_ERRORS,
} Eshm_Error;

typedef enum _Eshm_Message_Type
{
	ESHM_MSG_SEGMENT_NEW = EIX_REPLY_LAST,
	ESHM_MSG_SEGMENT_GET,
	ESHM_MSG_SEGMENT_LOCK,
	ESHM_MSG_SEGMENT_UNLOCK,
	ESHM_MSG_SEGMENT_DELETE,
	ESHM_MSGS,
} Eshm_Message_Type;

typedef enum _Eshm_Reply_Type
{
	ESHM_REPLY_SEGMENT_NEW = ESHM_MSGS,
	ESHM_REPLY_SEGMENT_GET,
	ESHM_REPLY_SEGMENT_LOCK,
	ESHM_REPLIES,
} Eshm_Reply_Type;

typedef struct _Eshm_Message_Segment_New
{
	const char *id;
	unsigned int size;
} Eshm_Message_Segment_New;

typedef struct _Eshm_Message_Segment_Get
{
	const char *id;
} Eshm_Message_Segment_Get;

typedef struct _Eshm_Message_Segment_Delete
{
	const char *id;
} Eshm_Message_Segment_Delete;

typedef struct _Eshm_Message_Segment_Lock
{
	const char *id;
	unsigned char write;
} Eshm_Message_Segment_Lock;

typedef struct _Eshm_Message_Segment_Unlock
{
	const char *id;
} Eshm_Message_Segment_Unlock;

typedef struct _Eshm_Reply_Segment_New
{
	int shmid;
} Eshm_Reply_Segment_New;

typedef struct _Eshm_Reply_Segment_Get
{
	int shmid;
	size_t size;
} Eshm_Reply_Segment_Get;

typedef struct _Eshm_Reply_Segment_Lock
{
} Eshm_Reply_Segment_Lock;

void eshm_message_init(void);
void eshm_message_shutdown(void);
int eshm_message_server_send(int type, void *data, double timeout, void **rdata);

#endif /*ESHM_PRIVATE_H_*/

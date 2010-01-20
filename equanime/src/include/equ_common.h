#ifndef _EQU_COMMON_H
#define _EQU_COMMON_H

#define EQUANIME_NAME "equanime"
#define EQUANIME_PORT 0xea

/*
 * TODO
 * change the name from reply to payload, this is getting confusing
 */

/* protocol structures */
typedef enum
{
	EQU_MSG_NO_REPLY,
	EQU_MSG_REPLY,
} Equ_Message_Reply;

/*
 * this errors are well known error number to retrieve the correct
 * error from the message, it is used only internally, for API errors
 * use TODO
 */
typedef enum
{
	EQU_ERR_NONE,
	EQU_ERR_ACCESS,  /* permission errors */
	EQU_ERR_EXIST,   /* a request for new segment that already exists */
	EQU_ERR_NEXIST,  /* a request for a segment that no longer exists */
	EQU_ERR_CODEC,   /* bad encoding or decoding */
	EQU_ERR_TIMEOUT, /* timeout error */
	EQU_ERRORS,
} Equ_Error;

/* A reply should always be the same value as the message this reply replies to
 * plus one
 */
typedef enum
{
	EQU_MSG_NAME_SEGMENT_NEW,
	EQU_MSG_NAME_SEGMENT_NEWR,
	EQU_MSG_NAME_SEGMENT_GET,
	EQU_MSG_NAME_SEGMENT_GETR,
	EQU_MSG_NAME_SEGMENT_LOCK,
	EQU_MSG_NAME_SEGMENT_UNLOCK,
	EQU_MSG_NAME_SEGMENT_DELETE,
	EQU_MSG_NAMES
} Equ_Message_Name;

typedef enum
{
	EQU_MSG_TYPE_SEGMENT_NEW    = ((EQU_MSG_NAME_SEGMENT_NEW << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_SEGMENT_NEWR   = ((EQU_MSG_NAME_SEGMENT_NEWR << 1) | EQU_MSG_NO_REPLY),
	EQU_MSG_TYPE_SEGMENT_GET    = ((EQU_MSG_NAME_SEGMENT_GET << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_SEGMENT_GETR   = ((EQU_MSG_NAME_SEGMENT_GETR << 1) | EQU_MSG_NO_REPLY),
	EQU_MSG_TYPE_SEGMENT_LOCK   = ((EQU_MSG_NAME_SEGMENT_LOCK << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_SEGMENT_UNLOCK = ((EQU_MSG_NAME_SEGMENT_UNLOCK << 1) | EQU_MSG_REPLY),
	EQU_MSG_TYPE_SEGMENT_DELETE = ((EQU_MSG_NAME_SEGMENT_DELETE << 1) | EQU_MSG_NO_REPLY),
} Equ_Message_Type;


/*
 * A message is composed of:
 * +----+------+------+-----------------
 * | id | type | size |
 * +----+------+------+-----------------
 * <-----header------>.<-----body------>
 */

typedef struct _Equ_Message
{
	unsigned int id; /* id of the message */
	Equ_Message_Type type; /* type of message */
	unsigned int size; /* size of the body */
} Equ_Message;

typedef struct _Equ_Message_Segment_New
{
	const char *id;
	unsigned int size;
} Equ_Message_Segment_New;

typedef struct _Equ_Message_Segment_Get
{
	const char *id;
} Equ_Message_Segment_Get;

typedef struct _Equ_Message_Segment_Delete
{
	const char *id;
} Equ_Message_Segment_Delete;

typedef struct _Equ_Message_Segment_Lock
{
	const char *id;
	unsigned char write;
} Equ_Message_Segment_Lock;

typedef struct _Equ_Message_Segment_Unlock
{
	const char *id;
} Equ_Message_Segment_Unlock;

/*
 * A reply is composed of:
 * +----+-------+------+-----------------
 * | id | error | size |
 * +----+-------+------+-----------------
 * <------header------>.<-----body------>
 */

typedef struct _Equ_Reply
{
	unsigned int id; /* id of the message this reply replies to */
	unsigned int error; /* in case of any error set by the daemon */
	unsigned int size; /* size of the body */
} Equ_Reply;

typedef struct _Equ_Reply_Segment_New
{
	int shmid;
} Equ_Reply_Segment_New;

typedef struct _Equ_Reply_Segment_Get
{
	int shmid;
} Equ_Reply_Segment_Get;

Equ_Error equ_server_send(Equ_Message *m, void *data, double timeout, void **rdata);

inline Equ_Message_Name equ_message_name_get(Equ_Message_Type t);
inline Eina_Bool equ_message_reply_has(Equ_Message_Type t);
inline Eina_Bool equ_message_reply_name_get(Equ_Message_Type t, Equ_Message_Name *n);
void equ_message_init(void);
void equ_message_shutdown(void);
void * equ_message_encode(Equ_Message_Name name, const void *data, int *size);
void * equ_message_decode(Equ_Message_Name name, const void *data, int size);
Equ_Error equ_message_server_send(Equ_Message_Type type, void *data, double timeout, void **rdata);

#endif /*_EQU_COMMON_H*/

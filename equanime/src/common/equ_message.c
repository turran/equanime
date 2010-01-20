#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "Eet.h"
#include "Ecore.h"
#include "Ecore_Con.h"

#include "equ_common.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
Eet_Data_Descriptor_Class _eddc = {
	.version = EET_DATA_DESCRIPTOR_CLASS_VERSION,
	.name = "equ",
};

Eet_Data_Descriptor *_descriptors[EQU_MSG_NAMES];

static Equ_Message * _new(Equ_Message_Type type)
{
	static int id = 0;
	Equ_Message *m;

	m = malloc(sizeof(Equ_Message));
	m->id = id;
	m->type = type;
	/* TODO this will cause an overflow sometime */
	id++;
	return m;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
inline Equ_Message_Name equ_message_name_get(Equ_Message_Type t)
{
	return (t & ~1) >> 1;
}

inline Eina_Bool equ_message_reply_has(Equ_Message_Type t)
{
	if (t & EQU_MSG_REPLY)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}
/**
 * Given a message type return the name of the reply. Note that the reply's
 * name is always the message plus one.
 */
inline Eina_Bool equ_message_reply_name_get(Equ_Message_Type t, Equ_Message_Name *n)
{
	if (equ_message_reply_has(t) == EINA_FALSE)
		return EINA_FALSE;
	*n = equ_message_name_get(t) + 1;
	return EINA_TRUE;
}

void equ_message_init(void)
{
	Eet_Data_Descriptor *edd;
	/* create all the messages' data descriptors */

	/* segment new */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[EQU_MSG_NAME_SEGMENT_NEW] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Segment_New, "id", id, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Segment_New, "size", size, EET_T_UINT);
	/* segment new reply */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[EQU_MSG_NAME_SEGMENT_NEWR] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Segment_New, "shmid", shmid, EET_T_UINT);
}

void equ_message_shutdown(void)
{
	int i;

	/* remove all the messages's data descriptors */
	for (i = 0; i < EQU_MSG_NAMES; i++)
		eet_data_descriptor_free(_descriptors[i]);
}

void * equ_message_encode(Equ_Message_Name name, const void *data, int *size)
{
	return eet_data_descriptor_encode(_descriptors[name], data, size);
}

void * equ_message_decode(Equ_Message_Name name, const void *data, int size)
{
	return eet_data_descriptor_decode(_descriptors[name], data, size);
}

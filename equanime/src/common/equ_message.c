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
Eet_Data_Descriptor *_ddescriptors[EQU_DATAS];

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
Equ_Message * equ_message_new(Equ_Message_Type type)
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

void equ_message_init(void)
{
	Eet_Data_Descriptor *edd;
	/* create all the data descriptors */
	edd = eet_data_descriptor2_new(&_eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "name", name, EET_T_STRING);
	_ddescriptors[EQU_DATA_HOST] = edd;
	/* create all the messages' data descriptors */
	/* hosts get */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[EQU_MSG_NAME_HOSTS_GET] = edd;
	/* hosts get reply */
	edd = eet_data_descriptor2_new(&_eddc);
	_descriptors[EQU_MSG_NAME_HOSTS_GETR] = edd;
	EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(edd, Equ_Reply_Hosts_Get, "hosts", hosts, _ddescriptors[EQU_DATA_HOST]);
}

void equ_message_shutdown(void)
{
	int i;

	/* remove all the messages's data descriptors */
	for (i = 0; i < EQU_MSG_NAMES; i++)
		eet_data_descriptor_free(_descriptors[i]);
	for (i = 0; i < EQU_DATAS; i++)
		eet_data_descriptor_free(_ddescriptors[i]);
}

void * equ_message_encode(Equ_Message_Name name, const void *data, int *size)
{
	return eet_data_descriptor_encode(_descriptors[name], data, size);
}

void * equ_message_decode(Equ_Message_Name name, const void *data, int size)
{
	return eet_data_descriptor_decode(_descriptors[name], data, size);
}

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
#define DATA_DESCRIPTOR_ADD_VAR_ARRAY(edd, struct_type, name, member, subtype) \
     { \
	struct_type ___ett; \
	\
	eet_data_descriptor_element_add(edd, name, subtype, EET_G_VAR_ARRAY, \
					(char *)(&(___ett.member)) - (char *)(&(___ett)), \
					(char *)(&(___ett.member ## _count)) - (char *)(&(___ett)), /* 0,  */NULL, NULL); \
     }

/*Eet_Data_Descriptor_Class _eddc = {
	.version = EET_DATA_DESCRIPTOR_CLASS_VERSION,
	.name = "equ",
};*/

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
	Eet_Data_Descriptor_Class _eddc;

	/* create all the data descriptors */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Common_Host", sizeof(Equ_Common_Host));
	edd = eet_data_descriptor_stream_new(&_eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "name", name, EET_T_STRING);
	_ddescriptors[EQU_DATA_HOST] = edd;

	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Common_Controller", sizeof(Equ_Common_Controller));
	edd = eet_data_descriptor_stream_new(&_eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "name", name, EET_T_STRING);
	_ddescriptors[EQU_DATA_CONTROLLER] = edd;

	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Common_Layer", sizeof(Equ_Common_Layer));
	edd = eet_data_descriptor_stream_new(&_eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "name", name, EET_T_STRING);
	_ddescriptors[EQU_DATA_LAYER] = edd;
	/* create all the messages' data descriptors */
	/* Host messages/replies */
	/* hosts get */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Message_Hosts_Get", sizeof(Equ_Message_Hosts_Get));
	edd = eet_data_descriptor_stream_new(&_eddc);
	_descriptors[EQU_MSG_NAME_HOSTS_GET] = edd;
	/* hosts get reply */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Reply_Hosts_Get", sizeof(Equ_Reply_Hosts_Get));
	edd = eet_data_descriptor_stream_new(&_eddc);
	_descriptors[EQU_MSG_NAME_HOSTS_GETR] = edd;
	EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(edd, Equ_Reply_Hosts_Get, "hosts", hosts, _ddescriptors[EQU_DATA_HOST]);
	/* host get */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Message_Host_Get", sizeof(Equ_Message_Host_Get));
	edd = eet_data_descriptor_stream_new(&_eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Host_Get, "name", name, EET_T_STRING);
	_descriptors[EQU_MSG_NAME_HOST_GET] = edd;
	/* host get reply */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Reply_Host_Get", sizeof(Equ_Reply_Host_Get));
	edd = eet_data_descriptor_stream_new(&_eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Host_Get, "id", id, EET_T_UINT);
	_descriptors[EQU_MSG_NAME_HOST_GETR] = edd;

	/* Controllers messages/replies */
	/* controllers get */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Message_Controllers_Get", sizeof(Equ_Message_Controllers_Get));
	edd = eet_data_descriptor_stream_new(&_eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Controllers_Get, "host_id", host_id, EET_T_UINT);
	_descriptors[EQU_MSG_NAME_CONTROLLERS_GET] = edd;
	/* controllers get reply */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Reply_Controller_Get", sizeof(Equ_Reply_Controller_Get));
	edd = eet_data_descriptor_stream_new(&_eddc);
	_descriptors[EQU_MSG_NAME_CONTROLLERS_GETR] = edd;
	EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(edd, Equ_Reply_Controllers_Get, "controllers", controllers, _ddescriptors[EQU_DATA_CONTROLLER]);
	
	/* Layers messages/replies */
	/* layers get */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Message_Layers_Get", sizeof(Equ_Message_Layers_Get));
	edd = eet_data_descriptor_stream_new(&_eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Layers_Get, "controller_id", controller_id, EET_T_UINT);
	_descriptors[EQU_MSG_NAME_LAYERS_GET] = edd;
	/* layers get reply */
	eet_eina_stream_data_descriptor_class_set(&_eddc, "Equ_Reply_Layers_Get", sizeof(Equ_Reply_Layers_Get));
	edd = eet_data_descriptor_stream_new(&_eddc);
	_descriptors[EQU_MSG_NAME_LAYERS_GETR] = edd;
	EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(edd, Equ_Reply_Layers_Get, "layers", layers, _ddescriptors[EQU_DATA_LAYER]);
	/* Pool messages/replies */
	/* Surfaces messages/replies */
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

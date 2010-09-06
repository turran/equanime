#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "equ_enums.h"
#include "equ_common.h"
/**
 * @todo Some day we'll need to generate this functions automatically
 */
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

static Eet_Data_Descriptor *_descriptors[EQU_INDEX(EQU_REPLY_LAST)];
static Eet_Data_Descriptor *_ddescriptors[EQU_DATAS];

static void _message_init(void)
{
	Eet_Data_Descriptor *edd;
	Eet_Data_Descriptor_Class eddc;

	/* create all the data descriptors */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Common_Host", sizeof(Equ_Common_Host));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "name", name, EET_T_STRING);
	_ddescriptors[EQU_DATA_HOST] = edd;

	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Common_Controller", sizeof(Equ_Common_Controller));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "name", name, EET_T_STRING);
	_ddescriptors[EQU_DATA_CONTROLLER] = edd;

	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Layer_Info", sizeof(Equ_Layer_Info));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Common_Host, "name", name, EET_T_STRING);
	_ddescriptors[EQU_DATA_LAYER] = edd;

	/* create all the messages' data descriptors */
	/* Host messages/replies */
	/* hosts get */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Message_Hosts_Get", sizeof(Equ_Message_Hosts_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[EQU_INDEX(EQU_MSG_HOSTS_GET)] = edd;
	/* hosts get reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Reply_Hosts_Get", sizeof(Equ_Reply_Hosts_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(edd, Equ_Reply_Hosts_Get, "hosts", hosts, _ddescriptors[EQU_DATA_HOST]);
	_descriptors[EQU_INDEX(EQU_REPLY_HOSTS_GET)] = edd;

	/* Controllers messages/replies */
	/* controllers get */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Message_Controllers_Get", sizeof(Equ_Message_Controllers_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Controllers_Get, "host_id", host_id, EET_T_UINT);
	_descriptors[EQU_INDEX(EQU_MSG_CONTROLLERS_GET)] = edd;
	/* controllers get reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Reply_ControllerS_Get", sizeof(Equ_Reply_Controllers_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(edd, Equ_Reply_Controllers_Get, "controllers", controllers, _ddescriptors[EQU_DATA_CONTROLLER]);
	_descriptors[EQU_INDEX(EQU_REPLY_CONTROLLERS_GET)] = edd;

	/* Layers messages/replies */
	/* layers get */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Message_Layers_Get", sizeof(Equ_Message_Layers_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Layers_Get, "controller_id", controller_id, EET_T_UINT);
	_descriptors[EQU_INDEX(EQU_MSG_LAYERS_GET)] = edd;
	/* layers get reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Reply_Layers_Get", sizeof(Equ_Reply_Layers_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_VAR_ARRAY(edd, Equ_Reply_Layers_Get, "layers", layers, _ddescriptors[EQU_DATA_LAYER]);
	_descriptors[EQU_INDEX(EQU_REPLY_LAYERS_GET)] = edd;
	/* layer caps get */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Message_Layer_Caps_Get", sizeof(Equ_Message_Layer_Caps_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Layer_Caps_Get, "layer_id", layer_id, EET_T_UINT);
	_descriptors[EQU_INDEX(EQU_MSG_LAYER_CAPS_GET)] = edd;
	/* layer caps get reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Reply_Layer_Caps_Get", sizeof(Equ_Reply_Layer_Caps_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Layer_Caps_Get, "fmt_mask", caps.fmt_mask, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Layer_Caps_Get, "flags_mask", caps.flags_mask, EET_T_UINT);
	_descriptors[EQU_INDEX(EQU_REPLY_LAYER_CAPS_GET)] = edd;
	/* layer status get */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Message_Layer_Status_Get", sizeof(Equ_Message_Layer_Status_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Layer_Status_Get, "layer_id", layer_id, EET_T_UINT);
	_descriptors[EQU_INDEX(EQU_MSG_LAYER_STATUS_GET)] = edd;
	/* layer status get reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Reply_Layer_Status_Get", sizeof(Equ_Reply_Layer_Status_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Layer_Status_Get, "x", status.x, EET_T_INT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Layer_Status_Get, "y", status.y, EET_T_INT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Layer_Status_Get, "w", status.w, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Layer_Status_Get, "h", status.h, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Layer_Status_Get, "fmt", status.fmt, EET_T_UINT);
	_descriptors[EQU_INDEX(EQU_REPLY_LAYER_STATUS_GET)] = edd;
	/* layer surface put */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Message_Surface_Put", sizeof(Equ_Message_Surface_Put));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[EQU_INDEX(EQU_MSG_SURFACE_PUT)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Put, "layer_id", layer_id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Put, "surface_id", surface_id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Put, "dx", dx, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Put, "dy", dy, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Put, "cx", cx, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Put, "cy", cy, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Put, "cw", cw, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Put, "ch", ch, EET_T_UINT);
	/* Pool messages/replies */
	/* Surfaces messages/replies */
	/* surface get */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Message_Surface_Get", sizeof(Equ_Message_Surface_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[EQU_INDEX(EQU_MSG_SURFACE_GET)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Get, "host_id", host_id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Get, "w", w, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Get, "h", h, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Get, "fmt", fmt, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Get, "type", type, EET_T_UINT);
	/* surface get reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Reply_Surface_Get", sizeof(Equ_Reply_Surface_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Surface_Get, "id", id, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Reply_Surface_Get, "sh_id", sh_id, EET_T_STRING);
	_descriptors[EQU_INDEX(EQU_REPLY_SURFACE_GET)] = edd;

	eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc), "Equ_Message_Surface_Delete", sizeof(Equ_Message_Surface_Delete));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[EQU_INDEX(EQU_MSG_SURFACE_DELETE)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Equ_Message_Surface_Delete, "surface_id", surface_id, EET_T_UINT);
}

static void _message_shutdown(void)
{
	int i;

	/* remove all the messages's data descriptors */
	for (i = 0; i < EQU_INDEX(EQU_REPLY_LAST); i++)
		eet_data_descriptor_free(_descriptors[i]);
	for (i = 0; i < EQU_DATAS; i++)
		eet_data_descriptor_free(_ddescriptors[i]);
}

#define ADD(m,r) \
	eix_server_message_add(s, m, _descriptors[EQU_INDEX(m)], r); 		\
	if (r) eix_server_message_add(s, r, _descriptors[EQU_INDEX(r)], 0);	\
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void equ_common_server_setup(Eix_Server *s)
{
	ADD(EQU_MSG_HOSTS_GET, EQU_REPLY_HOSTS_GET);
	ADD(EQU_MSG_CONTROLLERS_GET, EQU_REPLY_CONTROLLERS_GET);
	ADD(EQU_MSG_LAYERS_GET, EQU_REPLY_LAYERS_GET);
	ADD(EQU_MSG_LAYER_CAPS_GET, EQU_REPLY_LAYER_CAPS_GET);
	ADD(EQU_MSG_LAYER_STATUS_GET, EQU_REPLY_LAYER_STATUS_GET);
	ADD(EQU_MSG_SURFACE_GET, EQU_REPLY_SURFACE_GET);
	ADD(EQU_MSG_SURFACE_PUT, 0);
	/* FIXME do something with upload/download */
	ADD(EQU_MSG_SURFACE_UPLOAD, 0);
	ADD(EQU_MSG_SURFACE_DOWNLOAD, 0);
	ADD(EQU_MSG_SURFACE_DELETE, 0);
}

void equ_common_init(void)
{
	eina_init();
	eix_init();
	_message_init();
}

void equ_common_shutdown(void)
{
	eina_init();
	eix_init();
	_message_shutdown();
}

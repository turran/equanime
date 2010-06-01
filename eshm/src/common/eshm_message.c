#include "Eshm.h"
#include "eshm_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eet_Data_Descriptor *_descriptors[ESHM_MSG_NAMES];
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eshm_Message * eshm_message_new(Eshm_Message_Type type)
{
	static int id = 0;
	Eshm_Message *m;

	m = malloc(sizeof(Eshm_Message));
	m->id = id;
	m->type = type;
	/* TODO this will cause an overflow sometime */
	id++;
	return m;
}

void eshm_message_init(void)
{
	Eet_Data_Descriptor *edd;
	Eet_Data_Descriptor_Class eddc;
	/* create all the messages' data descriptors */

	/* segment new */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_New", sizeof(Eshm_Message_Segment_New));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_NEW] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_New, "id", id, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_New, "size", size, EET_T_UINT);
	/* segment new reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Reply_Segment_New", sizeof(Eshm_Reply_Segment_New));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_NEWR] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Reply_Segment_New, "shmid", shmid, EET_T_UINT);
	/* segment get */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_Get", sizeof(Eshm_Message_Segment_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_GET] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Get, "id", id, EET_T_INLINED_STRING);
	/* segment get reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Reply_Segment_Get", sizeof(Eshm_Reply_Segment_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_GETR] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Reply_Segment_Get, "shmid", shmid, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Reply_Segment_Get, "size", size, EET_T_UINT);
	/* segment lock */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_Lock", sizeof(Eshm_Message_Segment_Lock));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_LOCK] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Lock, "id", id, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Lock, "write", write, EET_T_UCHAR);
	/* segment unlock */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_Unlock", sizeof(Eshm_Message_Segment_Unlock));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_UNLOCK] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Unlock, "id", id, EET_T_INLINED_STRING);
	/* segment delete */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_Delete", sizeof(Eshm_Message_Segment_Delete));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[ESHM_MSG_NAME_SEGMENT_DELETE] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Delete, "id", id, EET_T_INLINED_STRING);
}

void eshm_message_shutdown(void)
{
	int i;

	/* remove all the messages's data descriptors */
	for (i = 0; i < ESHM_MSG_NAMES; i++)
		eet_data_descriptor_free(_descriptors[i]);
}

void * eshm_message_encode(Eshm_Message_Name name, const void *data, int *size)
{
	return eet_data_descriptor_encode(_descriptors[name], data, size);
}

void * eshm_message_decode(Eshm_Message_Name name, const void *data, int size)
{
	return eet_data_descriptor_decode(_descriptors[name], data, size);
}

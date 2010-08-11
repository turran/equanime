#include "Eshm.h"
#include "eshm_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define INDEX(n) (n - EIX_REPLY_LAST)
static Eet_Data_Descriptor *_descriptors[INDEX(ESHM_REPLIES)];

static void _message_init(void)
{
	Eet_Data_Descriptor *edd;
	Eet_Data_Descriptor_Class eddc;
	/* create all the messages' data descriptors */

	/* segment new */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_New", sizeof(Eshm_Message_Segment_New));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(ESHM_MSG_SEGMENT_NEW)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_New, "id", id, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_New, "size", size, EET_T_UINT);
	/* segment new reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Reply_Segment_New", sizeof(Eshm_Reply_Segment_New));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(ESHM_REPLY_SEGMENT_NEW)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Reply_Segment_New, "shmid", shmid, EET_T_UINT);
	/* segment get */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_Get", sizeof(Eshm_Message_Segment_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(ESHM_MSG_SEGMENT_GET)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Get, "id", id, EET_T_INLINED_STRING);
	/* segment get reply */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Reply_Segment_Get", sizeof(Eshm_Reply_Segment_Get));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(ESHM_REPLY_SEGMENT_GET)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Reply_Segment_Get, "shmid", shmid, EET_T_UINT);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Reply_Segment_Get, "size", size, EET_T_UINT);
	/* segment lock */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_Lock", sizeof(Eshm_Message_Segment_Lock));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(ESHM_MSG_SEGMENT_LOCK)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Lock, "id", id, EET_T_INLINED_STRING);
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Lock, "write", write, EET_T_UCHAR);
	/* segment unlock */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_Unlock", sizeof(Eshm_Message_Segment_Unlock));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(ESHM_MSG_SEGMENT_UNLOCK)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Unlock, "id", id, EET_T_INLINED_STRING);
	/* segment delete */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eshm_Message_Segment_Delete", sizeof(Eshm_Message_Segment_Delete));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[INDEX(ESHM_MSG_SEGMENT_DELETE)] = edd;
	EET_DATA_DESCRIPTOR_ADD_BASIC(edd, Eshm_Message_Segment_Delete, "id", id, EET_T_INLINED_STRING);
}

void _message_shutdown(void)
{
	int i;

	/* remove all the messages's data descriptors */
	for (i = 0; i < ESHM_REPLIES - EIX_REPLY_LAST; i++)
		eet_data_descriptor_free(_descriptors[i]);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void eshm_common_server_setup(Eix_Server *s)
{
	eix_server_message_add(s, ESHM_MSG_SEGMENT_NEW,
			_descriptors[INDEX(ESHM_MSG_SEGMENT_NEW)],
			ESHM_REPLY_SEGMENT_NEW);
	eix_server_message_add(s, ESHM_REPLY_SEGMENT_NEW,
			_descriptors[INDEX(ESHM_REPLY_SEGMENT_NEW)],
			0);
	eix_server_message_add(s, ESHM_MSG_SEGMENT_GET,
			_descriptors[INDEX(ESHM_MSG_SEGMENT_GET)],
			ESHM_REPLY_SEGMENT_GET);
	eix_server_message_add(s, ESHM_REPLY_SEGMENT_GET,
			_descriptors[INDEX(ESHM_REPLY_SEGMENT_GET)],
			0);
	eix_server_message_add(s, ESHM_MSG_SEGMENT_LOCK,
			_descriptors[INDEX(ESHM_MSG_SEGMENT_LOCK)],
			0);
	eix_server_message_add(s, ESHM_MSG_SEGMENT_UNLOCK,
			_descriptors[INDEX(ESHM_MSG_SEGMENT_UNLOCK)],
			0);
	eix_server_message_add(s, ESHM_MSG_SEGMENT_DELETE,
			_descriptors[INDEX(ESHM_MSG_SEGMENT_DELETE)],
			0);
}

void eshm_common_init(void)
{
	eina_init();
	ecore_init();
	eix_init();
	_message_init();
}
void eshm_common_shutdown(void)
{
	_message_shutdown();
	eix_shutdown();
	ecore_shutdown();
	eina_shutdown();
}


#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Eix.h"
#include "Eet.h"
#include "Ecore.h"
#include "Ecore_Con.h"

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

static Eet_Data_Descriptor *_descriptors[EIX_MSG_NAMES];
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eix_Message * eix_message_new(Eix_Message_Type type)
{
	static int id = 0;
	Eix_Message *m;

	m = malloc(sizeof(Eix_Message));
	m->id = id;
	m->type = type;
	/* TODO this will cause an overflow sometime */
	id++;

	return m;
}

void eix_message_init(void)
{
	Eet_Data_Descriptor *edd;
	Eet_Data_Descriptor_Class eddc;

	/* Core messages */
	/* sync */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eix_Message_Sync", sizeof(Eix_Message_Sync));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[EIX_MSG_NAME_SYNC] = edd;
	/* sync */
	eet_eina_stream_data_descriptor_class_set(&eddc, "Eix_Reply_Sync", sizeof(Eix_Reply_Sync));
	edd = eet_data_descriptor_stream_new(&eddc);
	_descriptors[EIX_MSG_NAME_SYNCR] = edd;
}

void eix_message_shutdown(void)
{
	int i;

	/* remove all the messages's data descriptors */
	for (i = 0; i < EIX_MSG_NAMES; i++)
		eet_data_descriptor_free(_descriptors[i]);
}

void * eix_message_encode(Eix_Message_Name name, const void *data, int *size)
{
	return eet_data_descriptor_encode(_descriptors[name], data, size);
}

void * eix_message_decode(Eix_Message_Name name, const void *data, int size)
{
	return eet_data_descriptor_decode(_descriptors[name], data, size);
}

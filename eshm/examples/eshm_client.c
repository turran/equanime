#include <stdio.h>
#include <string.h>

#include "Eshm.h"

#define S_4M (4 * 1024 * 1024)

int main(int argc, char **argv)
{
	Eshm_Segment *s;
	char *data;

	eshm_init();
	printf("Requesting the segment 'mysegment'\n");
	s = eshm_segment_new("mysegment", S_4M);
	/* if the segment already exists request it */
	if (!s)
	{
		s = eshm_segment_get("mysegment", 0, EINA_FALSE);
		if (!s)
		{
			printf("Segment can't be created or retrieved\n");
		}
	}
	/* request the segment for write */
	if (!eshm_segment_lock(s, EINA_TRUE))
	{
		printf("Couldn't lock, just read\n");
		/* TODO check if the error is for permissions
		 * if so, request the segment for read only
		 */
		eshm_segment_lock(s, EINA_FALSE);
		/* read up some data */
		data = eshm_segment_data_get(s);
		printf("eshm client data is: %s\n", data);
	}
	else
	{
		printf("Segment locked for writing\n");
		/* write up some data */
		data = eshm_segment_data_get(s);
		if (data) strcpy(data, "test data");
	}
	eshm_segment_unlock(s);
	ecore_main_loop_begin();
	eshm_shutdown();

	return 0;
}

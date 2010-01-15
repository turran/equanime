#include "Equanime.h"

void host_cb(void *p, void *data)
{
	Equ_Host **ref = data;
	Equ_Host *h = p;

	*ref = h;
}

void controller_cb(void *p, void *data)
{
	Equ_Controller **ref = data;
	Equ_Controller *c = p;

	*ref = c;
}

void output_cb(void *p, void *data)
{
	Equ_Output **ref = data;
	Equ_Output *o = p;

	if (!strcmp(equ_output_name_get(o), "composite"))
		*ref = o;
}

int main(void)
{
	Equ_Controller *c = NULL;
	Equ_Output *o = NULL;
	Equ_Host *h = NULL;
	Equ_Mode mode;

	equ_init();

	equ_hosts_get(host_cb, &h);
	if (!h)
	{
		printf("No hosts\n");
		return 0;
	}
	/* get the main controller */
	equ_host_controllers_get(h, controller_cb, &c);
	if (!c)
	{
		printf("No controllers\n");
		return 0;
	}
	/* get the composite output */
	equ_controller_outputs_get(c, output_cb, &o);
	if (!o)
	{
		printf("No output\n");
		return 0;
	}
	/* set mode */
	equ_mode_standard_from(&mode, EQU_STANDARD_NTSC);
	//equ_mode_standard_from(&mode, EQU_STANDARD_PAL);
	equ_output_mode_set(o, &mode);
	equ_controller_output_set(c, o);

	equ_shutdown();
	return 0;
}

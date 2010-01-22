#include <stdio.h>
#include "Equanime.h"

Equanime *eq;

/*
 * This application just shows the host hierarchy, showing all controllers,
 * layers, outputs, inputs, etc.
 */
const char *layer_flags[] = {
	"VISIBILITY",
	"POSITION",
	"SIZE",
	"LEVEL",
};

static void _output_desc_dump(Equ_Output *o)
{
	printf("\t- name = %s\n", equ_output_name_get(o));
}

static int _output_cb(Equ_Output *o, void *data)
{
	int *num = data;
	char *ptr;
	int i;

	printf("Output %d\n", *num);
	*num = *num + 1;
	_output_desc_dump(o);

	return 1;
}

static void _layer_desc_dump(const Equ_Layer *l)
{
	int i = 0;
	int flags;

	printf("\t- name = %s\n", equ_layer_name_get(l));
#if 0
	printf("\t- flags = ");
	while (flags)
	{
		if (flags & 1)
		{
			printf("%s ", layer_flags[i]);
		}
		flags = flags >> 1;
		i++;
	}
	printf("\n");
#endif
}

static int _layer_cb(Equ_Layer *l, void *data)
{
	int *num = data;
	char *ptr;
	int i;

	printf("Layer %d\n", *num);
	*num = *num + 1;
	_layer_desc_dump(l);

	return 1;
}

static void _controller_desc_dump(Equ_Controller *c)
{
	printf("\t- name = %s\n", equ_controller_name_get(c));
}

int _controller_cb(Equ_Controller *c, void *data)
{
	int *num = data;
	int num_layer = 0;
	int num_output = 0;

	printf("Controller %d\n", *num++);
	_controller_desc_dump(c);
	equ_controller_layers_get(c, (Equ_Cb)_layer_cb, &num_layer);
	equ_controller_outputs_get(c, (Equ_Cb)_output_cb, &num_output);

	return 1;
}

static void _host_desc_dump(Equ_Host *h)
{
	printf("\t- name = %s\n", equ_host_name_get(eq, h));
}

int _host_cb(Equ_Host *h, void *data)
{
	int num_controller = 0;
	int *num = data;

	printf("Host %d\n", *num++);
	_host_desc_dump(h);
	equ_host_controllers_get(h, (Equ_Cb)_controller_cb, &num_controller);

	return 1;
}

/**
 * List all controllers, layers, inputs and ouputs.
 */
int main(void)
{
	int num_host;

	equ_init();
	eq = equ_new(0xea);
	equ_hosts_get(eq, _host_cb, &num_host);
	equ_shutdown();

	return 0;
}

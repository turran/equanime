#include "Equ_Server.h"

#include "mp2530f_regs.h"

static struct mp2530f_rotator_reg *_regs;

static Eina_Bool _rotator_setup(Equ_Component *c)
{
	Equ_Rotator *r;

	r = (Equ_Rotator *)equ_component_interface_get(c, EQU_COMPONENT_ROTATOR);
	/* TODO check the parameters */
	return EINA_TRUE;
}

static void _rotator_run(Equ_Component *c)
{
	Equ_Rotator *r;

	r = (Equ_Rotator *)equ_component_interface_get(c, EQU_COMPONENT_ROTATOR);
	/* TODO trigger the run register */
}

static Equ_Component_Data _data = {
	.type = EQU_COMPONENT_ROTATOR,
};

static Equ_Component_Backend _backend = {
	.data = {
		{
			&_data
		},
	},
	.setup = _rotator_setup,
	.run = _rotator_run,
};

void mp2530f_rotator_init(void)
{
	/* TODO pass the host */
	/* TODO check that the host is the one we need */
	/* TODO first try to get the registers */
	/* register the component */
	equ_component_register(&_backend);
}

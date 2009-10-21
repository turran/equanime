#include "Equanime.h"
#include "equanime_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
Equ_Mode _modes[] = {
	{
		.std = EQU_STANDARD_NTSC,
	},
	{
		.std = EQU_STANDARD_PAL,

	},
	{
		.std = EQU_STANDARD_480P,

	},
	{
		.std = EQU_STANDARD_720P,

	},
	{
		.std = EQU_STANDARD_1080I,

	},
};

static int _modes_size = 5;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_mode_standard_from(Equ_Mode *m, Equ_Standard std)
{
	const num_modes = sizeof(_modes) / sizeof(Equ_Mode);
	int i;


	/* TODO iterate over the list of modes */
	for (i = 0; i < num_modes; i++)
	{
		if (_modes[i].std == std)
		{
			*m = _modes[i];
			break;
		}
	}
}

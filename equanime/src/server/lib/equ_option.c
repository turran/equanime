#include "Equ_Server.h"
#include <string.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/* false, true
 * 0, 1
 */
static void _bool_get(Equ_Option *o, const char *value,
		Eina_Bool *dst)
{
	if (!strcmp(value, "true"))
		*dst = EINA_TRUE;
	else if (!strcmp(value, "1"))
		*dst = EINA_TRUE;
	else if (!strcmp(value, "false"))
		*dst = EINA_FALSE;
	else if (!strcmp(value, "0"))
		*dst = EINA_FALSE;
	else
		printf("wrong bool!!!\n");
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_option_parse(Equ_Option *option, char *parse,
		Equ_Option_Type val, void *dest)
{
	char *op;
	/* FIXME we could already transform the parse string instead
	 * of parsing for every option
	 */
	op = strtok(parse, ",");
	if (!op) op = parse;
	for (; op; op = strtok(NULL, ","))
	{
		int len;
		char *value;

		/* split the string by '=' */
		value = strchr(op, '=');
		if (!value)
			continue;
		value++;
		len = value - op - 1;
		if (strncmp(op, option->name, len))
			continue;
		switch (val)
		{
			case EQU_OPTION_BOOL:
			_bool_get(option, value, (Eina_Bool *)dest);
			break;

			default:
			break;
		}
	}
}

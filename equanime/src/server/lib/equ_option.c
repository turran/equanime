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

static void _int_get(Equ_Option *o, const char *value,
		int *dst)
{
	*dst = strtol(value, NULL, 10);
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_option_parse(Equ_Option *option, char *parse,
		Equ_Option_Type val, void *dest)
{
	char *tmp;
	char *op;
	/* FIXME we could already transform the parse string instead
	 * of parsing for every option
	 */

	tmp = strdup(parse);
	op = strtok(tmp, ",");
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

			case EQU_OPTION_INT:
			_int_get(option, value, (int *)dest);
			break;

			default:
			break;
		}
	}
	free(tmp);
}

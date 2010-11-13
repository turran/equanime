%{
#include <stdio.h>
#include <string.h>
#include "enesim_parser.h"

#define YYSTYPE char *
 
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 

static Renderer *current = NULL;
static Type current_type = 0;
 
%}

%token UNSIGNED INT COLOR DOUBLE FLOAT IMAGE SURFACE COORD PATH STRING
%token WORD OBRACE EBRACE SEMICOLON EQUAL QUOTE COLON

%%

renderer
	: WORD 
	{
		current = malloc(sizeof(Renderer));
		current->parent = NULL;
		current->name = strdup($1);
		current->properties = NULL;
		renderers = eina_list_append(renderers, current);
	}
	renderer_inheritance OBRACE declaration_list EBRACE SEMICOLON
	;

renderer_inheritance
	:
	| COLON WORD
	{
		current->parent = strdup($2);
	}
	;


type_specifier
	: UNSIGNED INT
	{
		current_type = PROPERTY_UNSIGNED_INT;
	}
	| INT
	{
		current_type = PROPERTY_INT;
	}
	| COLOR
	{
		current_type = PROPERTY_COLOR;
	}
	| DOUBLE
	{
		current_type = PROPERTY_DOUBLE;
	}
	| FLOAT
	{
		current_type = PROPERTY_FLOAT;
	}
	| COORD
	{
		current_type = PROPERTY_COORD;
	}
	| SURFACE
	{
		current_type = PROPERTY_SURFACE;
	}
	| PATH
	{
		current_type = PROPERTY_PATH;
	}
	| STRING
	{
		current_type = PROPERTY_STRING;
	}
	;

declaration
	: type_specifier WORD SEMICOLON
	{
		Property * property;

		property = malloc(sizeof(Property));
		property->name = strdup($2);
		property->type = current_type;
		current->properties = eina_list_append(current->properties, property);
	}
	;

declaration_list
	:
	| declaration declaration_list
	;
%%

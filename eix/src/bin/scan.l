%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
(\"[^\"]*\")*       {{
                        char *tmp = NULL;
                        int i = 0;

                        yylval = (char *)calloc(yyleng - 1, sizeof(char));

                        for(i = 0; i < yyleng; i++) {
                            if (yytext[i] == '"') {
                                if (tmp != NULL) {
                                    yytext[i] = '\0';
                                    strcat(yylval, tmp);
                                    tmp = NULL;
                                }
                                continue;
                            }

                            if (tmp == NULL)
                                tmp = yytext + i;
                        }
                        if (tmp != NULL)
                            strcat(yylval, tmp);

			    return STRING;
                    }}                           
unsigned		return UNSIGNED;
int			return INT;
color			return COLOR;
double			return DOUBLE;
coord			return COORD;
float			return FLOAT;
surface			return SURFACE;
path			return PATH;
string			return STRING;
[a-zA-Z0-9_]*            yylval = strdup(yytext); return WORD;
\=                      return EQUAL;
\"                      return QUOTE;
\{                      return OBRACE;
\}                      return EBRACE;
;                       return SEMICOLON;
:                       return COLON;
\n                      /* ignore end of line */
[ \t]+                  /* ignore whitespace */;
%%

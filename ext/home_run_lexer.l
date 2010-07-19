%{
#include "home_run_parser.tab.h"
int rhrd__yylex(void);
#include "home_run_parser.tab.c"
%}

%option nounput
%option noyywrap

%%

[0-9][0-9][0-9][0-9]  rhrd__yylval=atoi(rhrd__yytext); return DIGIT4;
[0-9][0-9]            rhrd__yylval=atoi(rhrd__yytext); return DIGIT2;
-                     return DASH;
\/                    return SLASH;
.                     return OTHER;
%%


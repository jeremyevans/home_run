%{
#include "home_run_parser.tab.h"
int rhrd__yylex(void);
#include "home_run_parser.tab.c"

int rhrd__month_num(char * str) {
  if(strcasecmp(str, "nov") == 0 || strcasecmp(str, "november") == 0) {
    return 11;
  }

  return 0;
}
%}

%option nounput
%option noyywrap

%s NEGDIGIT

MONTH january|february|march|april|may|june|july|august|september|october|november|december
ABBR_MONTH jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec
DAY sunday|monday|tuesday|wednesday|thursday|friday|saturday
ABBR_DAY sun|mon|tue|wed|thu|fri|sat

%%

[0-9]{6}              BEGIN(INITIAL); rhrd__yylval=atol(rhrd__yytext); return DIGIT6;
(^-)?[0-9]{4}         BEGIN(INITIAL); rhrd__yylval=atol(rhrd__yytext); return DIGIT4;
<NEGDIGIT>-[0-9]{4}   BEGIN(INITIAL); rhrd__yylval=atol(rhrd__yytext); return DIGIT4;
[0-9]{2}              BEGIN(INITIAL); rhrd__yylval=atol(rhrd__yytext); return DIGIT2;
[0-9]{1}(th|st|nd|rd) BEGIN(INITIAL); rhrd__yytext[1] = '\0'; rhrd__yylval=atol(rhrd__yytext); return DAY;
[0-9]{2}(th|st|nd|rd) BEGIN(INITIAL); rhrd__yytext[2] = '\0'; rhrd__yylval=atol(rhrd__yytext); return DAY;
-                     BEGIN(NEGDIGIT); return DASH;
\/                    BEGIN(NEGDIGIT); return SLASH;
[ ]                   BEGIN(NEGDIGIT); return SPACE;
\.                    BEGIN(NEGDIGIT); return PERIOD;
{MONTH}               BEGIN(INITIAL); rhrd__yylval=rhrd__month_num(rhrd__yytext); return MONTH;
.                     return OTHER;

%%


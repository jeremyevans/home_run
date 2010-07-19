%{
#include <ruby.h>

extern VALUE rhrd_parsed_date_hash;

void rhrd__yyerror(const char *str) {
}

%}

%token DIGIT4 DIGIT2 DASH SLASH OTHER

%%
iso8601_date:
  DIGIT4 DASH DIGIT2 DASH DIGIT2
  {
    rb_hash_aset(rhrd_parsed_date_hash, ID2SYM(rb_intern("year")), INT2NUM($1));
    rb_hash_aset(rhrd_parsed_date_hash, ID2SYM(rb_intern("mon")), INT2NUM($3));
    rb_hash_aset(rhrd_parsed_date_hash, ID2SYM(rb_intern("mday")), INT2NUM($5));
    return 0;
  }
  ;
%%

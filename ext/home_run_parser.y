%{
#include <ruby.h>

extern VALUE rhrd_parsed_date_hash;

void rhrd__yyerror(const char *str) {
}

void rhrd__parse_set_ymd(long year, long month, long day) {
  rb_hash_aset(rhrd_parsed_date_hash, ID2SYM(rb_intern("year")), INT2NUM(year));
  rb_hash_aset(rhrd_parsed_date_hash, ID2SYM(rb_intern("mon")), INT2NUM(month));
  rb_hash_aset(rhrd_parsed_date_hash, ID2SYM(rb_intern("mday")), INT2NUM(day));
}
%}

%token DIGIT6 DIGIT4 DIGIT2 MONTH DAY DASH SLASH PERIOD SPACE OTHER

%%
date:
  iso_date_yyyymmdd
  | iso_date_yymmdd
  | us_date_yyyypmmpdd
  | us_date_yyyysmmsdd
  | us_date_mmpddpyyyy
  | us_date_mmsddsyyyy
  | us_date_mmpddpyy
  | us_date_mmsddsyy
  | eu_date_ddmmyy
  | digit_mmddyy
  | daymonthyear
  | monthdayyear
  | yearmonthday
  ;

iso_date_yyyymmdd:
  DIGIT4 DASH DIGIT2 DASH DIGIT2
  {
    rhrd__parse_set_ymd($1, $3, $5);
    return 0;
  }
  ;

iso_date_yymmdd:
  DIGIT2 DASH DIGIT2 DASH DIGIT2
  {
    rhrd__parse_set_ymd(2000 + $1, $3, $5);
    return 0;
  }
  ;

us_date_yyyypmmpdd:
  DIGIT4 PERIOD DIGIT2 PERIOD DIGIT2
  {
    rhrd__parse_set_ymd($1, $3, $5);
    return 0;
  }
  ;

us_date_yyyysmmsdd:
  DIGIT4 SLASH DIGIT2 SLASH DIGIT2
  {
    rhrd__parse_set_ymd($1, $3, $5);
    return 0;
  }
  ;

us_date_mmpddpyyyy:
  DIGIT2 PERIOD DIGIT2 PERIOD DIGIT4
  {
    rhrd__parse_set_ymd($5, $1, $3);
    return 0;
  }
  ;

us_date_mmsddsyyyy:
  DIGIT2 SLASH DIGIT2 SLASH DIGIT4
  {
    rhrd__parse_set_ymd($5, $1, $3);
    return 0;
  }
  ;

us_date_mmpddpyy:
  DIGIT2 PERIOD DIGIT2 PERIOD DIGIT2
  {
    rhrd__parse_set_ymd(2000 + $5, $1, $3);
    return 0;
  }
  ;

us_date_mmsddsyy:
  DIGIT2 SLASH DIGIT2 SLASH DIGIT2
  {
    rhrd__parse_set_ymd(2000 + $5, $1, $3);
    return 0;
  }
  ;

eu_date_ddmmyy:
  DIGIT2 DASH DIGIT2 DASH DIGIT4
  {
    rhrd__parse_set_ymd($5, $3, $1);
    return 0;
  }
  ;

digit_mmddyy:
  DIGIT6
  {
    rhrd__parse_set_ymd(2000 + $1%100, $1/10000, $1%10000 - $1%100);
    return 0;
  }
  ;

daymonthyear:
  DAY SPACE MONTH SPACE DIGIT4 
  {
    rhrd__parse_set_ymd($5, $3, $1);
    return 0;
  }
  ;

monthdayyear:
  MONTH SPACE DAY SPACE DIGIT4 
  {
    rhrd__parse_set_ymd($5, $1, $3);
    return 0;
  }
  ;

yearmonthday:
  DIGIT4 SPACE MONTH SPACE DAY
  {
    rhrd__parse_set_ymd($1, $3, $5);
    return 0;
  }
  ;

%%

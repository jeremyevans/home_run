
#define RHRR_YEAR_SET 0x1
#define RHRR_MONTH_SET 0x1 << 1
#define RHRR_DAY_SET 0x1 << 2
#define RHRR_YDAY_SET 0x1 << 3
#define RHRR_HOUR_SET 0x1 << 4
#define RHRR_MINUTE_SET 0x1 << 5
#define RHRR_SECOND_SET 0x1 << 6

long rhrd__month_num(char * str) {
  int i;

  for(i = 1; i < 13; i++) {
    if(strncasecmp(str, rhrd__abbr_month_names[i], 3) == 0) {
      return i;
    }
  }

  return 0;
}

%%{
  machine home_run_parser;

  action tag_iso_year4 { t_iso_year4 = p; }
  action tag_iso_month { t_iso_month = p; }
  action tag_iso_day { t_iso_day = p; }
  action tag_abbr_month { t_abbr_month = p; }
  action tag_us_day { t_us_day = p; }
  action tag_us_year { t_us_year = p; }

  action set_iso_date { 
    year = atol(t_iso_year4);
    month = atol(t_iso_month);
    day = atol(t_iso_day);
    state |= RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
  }
  action set_bc { 
    bc = 1;
  }
  action set_us_date { 
    if (t_us_year) {
      year = atol(t_us_year);
      state |= RHRR_YEAR_SET;
    }
    month = rhrd__month_num(t_abbr_month);
    day = atol(t_us_day);
    state |= RHRR_MONTH_SET | RHRR_DAY_SET;
  }

  abbr_month = (/jan/i | /feb/i | /mar/i | /apr/i | /may/i | /jun/i | /jul/i | /aug/i | /sep/i | /oct/i| /nov/i | /dec/i) >tag_abbr_month;
  ws = [ \t\n\r];
  bc = (/bc/i | /b\.c\./i | /bce/i | /b\.c\.e\./i) %set_bc;
  ad = (/ce/i | /ad/i | /c\.e\./i | /a\.d\./i);
  bc_ad = bc | ad;

  iso_year4 = ('-'? . digit{4}) >tag_iso_year4;
  iso_month = (('0' . [1-9]) | '1' . [0-2]) >tag_iso_month;
  iso_day = (([0-2] . [1-9]) | ([123] . '0') | '31') >tag_iso_day;
  iso_date = (iso_year4 . '-' . iso_month . '-' . iso_day) %set_iso_date;

  us_skip = [^\-0-9 \t\v\f\n\r']*;
  us_day = (([0-2]? . [1-9]) | ([123] . '0') | '31') >tag_us_day;
  us_year = ('-'? . digit+) >tag_us_year;
  us_date = (abbr_month . us_skip . space* . us_day . us_skip . (space* . bc_ad? . space* . us_year)?) %set_us_date;
  
  main := iso_date | us_date;
  write data;
}%%

VALUE rhrd__parse(char * p, long len) {
  VALUE hash;
  long state = 0;
  int cs = 0;
  int bc = 0;
  long year = 0;
  long month = 0;
  long day = 0;
  long yday = 0;
  long hour = 0;
  long minute = 0;
  long second = 0;
  long offset = 0;
  char * zone = NULL;
  char * t_iso_year4 = NULL;
  char * t_iso_month = NULL;
  char * t_iso_day = NULL;
  char * t_abbr_month = NULL;
  char * t_us_day = NULL;
  char * t_us_year = NULL;
  char * eof;
  char * pe;
  pe = p + len;
  eof = pe;

  %%write init;
  %%write exec;

  hash = rb_hash_new();
  if(state & RHRR_YEAR_SET) {
    rb_hash_aset(hash, ID2SYM(rb_intern("year")), INT2NUM(bc ? -(year - 1) : year));
  } 
  if(state & RHRR_MONTH_SET) {
    rb_hash_aset(hash, ID2SYM(rb_intern("mon")), INT2NUM(month));
  } 
  if(state & RHRR_DAY_SET) {
    rb_hash_aset(hash, ID2SYM(rb_intern("mday")), INT2NUM(day));
  } 
  return hash;
}

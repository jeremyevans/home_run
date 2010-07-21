
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

  return atol(str);
}

%%{
  machine home_run_parser;

  action tag_le_year { t_le_year = p; }
  action tag_le_month { t_le_month = p; }
  action tag_le_day { t_le_day = p; }
  action tag_be_year { t_be_year = p; }
  action tag_be_month { t_be_month = p; }
  action tag_be_day { t_be_day = p; }
  action tag_me_year { t_me_year = p; }
  action tag_me_month { t_me_month = p; }
  action tag_me_day { t_me_day = p; }
  action set_bc { bc = 1; }

  action set_le_date { 
    if (t_le_year) {
      year = atol(t_le_year);
      state |= RHRR_YEAR_SET;
    }
    month = rhrd__month_num(t_le_month);
    day = atol(t_le_day);
    state |= RHRR_MONTH_SET | RHRR_DAY_SET;
  }

  action set_be_date { 
    year = atol(t_be_year);
    month = rhrd__month_num(t_be_month);
    state |= RHRR_YEAR_SET | RHRR_MONTH_SET;
    if (t_be_day) {
      day = atol(t_be_day);
      state |= RHRR_DAY_SET;
    }
  }

  action set_me_date { 
    if (t_me_year) {
      year = atol(t_me_year);
      state |= RHRR_YEAR_SET;
    }
    month = rhrd__month_num(t_me_month);
    day = atol(t_me_day);
    state |= RHRR_MONTH_SET | RHRR_DAY_SET;
  }

  abbr_month = (/jan/i | /feb/i | /mar/i | /apr/i | /may/i | /jun/i | /jul/i | /aug/i | /sep/i | /oct/i| /nov/i | /dec/i);
  full_month = (/january/i | /february/i | /march/i | /april/i | /may/i | /june/i | /july/i | /august/i | /september/i | /october/i| /november/i | /december/i); 
  month_name = (abbr_month | full_month);
  month_num = (('0'? . [1-9]) | '1' . [0-2]); 
  month = (month_num | month_name);

  abbr_day = (/sun/i | /mon/i | /tue/i | /wed/i | /thu/i | /fri/i | /sat/i);
  full_day = (/sunday/i | /monday/i | /tuesday/i | /wednesday/i | /thursday/i | /friday/i | /saturday/i);

  bc = (/bc/i | /b\.c\./i | /bce/i | /b\.c\.e\./i) %set_bc;
  ad = (/ce/i | /ad/i | /c\.e\./i | /a\.d\./i);
  bc_ad = bc | ad;
  day = ((([0-2]? . [1-9]) | ([123] . '0') | '31')) . ('st' | 'nd' | 'rd' | 'th')?;
  year = ('-'? . digit{4}) . (space* . bc_ad)?;

  le_sep = [ \-/.];
  le_day = day >tag_le_day;
  le_month = month >tag_le_month;
  le_year = year >tag_le_year;
  le_date = (le_day . le_sep . le_month . (le_sep . le_year)?) %set_le_date;

  be_sep = ([ \-] | '. ')?;
  be_day = day >tag_be_day;
  be_month = month >tag_be_month;
  be_year = ('-'? . digit{4}) > tag_be_year;
  be_date = (be_year . be_sep . be_month . (be_sep . be_day)?) %set_be_date;

  me_sep = [.,/\-]? . space*;
  me_day = day >tag_me_day;
  me_month = month >tag_me_month;
  me_year = bc_ad? . space* . (year >tag_me_year);
  me_date = (me_month . me_sep . me_day . (me_sep . me_year)?) %set_me_date;

  date = (le_date | be_date | me_date);
  opt_day = (abbr_day | full_day)? . space*;
  
  main := space* . opt_day . (date | abbr_day | full_day) . space* . opt_day;
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

  char * t_le_year = NULL;
  char * t_le_month = NULL;
  char * t_le_day = NULL;

  char * t_be_year = NULL;
  char * t_be_month = NULL;
  char * t_be_day = NULL;

  char * t_me_year = NULL;
  char * t_me_month = NULL;
  char * t_me_day = NULL;

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

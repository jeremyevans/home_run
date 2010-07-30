
#define RHRR_LE_DATE_PRIORITY 40
#define RHRR_BE_DATE_PRIORITY 60
#define RHRR_ME_DATE_PRIORITY 50
#define RHRR_MY_DATE_PRIORITY 25
#define RHRR_NUM_DATE_PRIORITY 5
#define RHRR_MO_DATE_PRIORITY 5
#define RHRR_DO_DATE_PRIORITY 5

long rhrd__month_num(char * str) {
  int i;

  for(i = 1; i < 13; i++) {
    if(strncasecmp(str, rhrd__abbr_month_names[i], 3) == 0) {
      return i;
    }
  }

  return atol(str);
}

long rhrd__weekday_num(char * str) {
  int i;

  for(i = 0; i < 6; i++) {
    if(strncasecmp(str, rhrd__abbr_day_names[i], 3) == 0) {
      return i;
    }
  }

  return 7;
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
  action tag_my_year { t_my_year = p; }
  action tag_my_month { t_my_month = p; }
  action tag_num_date { t_num_date = p; }
  action tag_month_only { t_month_only = p; }
  action tag_day_only { t_day_only = p; }
  action tag_weekday { t_weekday = p; }

  action le_year_error { t_le_year = NULL; }
  action be_day_error { t_be_day = NULL; }
  action me_year_error { t_me_year = NULL; }

  action set_le_inc_pri { le_inc_pri = 1; }
  action set_be_dec_pri { be_dec_pri = 1; }

  action set_le_year2 { le_year2 = 1; }
  action unset_le_year2 { le_year2 = 0; }
  action set_be_year2 { be_year2 = 1; }
  action unset_be_year2 { be_year2 = 0; }
  action set_me_year2 { me_year2 = 1; }
  action unset_me_year2 { me_year2 = 0; }

  action set_bc { bc = 1; }

  action set_le_date { 
    if (date_priority <= RHRR_LE_DATE_PRIORITY) {
#ifdef DEBUG
      printf("Setting little endian (day month year) date\n");
#endif
      state &= ~(RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET);
      date_priority = RHRR_LE_DATE_PRIORITY;
      if (t_le_year) {
        year = atol(t_le_year);
        if (le_year2) {
          year += year < 70 ? 2000 : 1900;
        } else if (le_inc_pri) {
          date_priority += 11;
        }
        state |= RHRR_YEAR_SET;
      } else {
        date_priority -= 36;
      }
      month = rhrd__month_num(t_le_month);
      day = atol(t_le_day);
      state |= RHRR_MONTH_SET | RHRR_DAY_SET;
    }
  }

  action set_be_date { 
    if (date_priority <= RHRR_BE_DATE_PRIORITY) {
#ifdef DEBUG
      printf("Setting big endian (year month day) date\n");
#endif
      state &= ~(RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET);
      date_priority = RHRR_BE_DATE_PRIORITY;
      year = atol(t_be_year);
      if (be_year2) {
        year += year < 70 ? 2000 : 1900;
        if (be_dec_pri) {
        date_priority -= 11;
        }
      } 
      month = rhrd__month_num(t_be_month);
      state |= RHRR_YEAR_SET | RHRR_MONTH_SET;
      if (t_be_day) {
        day = atol(t_be_day);
        state |= RHRR_DAY_SET;
      } else {
        date_priority -= 36;
      }
    }
  }

  action set_me_date { 
    if (date_priority <= RHRR_ME_DATE_PRIORITY) {
#ifdef DEBUG
      printf("Setting middle endian (month day year) date\n");
#endif
      state &= ~(RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET);
      date_priority = RHRR_ME_DATE_PRIORITY;
      if (t_me_year) {
        year = atol(t_me_year);
        if (me_year2) {
          year += year < 70 ? 2000 : 1900;
        }
        state |= RHRR_YEAR_SET;
      } else {
        date_priority -= 36;
      }
      month = rhrd__month_num(t_me_month);
      day = atol(t_me_day);
      state |= RHRR_MONTH_SET | RHRR_DAY_SET;
    }
  }

  action set_my_date { 
    if (date_priority <= RHRR_MY_DATE_PRIORITY) {
#ifdef DEBUG
      printf("Setting month year date\n");
#endif
      state &= ~(RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET);
      date_priority = RHRR_MY_DATE_PRIORITY;
      year = atol(t_my_year);
      month = rhrd__month_num(t_my_month);
      state |= RHRR_YEAR_SET | RHRR_MONTH_SET;
    }
  }

  action set_num_date { 
    if (date_priority <= RHRR_NUM_DATE_PRIORITY) {
#ifdef DEBUG
      printf("Setting num date (for %ld characters)\n", p - t_num_date);
#endif
      switch(p - t_num_date) {
        case 2:
          sscanf(t_num_date, "%2ld", &day);
          state |= RHRR_DAY_SET;
          break;
        case 3:
          sscanf(t_num_date, "%3ld", &yday);
          state |= RHRR_YDAY_SET;
          break;
        case 4:
          sscanf(t_num_date, "%2ld%2ld", &month, &day);
          state |= RHRR_MONTH_SET | RHRR_DAY_SET;
          break;
        case 5:
          sscanf(t_num_date, "%2ld%3ld", &year, &yday);
          year += year < 70 ? 2000 : 1900;
          state |= RHRR_YEAR_SET | RHRR_YDAY_SET;
          break;
        case 6:
          sscanf(t_num_date, "%2ld%2ld%2ld", &year, &month, &day);
          year += year < 70 ? 2000 : 1900;
          state |= RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
          break;
        case 7:
          sscanf(t_num_date, "%4ld%3ld", &year, &yday);
          state |= RHRR_YEAR_SET | RHRR_YDAY_SET;
          break;
        case 8:
          sscanf(t_num_date, "%4ld%2ld%2ld", &year, &month, &day);
          state |= RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
          break;
      }
      date_priority = RHRR_NUM_DATE_PRIORITY;
    }
  }

  action set_month_only {
    if (date_priority <= RHRR_MO_DATE_PRIORITY) {
#ifdef DEBUG
      printf("Setting month only date\n");
#endif
      month = rhrd__month_num(t_month_only);
      state |= RHRR_MONTH_SET;
      date_priority = RHRR_MO_DATE_PRIORITY;
    }
  }

  action set_day_only {
    if (date_priority <= RHRR_DO_DATE_PRIORITY) {
#ifdef DEBUG
      printf("Setting day only date\n");
#endif
      day = atol(t_day_only);
      state |= RHRR_DAY_SET;
      date_priority = RHRR_DO_DATE_PRIORITY;
    }
  }

  action set_weekday {
#ifdef DEBUG
    printf("Setting weekday\n");
#endif
    wday = rhrd__weekday_num(t_weekday);
    state |= RHRR_WDAY_SET;
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
  le_year = bc_ad? . space* . ('-'? . (digit{2} %set_le_year2) :>> (digit{2} %unset_le_year2)? . (space* . bc_ad %unset_le_year2)?) >tag_le_year $^le_year_error;
  le_date = (le_day . ([\-] %set_le_inc_pri | [ /.]) . le_month . (le_sep . le_year)?) %set_le_date;

  be_sep = ([ /\-.] | '. ')?;
  be_day = day >tag_be_day $^be_day_error;
  be_month = month >tag_be_month;
  be_year = ('-'? . (digit{2} %set_be_year2) :>> (digit{2} %unset_be_year2)?) >tag_be_year;
  be_date = (be_year . ([/.] % set_be_dec_pri | [ \-] | '. ') . be_month . (be_sep . be_day)?) %set_be_date;

  me_sep = [.,/\- ] . space*;
  me_day = day >tag_me_day;
  me_month = month >tag_me_month;
  me_year = bc_ad? . space* . (('-'? . (digit{2} %set_me_year2) :>> (digit{2} %unset_me_year2)? :>> (space* . bc_ad %unset_me_year2)?) >tag_me_year $^me_year_error);
  me_date = (me_month . me_sep . me_day . (me_sep . me_year)?) %set_me_date;

  my_date = (month_name >tag_my_month . [ /.] . ('-'? . digit{4}) >tag_my_year) %set_my_date;

  num_date = digit{2,14} >tag_num_date %set_num_date;
  month_only_date = month_name >tag_month_only %set_month_only;
  day_only_date = (((([0-2]? . [1-9]) | ([123] . '0') | '31')) . ('st' | 'nd' | 'rd' | 'th')) >tag_day_only %set_day_only;

  date = (le_date | be_date | me_date | my_date | num_date | month_only_date | day_only_date);
  opt_day = ((abbr_day | full_day) >tag_weekday %set_weekday)? . space*;
  
  main := space* . opt_day . date . space* . opt_day;
  write data;
}%%

VALUE rhrd__parse(char * p, long len) {
  VALUE hash;
  long state = 0;
  int date_priority = 0;
  int cs = 0;

  int bc = 0;
  long year = 0;
  long month = 0;
  long day = 0;
  long yday = 0;
  long wday = 0;

  long hour = 0;
  long minute = 0;
  long second = 0;
  long offset = 0;
  char * zone = NULL;

  char * t_le_year = NULL;
  char * t_le_month = NULL;
  char * t_le_day = NULL;
  int le_year2 = 0;
  int le_inc_pri = 0;

  char * t_be_year = NULL;
  char * t_be_month = NULL;
  char * t_be_day = NULL;
  int be_year2 = 0;
  int be_dec_pri = 0;

  char * t_me_year = NULL;
  char * t_me_month = NULL;
  char * t_me_day = NULL;
  int me_year2 = 0;

  char * t_my_year = NULL;
  char * t_my_month = NULL;

  char * t_num_date = NULL;
  char * t_month_only = NULL;
  char * t_day_only = NULL;
  char * t_weekday = NULL;

  char * eof;
  char * pe;
  pe = p + len;
  eof = pe;

  %%write init;
  %%write exec;

  hash = rb_hash_new();
  if(state & RHRR_YEAR_SET) {
    rb_hash_aset(hash, rhrd_sym_year, INT2NUM(bc ? -(year - 1) : year));
  } 
  if(state & RHRR_MONTH_SET) {
    rb_hash_aset(hash, rhrd_sym_mon, INT2NUM(month));
  } 
  if(state & RHRR_DAY_SET) {
    rb_hash_aset(hash, rhrd_sym_mday, INT2NUM(day));
  } 
  if(state & RHRR_YDAY_SET) {
    rb_hash_aset(hash, rhrd_sym_yday, INT2NUM(yday));
  } 
  if(state & RHRR_WDAY_SET) {
    rb_hash_aset(hash, rhrd_sym_wday, INT2NUM(wday));
  } 
  return hash;
}

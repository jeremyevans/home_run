
#define RHRR_ISO_PARSER 0x1

#define RHRR_ISO_TIME_SET 0x1
#define RHRR_ISO_ZONE_SET 0x2

%%{
  machine date_parser;

  action tag_iso_year { t_iso_year = p; }
  action tag_iso_month { t_iso_month = p; }
  action tag_iso_day { t_iso_day = p; }
  action tag_iso_hour { t_iso_hour = p; }
  action tag_iso_minute { t_iso_minute = p; }
  action tag_iso_second { t_iso_second = p; }
  action tag_iso_zone { t_iso_zone = p; }

  action set_iso_time { iso_state |= RHRR_ISO_TIME_SET; }
  action set_iso_zone { iso_state |= RHRR_ISO_ZONE_SET; }
  action set_parser_iso { parsers |= RHRR_ISO_PARSER; }

  iso_year = ('-'? . digit{4}) >tag_iso_year;
  iso_month = ('0' . [1-9] | '1' . [0-2]) >tag_iso_month;
  iso_day = ('0' . [1-9] | [12] . [0-9] | '3' . [01]) >tag_iso_day;
  iso_hour = ([0-1] . [0-9] | '2' . [0-4]) >tag_iso_hour;
  iso_minute = ([0-5] . [0-9]) >tag_iso_minute;
  iso_second = ([0-5] . [0-9]) >tag_iso_second;
  iso_zone = ([+\-] . digit{2} . ':' . digit{2}) > tag_iso_zone;
  iso_date = (iso_year . '-' . iso_month . '-' . iso_day);
  iso_time = (iso_hour . ':' . iso_minute . ':' . iso_second . (iso_zone %set_iso_zone)?) %set_iso_time;
  iso_date_time = (iso_date . ([Tt ] . iso_time)? . space*) %/set_parser_iso;

  date_time = (iso_date_time);
  
  main := (space* . date_time);
  write data;
}%%

VALUE rhrd__ragel_parse(char * p, long len) {
  VALUE hash;
  long state = 0;
  long parsers = 0;

  long year = 0;
  long month = 0;
  long day = 0;

  long hour = 0;
  long minute = 0;
  long second = 0;

  VALUE rzone;
  char * zone = NULL;
  long zone_len = 0;
  long offset = 0;

  long iso_state = 0;
  char * t_iso_year = NULL;
  char * t_iso_month = NULL;
  char * t_iso_day = NULL;
  char * t_iso_hour= NULL;
  char * t_iso_minute = NULL;
  char * t_iso_second = NULL;
  char * t_iso_zone = NULL;

  int cs = 0;
  char * eof;
  char * pe;
  pe = p + len;
  eof = pe;

  %%write init;
  %%write exec;

  switch(parsers) {
    case RHRR_ISO_PARSER:
      year = atol(t_iso_year);
      month = atol(t_iso_month);
      day = atol(t_iso_day);
      state |= RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
      if (iso_state & RHRR_ISO_TIME_SET) {
        hour = atol(t_iso_hour);
        minute = atol(t_iso_minute);
        second = atol(t_iso_second);
        state |= RHRR_HOUR_SET | RHRR_MINUTE_SET | RHRR_SECOND_SET;
        if (iso_state & RHRR_ISO_ZONE_SET) {
          zone = t_iso_zone;
          offset = atol(zone) * 3600 + atol(zone + 4) * 60;
          state |= RHRR_ZONE_SET | RHRR_OFFSET_SET;
        }
      }
    break;
  }

  if (!state) {
    return Qnil;
  }

  hash = rb_hash_new();
  if(state & RHRR_YEAR_SET) {
    rb_hash_aset(hash, rhrd_sym_year, INT2NUM(year));
  } 
  if(state & RHRR_MONTH_SET) {
    rb_hash_aset(hash, rhrd_sym_mon, INT2NUM(month));
  } 
  if(state & RHRR_DAY_SET) {
    rb_hash_aset(hash, rhrd_sym_mday, INT2NUM(day));
  } 
  if(state & RHRR_HOUR_SET) {
    rb_hash_aset(hash, rhrd_sym_hour, INT2NUM(hour));
  } 
  if(state & RHRR_MINUTE_SET) {
    rb_hash_aset(hash, rhrd_sym_min, INT2NUM(minute));
  } 
  if(state & RHRR_SECOND_SET) {
    rb_hash_aset(hash, rhrd_sym_sec, INT2NUM(second));
  } 
  if(state & RHRR_ZONE_SET) {
    rzone = rb_str_new(zone, zone_len);
    rb_hash_aset(hash, rhrd_sym_zone, rzone);
    if (state & RHRR_OFFSET_SET) {
        rb_hash_aset(hash, rhrd_sym_offset, INT2NUM(offset));
    } else {
      rzone = rhrd_s_zone_to_diff(rzone, rzone);
      if(RTEST(rzone)) {
        rb_hash_aset(hash, rhrd_sym_offset, rzone);
      }
    }
  } 

  return hash;
}

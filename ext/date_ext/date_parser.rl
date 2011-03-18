#include "date_ext.h"

#ifdef RHR_ENCODING
extern int rhrd_encoding_index;
#endif

#define RHRR_ISO_PARSER 0x1
#define RHRR_RFC_PARSER 0x2
#define RHRR_CLF_PARSER 0x4

#define RHRR_ISO_TIME_SET 0x1
#define RHRR_ISO_ZONE_SET 0x2
#define RHRR_ISO_SEC_FRACTION_SET 0x4

#define RHRR_RFC_TIME_SET 0x1
#define RHRR_RFC_ZONE_SET 0x2
#define RHRR_RFC_ZONE_NUM_SET 0x4
#define RHRR_RFC_ZONE_NAME_SET 0x8

#define RHRR_CLF_TIME_SET 0x1
#define RHRR_CLF_ZONE_SET 0x2

#define RHRR_BAD_OFFSET 86400

extern const char * rhrd__abbr_month_names[];
extern const char * rhrd__abbr_day_names[];

extern VALUE rhrd_sym_hour;
extern VALUE rhrd_sym_mday;
extern VALUE rhrd_sym_min;
extern VALUE rhrd_sym_mon;
extern VALUE rhrd_sym_offset;
extern VALUE rhrd_sym_sec;
extern VALUE rhrd_sym_sec_fraction;
extern VALUE rhrd_sym_wday;
extern VALUE rhrd_sym_year;
extern VALUE rhrd_sym_zone;

const char * rhrd__rfc_zone_names[] = {"UT", "GMT", "EST", "EDT", "CST", "CDT", "MST", "MDT", "PST", "PDT",
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "K", "L", "M",
  "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
long rhrd__rfc_zone_offsets[] = {0, 0, -18000, -14400, -21600, -18000, -25200, -21600, -28800, -25200,
  3600, 7200, 10800, 14400, 18000, 21600, 25200, 28800, 32400, 36000, 39600, 43200,
  -3600, -7200, -10800, -14400, -18000, -21600, -25200, -28800, -32400, -36000, -39600, -43200, 0};

long rhrd__rfc_zone_offset(char * str) {
  int i;

  for(i = 0; i < 36; i++) {
    if(strcasecmp(rhrd__rfc_zone_names[i], str) == 0) {
      return rhrd__rfc_zone_offsets[i];
    }
  }

  return RHRR_BAD_OFFSET;
}

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

  for(i = 0; i < 7; i++) {
    if(strncasecmp(str, rhrd__abbr_day_names[i], 3) == 0) {
      return i;
    }
  }

  return 7;
}

%%{
  machine date_parser;

  # Shared

  abbr_month_name = /jan/i | /feb/i | /mar/i | /apr/i | /may/i | /jun/i |
                    /jul/i | /aug/i | /sep/i | /oct/i | /nov/i | /dec/i;
  # ISO 8601

  action tag_iso_year { t_iso_year = p; }
  action tag_iso_month { t_iso_month = p; }
  action tag_iso_day { t_iso_day = p; }
  action tag_iso_hour { t_iso_hour = p; }
  action tag_iso_minute { t_iso_minute = p; }
  action tag_iso_second { t_iso_second = p; }
  action tag_iso_sec_fraction { t_iso_sec_fraction = p; }
  action tag_iso_zone { t_iso_zone = p; }

  action set_iso_time { iso_state |= RHRR_ISO_TIME_SET; }
  action set_iso_sec_fraction {
    t_iso_sec_fraction_end = p;
    iso_state |= RHRR_ISO_SEC_FRACTION_SET;
  }
  action set_iso_zone {
    t_iso_zone_end = p;
    iso_state |= RHRR_ISO_ZONE_SET;
  }
  action set_parser_iso { parsers |= RHRR_ISO_PARSER; }

  iso_year = ('-'? . digit{4}) >tag_iso_year;
  iso_month = ('0' . [1-9] | '1' . [0-2]) >tag_iso_month;
  iso_day = ('0' . [1-9] | [12] . [0-9] | '3' . [01]) >tag_iso_day;
  iso_hour = ([0-1] . [0-9] | '2' . [0-4]) >tag_iso_hour;
  iso_minute = ([0-5] . [0-9]) >tag_iso_minute;
  iso_second = ([0-5] . [0-9]) >tag_iso_second;
  iso_sec_fraction = ('.' . digit{1,9}) >tag_iso_sec_fraction;
  iso_zone = ([+\-] . digit{2} . ':'? . digit{2}?) > tag_iso_zone;

  iso_date = (iso_year . [\-/] . iso_month . [\-/] . iso_day);
  iso_time = (iso_hour . ':' . iso_minute . ':' . iso_second . 
              (iso_sec_fraction %set_iso_sec_fraction)? .
              (iso_zone %set_iso_zone)? ) %set_iso_time;
  iso_date_time = (iso_date . ([Tt ] . iso_time)? . space*) %/set_parser_iso;

  # RFC 2822 / HTTP

  action tag_rfc_wday { t_rfc_wday = p; }
  action tag_rfc_day { t_rfc_day = p; }
  action tag_rfc_month { t_rfc_month = p; }
  action tag_rfc_year { t_rfc_year = p; }
  action tag_rfc_hour { t_rfc_hour = p; }
  action tag_rfc_minute { t_rfc_minute = p; }
  action tag_rfc_second { t_rfc_second = p; }
  action tag_rfc_zone { t_rfc_zone = p; }

  action set_rfc_time { rfc_state |= RHRR_RFC_TIME_SET; }
  action set_rfc_zone_num {
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NUM_SET;
  }
  action set_rfc_zone_name {
    t_rfc_zone_end = p;
    rfc_state |= RHRR_RFC_ZONE_SET | RHRR_RFC_ZONE_NAME_SET;
  }
  action set_parser_rfc { parsers |= RHRR_RFC_PARSER; }

  rfc_wday = (/sun/i | /mon/i | /tue/i | /wed/i | /thu/i | /fri/i | /sat/i) >tag_rfc_wday;
  rfc_day = ([ 0] . [1-9] | [12] . [0-9] | '3' . [01]) >tag_rfc_day;
  rfc_month = abbr_month_name >tag_rfc_month;
  rfc_year = ('-'? . digit{4}) >tag_rfc_year;
  rfc_hour = ([0-1] . [0-9] | '2' . [0-4]) >tag_rfc_hour;
  rfc_minute = ([0-5] . [0-9]) >tag_rfc_minute;
  rfc_second = ([0-5] . [0-9]) >tag_rfc_second;
  rfc_zone_num = ([+\-] . digit{4}) >tag_rfc_zone %set_rfc_zone_num;
  rfc_zone_name = (/[a-ik-z]/i | /gmt/i | /ut/i | /[ecmp][ds]t/i) >tag_rfc_zone %set_rfc_zone_name;
  rfc_zone = space* . (rfc_zone_num | rfc_zone_name );

  rfc_date = (rfc_wday . ',' . space* . rfc_day . space* . rfc_month . space* . rfc_year);
  rfc_time = (rfc_hour . ':' . rfc_minute . ':' . rfc_second . (space* . rfc_zone)?) %set_rfc_time;
  rfc_date_time = (rfc_date . (space* . rfc_time)? . space*) %/set_parser_rfc;

  # Common Log Format

  action tag_clf_day { t_clf_day = p; }
  action tag_clf_month { t_clf_month = p; }
  action tag_clf_year { t_clf_year = p; }
  action tag_clf_hour { t_clf_hour = p; }
  action tag_clf_minute { t_clf_minute = p; }
  action tag_clf_second { t_clf_second = p; }
  action tag_clf_zone { t_clf_zone = p; }

  action set_clf_time { clf_state |= RHRR_CLF_TIME_SET; }
  action set_clf_zone {
    t_clf_zone_end = p;
    clf_state |= RHRR_CLF_ZONE_SET;
  }
  action set_parser_clf { parsers |= RHRR_CLF_PARSER; }

  clf_year = ('-'? . digit{4}) >tag_clf_year;
  clf_month = abbr_month_name >tag_clf_month;
  clf_day = ('0' . [1-9] | [12] . [0-9] | '3' . [01]) >tag_clf_day;
  clf_hour = ([0-1] . [0-9] | '2' . [0-4]) >tag_clf_hour;
  clf_minute = ([0-5] . [0-9]) >tag_clf_minute;
  clf_second = ([0-5] . [0-9]) >tag_clf_second;
  clf_zone = ([+\-] . digit{4}) > tag_clf_zone;

  clf_date = (clf_day . '/' . clf_month . '/' . clf_year);
  clf_time = (clf_hour . ':' . clf_minute . ':' . clf_second . (space* . clf_zone %set_clf_zone)?) %set_clf_time;
  clf_date_time = (clf_date . (':' . clf_time)? . space*) %/set_parser_clf;

  date_time = (iso_date_time | rfc_date_time | clf_date_time);
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
  long wday = 0;

  long hour = 0;
  long minute = 0;
  long second = 0;
  double sec_fraction = 0;

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
  char * t_iso_sec_fraction = NULL;
  char * t_iso_sec_fraction_end = NULL;
  char * t_iso_zone = NULL;
  char * t_iso_zone_end = NULL;

  long rfc_state = 0;
  char * t_rfc_wday = NULL;
  char * t_rfc_year = NULL;
  char * t_rfc_month = NULL;
  char * t_rfc_day = NULL;
  char * t_rfc_hour= NULL;
  char * t_rfc_minute = NULL;
  char * t_rfc_second = NULL;
  char * t_rfc_zone = NULL;
  char * t_rfc_zone_end = NULL;

  long clf_state = 0;
  char * t_clf_year = NULL;
  char * t_clf_month = NULL;
  char * t_clf_day = NULL;
  char * t_clf_hour= NULL;
  char * t_clf_minute = NULL;
  char * t_clf_second = NULL;
  char * t_clf_zone = NULL;
  char * t_clf_zone_end = NULL;

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
        if (iso_state & RHRR_ISO_SEC_FRACTION_SET) {
          sec_fraction = atoi(t_iso_sec_fraction+1) / pow(10, t_iso_sec_fraction_end - t_iso_sec_fraction - 1);
          state |= RHRR_SEC_FRACTION_SET;
        }
        if (iso_state & RHRR_ISO_ZONE_SET) {
          zone = t_iso_zone;
          zone_len = t_iso_zone_end - zone;
          if (zone_len == 3) { /* case for postgresql +03 */
            offset = atol(zone) * 3600;
          } else if (zone_len == 6) { /* standart iso */
            offset = atol(zone) * 3600 + atol(zone + 4) * 60;
          } else if (zone_len == 5) { /* case for sqlite +0300 */
            char hours[3] = { zone[1], zone[2], 0};
            offset = atol(hours) * 3600 + atol(zone + 3) * 60;
            offset *= (zone[0] == '-') ? -1 : 1;
          }
          state |= RHRR_ZONE_SET | RHRR_OFFSET_SET;
        }
      }
      break;
    case RHRR_RFC_PARSER:
      wday = rhrd__weekday_num(t_rfc_wday);
      day = atol(t_rfc_day);
      month = rhrd__month_num(t_rfc_month);
      year = atol(t_rfc_year);
      state |= RHRR_WDAY_SET | RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
      if (rfc_state & RHRR_RFC_TIME_SET) {
        hour = atol(t_rfc_hour);
        minute = atol(t_rfc_minute);
        second = atol(t_rfc_second);
        state |= RHRR_HOUR_SET | RHRR_MINUTE_SET | RHRR_SECOND_SET;
        if (rfc_state & RHRR_RFC_ZONE_SET) {
          zone = t_rfc_zone;
          zone_len = t_rfc_zone_end - zone;
          state |= RHRR_ZONE_SET;
          if (rfc_state & RHRR_RFC_ZONE_NUM_SET) {
            offset = atol(zone);
            offset = (offset/100) * 3600 + (labs(offset) % 100) * 60 * (offset < 0 ? -1 : 1);
            state |= RHRR_OFFSET_SET;
          } else if (rfc_state & RHRR_RFC_ZONE_NAME_SET) {
            offset = rhrd__rfc_zone_offset(zone);
            if (offset == RHRR_BAD_OFFSET) {
              state = 0;
            } else {
              state |= RHRR_OFFSET_SET;
            }
          }
        }
      }
      break;
    case RHRR_CLF_PARSER:
      year = atol(t_clf_year);
      month = rhrd__month_num(t_clf_month);
      day = atol(t_clf_day);
      state |= RHRR_YEAR_SET | RHRR_MONTH_SET | RHRR_DAY_SET;
      if (clf_state & RHRR_ISO_TIME_SET) {
        hour = atol(t_clf_hour);
        minute = atol(t_clf_minute);
        second = atol(t_clf_second);
        state |= RHRR_HOUR_SET | RHRR_MINUTE_SET | RHRR_SECOND_SET;
        if (clf_state & RHRR_ISO_ZONE_SET) {
          zone = t_clf_zone;
          zone_len = t_clf_zone_end - zone;
          offset = atol(zone);
          offset = (offset/100) * 3600 + (labs(offset) % 100) * 60 * (offset < 0 ? -1 : 1);
          state |= RHRR_ZONE_SET | RHRR_OFFSET_SET;
        }
      }
      break;
  }

  if (!state) {
    return Qnil;
  }

  hash = rb_hash_new();
  if(state & RHRR_WDAY_SET) {
    rb_hash_aset(hash, rhrd_sym_wday, LONG2NUM(wday));
  } 
  if(state & RHRR_YEAR_SET) {
    rb_hash_aset(hash, rhrd_sym_year, LONG2NUM(year));
  } 
  if(state & RHRR_MONTH_SET) {
    rb_hash_aset(hash, rhrd_sym_mon, LONG2NUM(month));
  } 
  if(state & RHRR_DAY_SET) {
    rb_hash_aset(hash, rhrd_sym_mday, LONG2NUM(day));
  } 
  if(state & RHRR_HOUR_SET) {
    rb_hash_aset(hash, rhrd_sym_hour, LONG2NUM(hour));
  } 
  if(state & RHRR_MINUTE_SET) {
    rb_hash_aset(hash, rhrd_sym_min, LONG2NUM(minute));
  } 
  if(state & RHRR_SECOND_SET) {
    rb_hash_aset(hash, rhrd_sym_sec, LONG2NUM(second));
  } 
  if(state & RHRR_SEC_FRACTION_SET) {
    rb_hash_aset(hash, rhrd_sym_sec_fraction, rb_float_new(sec_fraction));
  }
  if(state & RHRR_ZONE_SET) {
    rzone = rb_str_new(zone, zone_len);
    rb_hash_aset(hash, rhrd_sym_zone, rzone);
    if (state & RHRR_OFFSET_SET) {
        rb_hash_aset(hash, rhrd_sym_offset, LONG2NUM(offset));
    } else {
      rzone = rhrd_s_zone_to_diff(rzone, rzone);
      if(RTEST(rzone)) {
        rb_hash_aset(hash, rhrd_sym_offset, rzone);
      }
    }
  } 

  return hash;
}

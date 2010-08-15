
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ruby.h>

#ifdef RUBY19
#define RHR_DEFAULT_CWYEAR -4713
#define RHR_DEFAULT_CWEEK 1
#define RHR_DEFAULT_CWDAY 1
#else
#define RHR_DEFAULT_CWYEAR 1582
#define RHR_DEFAULT_CWEEK 41
#define RHR_DEFAULT_CWDAY 5
#endif

#define RHR_JD_MJD 2400001
#define RHR_JD_LD 2299160
#define RHR_JD_ITALY 2299161
#define RHR_JD_ENGLAND 2361222
#define RHR_UNIX_EPOCH 2440588
#define RHR_SECONDS_PER_DAY 86400

/*
In both the 32-bit and 64-bit cases, the limits are chosen so that you cannot
store a civil date where converting it to a jd would cause an overflow.
*/
#if __LP64__
/* 
On 64-bit systems, the limits depends on the number of significant digits in
a double (15).  These are slightly below the maximum so that all numbers used
in calculations have fewer than 15 digits.
*/
#define RHR_JD_MAX 999979466117609
#define RHR_JD_MIN -999979466119058
#define RHR_YEAR_MAX 2737850782415
#define RHR_MONTH_MAX 12
#define RHR_DAY_MAX 5
#define RHR_YEAR_MIN -2737850791845
#define RHR_MONTH_MIN 11
#define RHR_DAY_MIN 26

#else
/* 
On 32-bit systems, the limits depend on the storage limits of 32-bit integers.
The numbers are slightly less than 2**31 - 1 and slightly greater than -2**31
so that no calculations can overflow.
*/
#define RHR_JD_MAX 2147438064
#define RHR_JD_MIN -2145083647
#define RHR_YEAR_MAX 5874773
#define RHR_MONTH_MAX 8
#define RHR_DAY_MAX 15
#define RHR_YEAR_MIN -5877752
#define RHR_MONTH_MIN 5
#define RHR_DAY_MIN 8
#endif

#define RHR_HAVE_JD 1
#define RHR_HAVE_CIVIL 2
#define RHR_HAVE_NANOS 4
#define RHR_HAVE_HMS 8

#define RHR_NANOS_PER_SECOND 1000000000LL
#define RHR_NANOS_PER_MINUTE 60000000000LL
#define RHR_NANOS_PER_DAY 86400000000000LL
#define RHR_NANOS_PER_DAYD 86400000000000.0
#define RHR_NANOS_PER_SECONDD 1000000000.0

#define RHRR_YEAR_SET 0x1
#define RHRR_MONTH_SET 0x2
#define RHRR_DAY_SET 0x4
#define RHRR_YDAY_SET 0x8
#define RHRR_HOUR_SET 0x10
#define RHRR_MINUTE_SET 0x20
#define RHRR_SECOND_SET 0x40
#define RHRR_WDAY_SET 0x80
#define RHRR_CENTURY_SET 0x100
#define RHRR_CWYEAR_SET 0x200
#define RHRR_CWEEK_SET 0x400
#define RHRR_CWDAY_SET 0x800
#define RHRR_SEC_FRACTION_SET 0x1000
#define RHRR_UNIX_SET 0x2000
#define RHRR_WNUM0_SET 0x4000
#define RHRR_WNUM1_SET 0x8000
#define RHRR_MERIDIAN_SET 0x10000
#define RHRR_ZONE_SET 0x20000
#define RHRR_OFFSET_SET 0x40000

#define RHR_HAS_JD(d) (((d)->flags & RHR_HAVE_JD) == RHR_HAVE_JD)
#define RHR_HAS_CIVIL(d) (((d)->flags & RHR_HAVE_CIVIL) == RHR_HAVE_CIVIL)

#define RHR_FILL_JD(d) if (((d)->flags & RHR_HAVE_JD) == 0) { rhrd__civil_to_jd(d); }
#define RHR_FILL_CIVIL(d) if (((d)->flags & RHR_HAVE_CIVIL) == 0) { rhrd__jd_to_civil(d); }

#define RHRDT_FILL_JD(d) if (!((d)->flags & RHR_HAVE_JD)) { rhrdt__civil_to_jd(d); }
#define RHRDT_FILL_CIVIL(d) if (!((d)->flags & RHR_HAVE_CIVIL)) { rhrdt__jd_to_civil(d); }
#define RHRDT_FILL_HMS(d) if (!((d)->flags & RHR_HAVE_HMS)) { rhrdt__nanos_to_hms(d); }
#define RHRDT_FILL_NANOS(d) if (!((d)->flags & RHR_HAVE_NANOS)) { rhrdt__hms_to_nanos(d); }

#ifdef RUBY186
#define RHR_RETURN_RESIZED_STR(s, len) return rb_str_resize(s, len);
#else
#define RHR_RETURN_RESIZED_STR(s, len) rb_str_set_len(s, len); return s;
#endif

#define RHR_SPACE_SHIP(x, l, r) if (l < r) { x = -1; } else if (l == r) { x = 0; } else { x = 1; } 

#define RHR_CHECK_JD(d) if ((d->jd > RHR_JD_MAX) || (d->jd < RHR_JD_MIN)) { rb_raise(rb_eRangeError, "date out of range: jd = %li", d->jd);}
#define RHR_CHECK_CIVIL(d) if (!rhrd__valid_civil_limits(d->year, d->month, d->day)) { rb_raise(rb_eRangeError, "date out of range: year = %li, month = %hhi, day = %hhi", d->year, d->month, d->day);}

typedef struct rhrd_s {
  long jd;
  long year;
  unsigned char month;
  unsigned char day;
  unsigned char flags;
} rhrd_t;

typedef struct rhrdt_s {
  long long nanos; /* Nanoseconds since start of day */
  long jd;
  long year;
  short offset; /* Offset from UTC in minutes */
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
  unsigned char flags;
} rhrdt_t;

const unsigned char rhrd_days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const long rhrd_cumulative_days_in_month[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
const unsigned char rhrd_yday_to_month[366] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12};
const unsigned char rhrd_leap_yday_to_month[367] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12};
const char * rhrd__month_names[13] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char * rhrd__abbr_month_names[13] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char * rhrd__day_names[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char * rhrd__abbr_day_names[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char rhrd__zone_re_str[] = "\\A(?:gmt|utc?)?[-+]\\d+(?:[,.:]\\d+(?::\\d+)?)?|[[:alpha:].\\s]+(?:standard|daylight)\\s+time\\b|[[:alpha:]]+(?:\\s+dst)?\\b";
const char rhrd__zone_dst_re_str[] = "\\s+(?:(?:(standard)|daylight)\\s+time|dst)\\z";
const char rhrd__zone_sign_re_str[] = "\\A(?:gmt|utc?)?(?:(-)|\\+)";

VALUE rhrd_class;
VALUE rhrd_s_class;
VALUE rhrdt_class;
VALUE rhrdt_s_class;
VALUE rhrd_monthnames;
VALUE rhrd_abbr_monthnames;
VALUE rhrd_daynames;
VALUE rhrd_abbr_daynames;
VALUE rhrd_zone_re;
VALUE rhrd_zone_dst_re;
VALUE rhrd_zone_sign_re;
VALUE rhrd_zones_hash;
VALUE rhrd_empty_string;
VALUE rhrd_string_colon;
VALUE rhrd_re_comma_period;

ID rhrd_id_op_array;
ID rhrd_id_op_gte;
ID rhrd_id_op_lt;
ID rhrd_id__parse;
ID rhrd_id_downcase;
ID rhrd_id_getlocal;
ID rhrd_id_hash;
ID rhrd_id_include_q;
ID rhrd_id_length;
ID rhrd_id_local;
ID rhrd_id_localtime;
ID rhrd_id_match;
ID rhrd_id_now;
ID rhrd_id_slice;
ID rhrd_id_split;
ID rhrd_id_sub_b;
ID rhrd_id_to_i;
#ifdef RUBY19
ID rhrd_id_nsec;
#else
ID rhrd_id_usec;
#endif
ID rhrd_id_utc;
ID rhrd_id_utc_offset;

#ifdef RUBY19
ID rhrd_id__httpdate;
ID rhrd_id__iso8601;
ID rhrd_id__jisx0301;
ID rhrd_id__rfc2822;
ID rhrd_id__rfc3339;
ID rhrd_id__xmlschema;
#endif

VALUE rhrd_sym_cwday;
VALUE rhrd_sym_cweek;
VALUE rhrd_sym_cwyear;
VALUE rhrd_sym_hour;
#ifdef RUBY19
VALUE rhrd_sym_leftover;
#endif
VALUE rhrd_sym_mday;
VALUE rhrd_sym_min;
VALUE rhrd_sym_mon;
VALUE rhrd_sym_offset;
VALUE rhrd_sym_sec;
VALUE rhrd_sym_sec_fraction;
VALUE rhrd_sym_seconds;
VALUE rhrd_sym_wday;
VALUE rhrd_sym_wnum0;
VALUE rhrd_sym_wnum1;
VALUE rhrd_sym_yday;
VALUE rhrd_sym_year;
VALUE rhrd_sym_zone;

static VALUE rhrd_step(int argc, VALUE *argv, VALUE self);
static VALUE rhrdt_step(int argc, VALUE *argv, VALUE self);
static VALUE rhrd_to_s(VALUE self);
static VALUE rhrdt_to_s(VALUE self);
static VALUE rhrd_s_zone_to_diff(VALUE self, VALUE zone);
VALUE rhrdt__new_offset(VALUE self, double offset);
void rhrdt__civil_to_jd(rhrdt_t *d);
void rhrdt__jd_to_civil(rhrdt_t *date);
void rhrdt__nanos_to_hms(rhrdt_t *d);
void rhrdt__hms_to_nanos(rhrdt_t *d);

/* C Helper Methods */

long rhrd__mod(long a, long b) {
  long c;
  c = a % b;
  if (c < 0) {
    c += b;
  }
  return c;
}

int rhrd__valid_civil_limits(long year, long month, long day) {
  if (year > RHR_YEAR_MAX || year < RHR_YEAR_MIN) {
    return 0;
  }
  else if (year == RHR_YEAR_MAX) {
    if (month > RHR_MONTH_MAX) {
      return 0;
    } else if (month == RHR_MONTH_MAX) {
      if (day > RHR_DAY_MAX) {
        return 0;
      }
    }
  } else if (year == RHR_YEAR_MIN) {
    if (month < RHR_MONTH_MIN) {
      return 0;
    } else if (month == RHR_MONTH_MIN) {
      if (day < RHR_DAY_MIN) {
        return 0;
      }
    }
  }
  return 1;
}

long rhrd__safe_add_long(long a, long b) {
  if ((a > 0 && b > 0 && (a > LONG_MAX - b)) ||
      (a < 0 && b < 0 && (a < LONG_MIN - b))) {
    rb_raise(rb_eRangeError, "addition would overflow");
  }
  return a + b;
}

long rhrd__ymd_to_jd(long year, long month, long day) {
  long a;
  if (month <= 2) {
    a = (long)floor((year - 1)/100.0);
    return (long)floor(365.25 * (year + 4715)) + \
          (long)floor(30.6001 * (month + 13)) + \
          day - 1524 + (2 - a + (long)floor(a / 4.0));
  } else {
    a = (long)floor(year/100.0);
    return (long)floor(365.25 * (year + 4716)) + \
          (long)floor(30.6001 * (month + 1)) + \
          day - 1524 + (2 - a + (long)floor(a / 4.0));
  }
}

void rhrd__civil_to_jd(rhrd_t *d) {
  d->jd = rhrd__ymd_to_jd(d->year, d->month, d->day);
  d->flags |= RHR_HAVE_JD;
}

void rhrd__jd_to_civil(rhrd_t *date) {
  long x, a, b, c, d, e;
  x = (long)floor((date->jd - 1867216.25) / 36524.25);
  a = date->jd + 1 + x - (long)floor(x / 4.0);
  b = a + 1524;
  c = (long)floor((b - 122.1) / 365.25);
  d = (long)floor(365.25 * c);
  e = (long)floor((b - d) / 30.6001);
  date->day = b - d - (long)floor(30.6001 * e);
  if (e <= 13) {
    date->month = e - 1;
    date->year = c - 4716;
  } else {
    date->month = e - 13;
    date->year = c - 4715;
  }
  date->flags |= RHR_HAVE_CIVIL;
}

int rhrd__leap_year(long year) {
  if (year % 400 == 0) {
    return 1;
  } else if (year % 100 == 0) {
    return 0;
  } else if (year % 4 == 0) {
    return 1;
  } else {
    return 0;
  }
}

int rhrd__valid_civil(rhrd_t *d, long year, long month, long day) {
  if (month < 0 && month >= -12) {
    month += 13;
  }
  if (month < 1 || month > 12) {
    return 0;
  }

  if (day < 0) {
    if (month == 2) {
      day += rhrd__leap_year(year) ? 30 : 29;
    } else {
      day += rhrd_days_in_month[month] + 1;
    }
  }
  if (day < 1 || day > 28) {
    if (day > 31 || day <= 0) {
      return 0;
    } else if (month == 2) {
      if (rhrd__leap_year(year)) {
        if (day > 29) {
          return 0;
        }
      } else if (day > 28) {
        return 0;
      }
    } else if (day > rhrd_days_in_month[month]) {
      return 0;
    }
  }

  if(!rhrd__valid_civil_limits(year, month, day)) {
    return 0;
  } 

  d->year = year;
  d->month = (unsigned char)month;
  d->day = (unsigned char)day;
  d->flags |= RHR_HAVE_CIVIL;
  return 1;
}

unsigned char rhrd__days_in_month(long year, unsigned char month) {
  if (month == 2) {
    return rhrd__leap_year(year) ? 29 : 28;
  } else {
    return rhrd_days_in_month[month];
  }
}

VALUE rhrd__add_days(VALUE self, long n) {
  rhrd_t *d;
  rhrd_t *newd;
  VALUE new;
  long x;
  Data_Get_Struct(self, rhrd_t, d);
  new = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, newd);

  if(!(RHR_HAS_JD(d))) {
    x = rhrd__safe_add_long(n, (long)(d->day));
    if (x >= 1 && x <= 28) {
      newd->year = d->year;
      newd->month = d->month;
      newd->day = (unsigned char)x;
      RHR_CHECK_CIVIL(newd)
      newd->flags = RHR_HAVE_CIVIL;
      return new;
    }
    RHR_FILL_JD(d)
  }

  newd->jd = rhrd__safe_add_long(n, d->jd);
  RHR_CHECK_JD(newd)
  newd->flags = RHR_HAVE_JD;
  return new;
}

VALUE rhrd__add_months(VALUE self, long n) {
  rhrd_t *d;
  rhrd_t *newd;
  VALUE new;
  long x;
  Data_Get_Struct(self, rhrd_t, d);

  new = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, newd);
  RHR_FILL_CIVIL(d)
  n = rhrd__safe_add_long(n, (long)(d->month));
  if(n > 1 && n <= 12) {
    newd->year = d->year;
    newd->month = n;
  } else {
    x = n / 12;
    n = n % 12;
    if (n <= 0) {
      newd->year = d->year + x - 1;
      newd->month = n + 12;
    } else {
      newd->year = d->year + x;
      newd->month = (unsigned char)n;
    }
  }
  x = rhrd__days_in_month(newd->year, newd->month);
  newd->day = (unsigned char)(d->day > x ? x : d->day);
  RHR_CHECK_CIVIL(newd)
  newd->flags = RHR_HAVE_CIVIL;
  return new;
}

long rhrd__spaceship(rhrd_t *d, rhrd_t *o) {
  long diff;
  if (RHR_HAS_JD(d) && RHR_HAS_JD(o)) {
    RHR_SPACE_SHIP(diff, d->jd, o->jd)
  } else if (RHR_HAS_CIVIL(d) && RHR_HAS_CIVIL(o)) {
    RHR_SPACE_SHIP(diff, d->year, o->year)
    if (!diff) {
      RHR_SPACE_SHIP(diff, d->month, o->month)
      if (!diff) {
        RHR_SPACE_SHIP(diff, d->day, o->day)
      }
    }
  } else {
   RHR_FILL_JD(d)
   RHR_FILL_JD(o)
   RHR_SPACE_SHIP(diff, d->jd, o->jd)
  }
  return diff;
}

long rhrd__commercial_to_jd(long cwyear, long cweek, long cwday) {
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));

  n.year = cwyear;
  n.month = 1;
  n.day = 4;
  rhrd__civil_to_jd(&n);
  return n.jd - rhrd__mod(n.jd, 7) + 7 * (cweek - 1) + (cwday - 1);
}

long rhrd__jd_to_cwday(long jd) {
  long day;
  day = (jd + 1) % 7; 
  if (day <= 0) {
    day += 7;
  }
  return day;
}

long rhrd__jd_to_wday(long jd) {
  return rhrd__mod(jd + 1, 7);
}

/* Abuses the year, month, and day fields to store
 * cwyear, cweek, and cwday */
void rhrd__fill_commercial(rhrd_t *d) {
  long a;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));

  n.jd = d->jd - 3;
  rhrd__jd_to_civil(&n);
  a = n.year;
  d->year = d->jd >= rhrd__commercial_to_jd(a + 1, 1, 1) ? a + 1 : a;
  d->month = 1 + (d->jd - rhrd__commercial_to_jd(d->year, 1, 1)) / 7;
  d->day = (unsigned char)rhrd__jd_to_cwday(d->jd);
}

int rhrd__valid_commercial(rhrd_t *d, long cwyear, long cweek, long cwday) {
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));

  if (cwday < 0) {
    if (cwday < -8) {
      return 0;
    }
    cwday += 8;
  }
  if (cweek < 0) {
    if (cweek < -53) {
      return 0;
    }
    n.jd = rhrd__commercial_to_jd(cwyear + 1, 1, 1) + cweek * 7;
    rhrd__fill_commercial(&n);
    if (n.year != cwyear) {
      return 0;
    }
    cweek = n.month;
    memset(&n, 0, sizeof(rhrd_t));
  }

  n.jd = rhrd__commercial_to_jd(cwyear, cweek, cwday);
  rhrd__fill_commercial(&n);
  if(cwyear != n.year || cweek != n.month || cwday != n.day) {
    return 0;
  }

  if ((n.jd > RHR_JD_MAX) || (n.jd < RHR_JD_MIN)) {
    return 0;
  }

  d->jd = n.jd;
  d->flags = RHR_HAVE_JD;
  return 1;
}

long rhrd__ordinal_day(long year, unsigned char month, unsigned char day) {
  long yday;
  yday = rhrd_cumulative_days_in_month[month] + day;
  if(month > 2 && rhrd__leap_year(year)) {
    yday += 1;
  }
  return yday;
}

int rhrd__valid_ordinal(rhrd_t *d, long year, long yday) {
  int leap;
  long month, day;

  leap = rhrd__leap_year(year);
  if (yday < 0) {
    if (leap) {
      yday += 367;
    } else {
      yday += 366;
    }
  }
  if (yday < 1 || yday > (leap ? 366 : 365)) {
    return 0;
  }
  if (leap) {
    month = rhrd_leap_yday_to_month[yday];
    if (yday > 60) {
      day = yday - rhrd_cumulative_days_in_month[month] - 1;
    } else {
      day = yday - rhrd_cumulative_days_in_month[month];
    }
  } else {
    month = rhrd_yday_to_month[yday];
    day = yday - rhrd_cumulative_days_in_month[month];
  }

  if(!rhrd__valid_civil_limits(year, month, day)) {
    return 0;
  } 

  d->year = year;
  d->month = (unsigned char)month;
  d->day = (unsigned char)day;
  d->flags |= RHR_HAVE_CIVIL;
  return 1;
}

long rhrd__jd_to_unix(long jd) {
  return (jd - RHR_UNIX_EPOCH) * RHR_SECONDS_PER_DAY;
}

long rhrd__unix_to_jd(long t) {
  return t/RHR_SECONDS_PER_DAY + RHR_UNIX_EPOCH;
}

void rhrd__today(rhrd_t * d) {
  VALUE t;
  t = rb_funcall(rb_cTime, rhrd_id_now, 0);
  d->jd = rhrd__unix_to_jd(NUM2LONG(rb_funcall(t, rhrd_id_to_i, 0)) + NUM2LONG(rb_funcall(t, rhrd_id_utc_offset, 0)));
  d->flags |= RHR_HAVE_JD;
  RHR_CHECK_JD(d);
}

long rhrd__current_year(void) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));
  rhrd__today(&d);
  RHR_FILL_CIVIL(&d);
  return d.year;
}

long rhrd__current_month(void) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));
  rhrd__today(&d);
  RHR_FILL_CIVIL(&d);
  return d.month;
}

long rhrd__yday1_jd(long year) {
  rhrd_t d;

  memset(&d, 0, sizeof(rhrd_t));
  d.year = year;
  d.month = 1;
  d.day = 1;
  d.flags = RHR_HAVE_CIVIL;
  RHR_FILL_JD(&d);

  return d.jd;
}

long rhrd__jd_to_weeknum(long jd, int f) {
  long yday1_jd;
  rhrd_t d;

  memset(&d, 0, sizeof(rhrd_t));
  d.jd = jd;
  d.flags = RHR_HAVE_JD;
  RHR_FILL_CIVIL(&d)

  yday1_jd = rhrd__yday1_jd(d.year) + 6;
  return (jd - (yday1_jd - (rhrd__mod(yday1_jd - f + 1, 7) + 7))) / 7;
}

long rhrd__weeknum_to_jd(long year, long week, long wday, int f) {
  long yday1_jd;
  yday1_jd = rhrd__yday1_jd(year) + 6;
  return (yday1_jd - rhrd__mod(yday1_jd - f + 1, 7) - 7) + 7 * week + wday;
}

/* Fills the date structure with information from the hash, returns:
 *  0: No errors (i.e. jd and/or year, month, day filled)
 *  1: Bad date information given (e.g. 2009-02-29)
 * -1: No date information given */
int rhrd__fill_from_hash(rhrd_t *d, VALUE hash) {
  long year = 0;
  long month = 0;
  long day = 0;
  long yday = 0;
  long wday = 0;
  long cwyear = 0;
  long cweek = 0;
  long cwday = 0;
  VALUE ryear, rmonth, rday, ryday, rwday, rcwyear, rcweek, rcwday, runix, rwnum0, rwnum1;

  if (!RTEST(hash)) {
    return -1;
  }
  runix = rb_hash_aref(hash, rhrd_sym_seconds);
  if (RTEST(runix)) {
    d->jd = rhrd__unix_to_jd(NUM2LONG(runix));
    d->flags |= RHR_HAVE_JD;
    return 0;
  }

  ryear = rb_hash_aref(hash, rhrd_sym_year);
  rmonth = rb_hash_aref(hash, rhrd_sym_mon);
  rday = rb_hash_aref(hash, rhrd_sym_mday);
  ryday = rb_hash_aref(hash, rhrd_sym_yday);
  rwday = rb_hash_aref(hash, rhrd_sym_wday);
  rcwyear = rb_hash_aref(hash, rhrd_sym_cwyear);
  rcweek = rb_hash_aref(hash, rhrd_sym_cweek);
  rcwday = rb_hash_aref(hash, rhrd_sym_cwday);
  rwnum0 = rb_hash_aref(hash, rhrd_sym_wnum0);
  rwnum1 = rb_hash_aref(hash, rhrd_sym_wnum1);

  if (RTEST(ryear)) {
    year = NUM2LONG(ryear);
    if (RTEST(ryday)) {
      yday = NUM2LONG(ryday);
    } else if (RTEST(rmonth) && RTEST(rday)) {
      month = NUM2LONG(rmonth);
      day = NUM2LONG(rday);
    } else if (RTEST(rwday)) {
      d->jd = rhrd__yday1_jd(year);
      d->flags |= RHR_HAVE_JD;
      rhrd__fill_commercial(d);
      if(!rhrd__valid_commercial(d, d->year, 1, NUM2LONG(rwday))) {
        return 1;
      }
      d->flags &= ~RHR_HAVE_CIVIL;
      return 0;
    } else if (RTEST(rwnum0)) {
      d->jd = rhrd__weeknum_to_jd(year, NUM2LONG(rwnum0), RTEST(rwday) ? NUM2LONG(rwday) : (RTEST(rcwday) ? rhrd__mod(NUM2LONG(rcwday), 7) : 0), 0);
      d->flags |= RHR_HAVE_JD;
      return 0;
    } else if (RTEST(rwnum1)) {
      d->jd = rhrd__weeknum_to_jd(year, NUM2LONG(rwnum1), RTEST(rwday) ? rhrd__mod(NUM2LONG(rwday) - 1, 7) : (RTEST(rcwday) ? rhrd__mod(NUM2LONG(rcwday) - 1, 7) : 0), 1);
      d->flags |= RHR_HAVE_JD;
      return 0;
    } else {
      month = RTEST(rmonth) ? NUM2LONG(rmonth) : 1;
      day = RTEST(rday) ? NUM2LONG(rday) : 1;
    }
  } else if (RTEST(rmonth)) {
    year = rhrd__current_year();
    month = NUM2LONG(rmonth);
    day = RTEST(rday) ? NUM2LONG(rday) : 1;
  } else if (RTEST(rday)) {
    year = rhrd__current_year();
    month = rhrd__current_month();
    day = NUM2LONG(rday);
  } else if (RTEST(ryday)) {
    year = rhrd__current_year();
    yday = NUM2LONG(ryday);
  } else if (RTEST(rwnum0)) {
    d->jd = rhrd__weeknum_to_jd(rhrd__current_year(), NUM2LONG(rwnum0), RTEST(rwday) ? NUM2LONG(rwday) : (RTEST(rcwday) ? rhrd__mod(NUM2LONG(rcwday), 7) : 0), 0);
    d->flags |= RHR_HAVE_JD;
    return 0;
  } else if (RTEST(rwnum1)) {
    d->jd = rhrd__weeknum_to_jd(rhrd__current_year(), NUM2LONG(rwnum1), RTEST(rwday) ? rhrd__mod(NUM2LONG(rwday) - 1, 7) : (RTEST(rcwday) ? rhrd__mod(NUM2LONG(rcwday) - 1, 7) : 0), 1);
    d->flags |= RHR_HAVE_JD;
    return 0;
  } else if (RTEST(rcwyear)) {
    cwyear = NUM2LONG(rcwyear);
    cweek = RTEST(rcweek) ? NUM2LONG(rcweek) : 1;
    cwday = RTEST(rcwday) ? NUM2LONG(rcwday) : 1;
  } else if (RTEST(rcweek)) {
    cwyear = rhrd__current_year();
    cweek = NUM2LONG(rcweek);
    cwday = RTEST(rcwday) ? NUM2LONG(rcwday) : 1;
  } else if (RTEST(rcwday)) {
    cwyear = rhrd__current_year();
    cweek = 1;
    cwday = NUM2LONG(rcwday);
  } else if (RTEST(rwday)) {
    wday = NUM2LONG(rwday);
    rhrd__today(d);
    d->jd += wday - rhrd__jd_to_wday(d->jd);
    d->flags &= ~RHR_HAVE_CIVIL;
    return 0;
  } else {
    return -1;
  }
  if (yday && rhrd__valid_ordinal(d, year, yday)) {
    return 0;
  } else if (cweek && cwday && rhrd__valid_commercial(d, cwyear, cweek, cwday)) {
    return 0;
  } else if (!rhrd__valid_civil(d, year, month, day)) {
    return 1;
  }

  return 0;
}

VALUE rhrd__from_hash(VALUE hash) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, d);
  if(rhrd__fill_from_hash(d, hash)) {
    rb_raise(rb_eArgError, "invalid date");
  } else {
    RHR_FILL_JD(d)
    RHR_CHECK_JD(d)
    return rd;
  }
}

VALUE rhrd__strftime(rhrdt_t *d, char * fmt, int fmt_len) {
  int i;
  int cp = 0;
  int str_len = 128;
  int str_lim = 64;
  int mod = 0;
  char * str;
  char c;
  VALUE s;
  rhrd_t cd;

  memset(&cd, 0, sizeof(rhrd_t));
  cd.jd = d->jd;
  cd.flags = RHR_HAVE_JD;
  rhrd__fill_commercial(&cd);

  s = rb_str_buf_new(str_len);
  str = RSTRING_PTR(s);
  for (i = 0; i < fmt_len; i++) {
    if (cp >= str_lim) {
       str_len *= 2;
       str_lim = str_len - 64;
#ifndef RUBY186
       rb_str_set_len(s, cp);
#endif
       s = rb_str_resize(s, str_len);
       str = RSTRING_PTR(s);
    }
    c = fmt[i];

    if (mod) {
      switch (c) {
        case 'a':
          cp += sprintf(str + cp, "%s", rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)]);
          break;
        case 'A':
          cp += sprintf(str + cp, "%s", rhrd__day_names[rhrd__jd_to_wday(d->jd)]);
          break;
        case 'h':
        case 'b':
          cp += sprintf(str + cp, "%s", rhrd__abbr_month_names[d->month]);
          break;
        case 'B':
          cp += sprintf(str + cp, "%s", rhrd__month_names[d->month]);
          break;
        case 'c':
          cp += sprintf(str + cp, "%s %s %2hhi %02hhi:%02hhi:%02hhi %04li", rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)], rhrd__abbr_month_names[d->month], d->day, d->hour, d->minute, d->second, d->year);
          break;
        case 'C':
          cp += sprintf(str + cp, "%02li", d->year / 100);
          break;
        case 'd':
          cp += sprintf(str + cp, "%02hhi", d->day);
          break;
        case 'e':
          cp += sprintf(str + cp, "%2hhi", d->day);
          break;
        case 'F':
          cp += sprintf(str + cp, "%04li-%02hhi-%02hhi", d->year, d->month, d->day);
          break;
        case 'g':
          cp += sprintf(str + cp, "%02li", cd.year % 100);
          break;
        case 'G':
          cp += sprintf(str + cp, "%04li", cd.year);
          break;
        case 'H':
          cp += sprintf(str + cp, "%02hhi", d->hour);
          break;
        case 'I':
          cp += sprintf(str + cp, "%02hhi", (d->hour == 12 || d->hour == 0) ? 12 : d->hour % 12);
          break;
        case 'j':
          cp += sprintf(str + cp, "%03li", rhrd__ordinal_day(d->year, d->month, d->day));
          break;
        case 'k':
          cp += sprintf(str + cp, "%2hhi", d->hour);
          break;
        case 'l':
          cp += sprintf(str + cp, "%2hhi", (d->hour == 12 || d->hour == 0) ? 12 : d->hour % 12);
          break;
        case 'L':
          cp += sprintf(str + cp, "%03lli", (d->nanos % RHR_NANOS_PER_SECOND)/1000000);
          break;
        case 'm':
          cp += sprintf(str + cp, "%02hhi", d->month);
          break;
        case 'M':
          cp += sprintf(str + cp, "%02hhi", d->minute);
          break;
        case 'N':
          cp += sprintf(str + cp, "%09lli", (d->nanos % RHR_NANOS_PER_SECOND));
          break;
        case 'n':
          cp += sprintf(str + cp, "\n");
          break;
        case 'p':
          cp += sprintf(str + cp, d->hour >= 12 ? "PM" : "AM");
          break;
        case 'P':
          cp += sprintf(str + cp, d->hour >= 12 ? "pm" : "am");
          break;
        case 'Q':
          cp += sprintf(str + cp, "%li", rhrd__jd_to_unix(d->jd) * 1000);
          break;
        case 'r':
          cp += sprintf(str + cp, "%2hhi:%02hhi:%02hhi %s", (d->hour == 12 || d->hour == 0) ? 12 : d->hour % 12, d->minute, d->second, d->hour >= 12 ? "PM" : "AM");
          break;
        case 'R':
          cp += sprintf(str + cp, "%02hhi:%02hhi", d->hour, d->minute);
          break;
        case 's':
          cp += sprintf(str + cp, "%li", (long)(rhrd__jd_to_unix(d->jd) + d->nanos/RHR_NANOS_PER_SECOND - d->offset * 60));
          break;
        case 'S':
          cp += sprintf(str + cp, "%02hhi", d->second);
          break;
        case 't':
          cp += sprintf(str + cp, "\t");
          break;
        case 'X':
        case 'T':
          cp += sprintf(str + cp, "%02hhi:%02hhi:%02hhi", d->hour, d->minute, d->second);
          break;
        case 'u':
          cp += sprintf(str + cp, "%hhi", cd.day);
          break;
        case 'U':
          cp += sprintf(str + cp, "%li", rhrd__jd_to_weeknum(d->jd, 0));
          break;
        case 'v':
          cp += sprintf(str + cp, "%2hhi-%s-%04li", d->day, rhrd__abbr_month_names[d->month], d->year);
          break;
        case 'V':
          cp += sprintf(str + cp, "%02hhi", cd.month);
          break;
        case 'w':
          cp += sprintf(str + cp, "%li", rhrd__jd_to_wday(d->jd));
          break;
        case 'W':
          cp += sprintf(str + cp, "%li", rhrd__jd_to_weeknum(d->jd, 1));
          break;
        case 'D':
        case 'x':
          cp += sprintf(str + cp, "%02hhi/%02hhi/%02li", d->month, d->day, d->year % 100);
          break;
        case 'y':
          cp += sprintf(str + cp, "%02li", d->year % 100);
          break;
        case 'Y':
          cp += sprintf(str + cp, "%04li", d->year);
          break;
        case 'z':
          cp += sprintf(str + cp, "%+03i%02i", d->offset/60, abs(d->offset % 60));
          break;
        case 'Z':
          cp += sprintf(str + cp, "%+03i:%02i", d->offset/60, abs(d->offset % 60));
          break;
        case '+':
          cp += sprintf(str + cp, "%s %s %2hhi %02hhi:%02hhi:%02hhi %+03i:%02i %04li", rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)], rhrd__abbr_month_names[d->month], d->day, d->hour, d->minute, d->second, d->offset/60, abs(d->offset % 60), d->year);
          break;
        default:
          str[cp] = c;
          cp += 1;
      }
      mod = 0;
    } else {
      if (c == '%') {
        mod = 1;
      } else {
        str[cp] = c;
        cp += 1;
      }
    }
  }

  RHR_RETURN_RESIZED_STR(s, cp)
}

VALUE rhrd__strptime(VALUE rstr, char *fmt_str, long fmt_len) {
  char * str;
  long len;
  long year = 0;
  long month = 0;
  long day = 0;
  long yday = 0;
  long wday = 0;
  long cwyear = 0;
  long cweek = 0;
  long cwday = 0;
  long century = 0;
  long hour = 0;
  long minute = 0;
  long second = 0;
  long seconds = 0;
  long sec_fraction_num = 0;
  double sec_fraction = 0.0;
  long meridian = 0;
  long state = 0;
  long mod = 0;
  long pos = 0;
  long wnum0 = 0;
  long wnum1 = 0;
  long i;
  long fmt_pos;
  int scan_len;
  VALUE zone = Qnil;
  VALUE hash;
  rstr = rb_str_to_str(rstr);
  str = RSTRING_PTR(rstr);
  len = RSTRING_LEN(rstr);

  for (fmt_pos = 0; fmt_pos < fmt_len; fmt_pos++) {
    if (pos >= len) {
#ifdef DEBUG
      printf("format longer than input\n");
#endif
      return Qnil;
    }
    if (mod) {
      scan_len = 0;
      switch (fmt_str[fmt_pos]) {
        case 'a':
#define RHR_PARSE_a if (pos + 3 > len) {\
            return Qnil;\
          }\
          for(i = 0; i < 7; i++) {\
            if(strncasecmp(str + pos, rhrd__abbr_day_names[i], 3) == 0) {\
              wday = i;\
              break;\
            }\
          }\
          if (i >= 7) {\
            return Qnil;\
          }\
          scan_len = 3;\
          state |= RHRR_WDAY_SET;
          RHR_PARSE_a
          break;
        case 'A':
          for(i = 0; i < 7; i++) {
            scan_len = strlen(rhrd__day_names[i]);
            if (pos + scan_len <= len) {
              if(strncasecmp(str + pos, rhrd__day_names[i], scan_len) == 0) {
                wday = i;
                break;
              }
            }
          }
          if (i >= 7) {
            return Qnil;
          }
          state |= RHRR_WDAY_SET;
          break;
        case 'h':
        case 'b':
#define RHR_PARSE_b if (pos + 3 > len) {\
            return Qnil;\
          }\
          for(i = 1; i < 13; i++) {\
            if(strncasecmp(str + pos, rhrd__abbr_month_names[i], 3) == 0) {\
              month = i;\
              break;\
            }\
          }\
          if (i >= 13) {\
            return Qnil;\
          }\
          scan_len = 3;\
          state |= RHRR_MONTH_SET;
          RHR_PARSE_b
          break;
        case 'B':
          for(i = 1; i < 13; i++) {
            scan_len = strlen(rhrd__month_names[i]);
            if (pos + scan_len <= len) {
              if(strncasecmp(str + pos, rhrd__month_names[i], scan_len) == 0) {
                month = i;
                break;
              }
            }
          }
          if (i >= 13) {
            return Qnil;
          }
          state |= RHRR_MONTH_SET;
          break;
        case 'C':
          if (sscanf(str + pos, "%2ld%n", &century, &scan_len) != 1) {
            return Qnil;
          }
          state |= RHRR_CENTURY_SET;
          break;
        case 'e':
        case 'd':
#define RHR_PARSE_d if (sscanf(str + pos, "%02ld%n", &day, &scan_len) != 1) {\
            return Qnil;\
          }\
          if (day < 1 || day > 31) {\
            return Qnil;\
          }\
          state |= RHRR_DAY_SET;
          RHR_PARSE_d
          break;
        case 'g':
          if (sscanf(str + pos, "%02ld%n", &cwyear, &scan_len) != 1) {
            return Qnil;
          }
          if (!(state & RHRR_CENTURY_SET)) {
            century = cwyear < 69 ? 20 : 19;
          }
          state |= RHRR_CWYEAR_SET | RHRR_CENTURY_SET;
          break;
        case 'G':
          if (sscanf(str + pos, "%ld%n", &cwyear, &scan_len) != 1) {
            return Qnil;
          }
          state |= RHRR_CWYEAR_SET + RHRR_CENTURY_SET;
          break;
        case 'k':
        case 'H':
#define RHR_PARSE_H if (sscanf(str + pos, "%02ld%n", &hour, &scan_len) != 1) {\
            return Qnil;\
          }\
          if (hour < 0 || hour > 24) {\
            return Qnil;\
          }\
          state |= RHRR_HOUR_SET;
          RHR_PARSE_H
          break;
        case 'l':
        case 'I':
#define RHR_PARSE_I if (sscanf(str + pos, "%02ld%n", &hour, &scan_len) != 1) {\
            return Qnil;\
          }\
          if (hour < 1 || hour > 12) {\
            return Qnil;\
          }\
          state |= RHRR_HOUR_SET;
          RHR_PARSE_I
          break;
        case 'j':
          if (sscanf(str + pos, "%03ld%n", &yday, &scan_len) != 1) {
            return Qnil;
          }
          if (yday < 1 || yday > 366) {
            return Qnil;
          }
          state |= RHRR_YDAY_SET;
          break;
        case 'L':
          if (sscanf(str + pos, "%03ld%n", &sec_fraction_num, &scan_len) != 1) {
            return Qnil;
          }
          sec_fraction = sec_fraction_num/pow(10, scan_len);
          state |= RHRR_SEC_FRACTION_SET;
          break;
        case 'm':
#define RHR_PARSE_m if (sscanf(str + pos, "%02ld%n", &month, &scan_len) != 1) {\
            return Qnil;\
          }\
          if (month < 1 || month > 12) {\
            return Qnil;\
          }\
          state |= RHRR_MONTH_SET;
          RHR_PARSE_m
          break;
        case 'M':
#define RHR_PARSE_M if (sscanf(str + pos, "%02ld%n", &minute, &scan_len) != 1) {\
            return Qnil;\
          }\
          if (minute < 0 || minute > 59) {\
            return Qnil;\
          }\
          state |= RHRR_MINUTE_SET;
          RHR_PARSE_M
          break;
        case 'n':
          if (str[pos] != '\n') {
            return Qnil;
          }
          pos++;
          break;
        case 'N':
          if (sscanf(str + pos, "%09ld%n", &sec_fraction_num, &scan_len) != 1) {
            return Qnil;
          }
          sec_fraction = sec_fraction_num/pow(10, scan_len);
          state |= RHRR_SEC_FRACTION_SET;
          break;
        case 'P':
        case 'p':
#define RHR_PARSE_p if (!(str[pos] == 'a' || str[pos] == 'A' ||\
                str[pos] == 'p' || str[pos] == 'P')) {\
            return Qnil;\
          } else {\
            state |= RHRR_MERIDIAN_SET;\
            meridian = str[pos] == 'p' || str[pos] == 'P';\
          }\
          if (pos + 2 <= len) {\
            if (!(str[pos + 1] == 'M' || str[pos + 1] == 'm')) {\
              if (pos + 4 <= len) {\
                if (!((str[pos + 2] == 'M' || str[pos + 2] == 'm') &&\
                      str[pos + 1] == '.' && str[pos + 3] == '.')) {\
                  return Qnil;\
                }\
              } else {\
                return Qnil;\
              }\
            }\
          } else {\
            return Qnil;\
          }
          RHR_PARSE_p
          break;
        case 'Q':
          if (sscanf(str + pos, "%ld%n", &seconds, &scan_len) != 1) {
            return Qnil;
          }
          seconds /= 1000;
          state |= RHRR_UNIX_SET;
          break;
        case 's':
          if (sscanf(str + pos, "%ld%n", &seconds, &scan_len) != 1) {
            return Qnil;
          }
          state |= RHRR_UNIX_SET;
          break;
        case 'S':
#define RHR_PARSE_S if (sscanf(str + pos, "%02ld%n", &second, &scan_len) != 1) {\
            return Qnil;\
          }\
          if (second < 0 || second > 59) {\
            return Qnil;\
          }\
          state |= RHRR_SECOND_SET;
          RHR_PARSE_S
          break;
        case 't':
          if (str[pos] != '\t') {
            return Qnil;
          }
          pos++;
          break;
        case 'u':
          if (sscanf(str + pos, "%02ld%n", &cwday, &scan_len) != 1) {
            return Qnil;
          }
          if (cwday < 1 || cwday > 7) {
            return Qnil;
          }
          state |= RHRR_CWDAY_SET;
          break;
        case 'U':
          if (sscanf(str + pos, "%02ld%n", &wnum0, &scan_len) != 1) {
            return Qnil;
          }
          if (wnum0 < 0 || wnum0 > 53) {
            return Qnil;
          }
          state |= RHRR_WNUM0_SET;
          break;
        case 'V':
          if (sscanf(str + pos, "%02ld%n", &cweek, &scan_len) != 1) {
            return Qnil;
          }
          if (cweek < 1 || cweek > 53) {
            return Qnil;
          }
          state |= RHRR_CWEEK_SET;
          break;
        case 'w':
          if (sscanf(str + pos, "%02ld%n", &wday, &scan_len) != 1) {
            return Qnil;
          }
          if (wday < 0 || wday > 6) {
            return Qnil;
          }
          state |= RHRR_WDAY_SET;
          break;
        case 'W':
          if (sscanf(str + pos, "%02ld%n", &wnum1, &scan_len) != 1) {
            return Qnil;
          }
          if (wnum1 < 0 || wnum1 > 53) {
            return Qnil;
          }
          state |= RHRR_WNUM1_SET;
          break;
        case 'y':
#define RHR_PARSE_y if (sscanf(str + pos, "%02ld%n", &year, &scan_len) != 1) {\
            return Qnil;\
          }\
          if (!(state & RHRR_CENTURY_SET)) {\
            century = year < 69 ? 20 : 19;\
          }\
          state |= RHRR_YEAR_SET | RHRR_CENTURY_SET;
          RHR_PARSE_y
          break;
        case 'Y':
#define RHR_PARSE_Y if (sscanf(str + pos, "%ld%n", &year, &scan_len) != 1) {\
            return Qnil;\
          }\
          state |= RHRR_YEAR_SET + RHRR_CENTURY_SET;
          RHR_PARSE_Y
          break;
        case 'z':
        case 'Z':
#define RHR_PARSE_Z zone = rb_funcall(rhrd_zone_re, rhrd_id_match, 1, rb_funcall(rstr, rhrd_id_slice, 2, LONG2NUM(pos), LONG2NUM(len)));\
          if (RTEST(zone)) {\
            zone = rb_funcall(zone, rhrd_id_op_array, 1, LONG2NUM(0));\
            scan_len = NUM2LONG(rb_funcall(zone, rhrd_id_length, 0));\
          } else {\
            return Qnil;\
          }
          RHR_PARSE_Z
          break;
        /* Composite formats */
#define RHR_PARSE_sep(x) pos += scan_len;\
          scan_len = 0;\
          if (str[pos] != x) {\
            return Qnil;\
          }\
          pos++;
        case 'c':
          RHR_PARSE_a
          RHR_PARSE_sep(' ')
          RHR_PARSE_b
          RHR_PARSE_sep(' ')
          RHR_PARSE_d
          RHR_PARSE_sep(' ')
          RHR_PARSE_H
          RHR_PARSE_sep(':')
          RHR_PARSE_M
          RHR_PARSE_sep(':')
          RHR_PARSE_S
          RHR_PARSE_sep(' ')
          RHR_PARSE_Y
          break;
        case 'x':
        case 'D':
          RHR_PARSE_m
          RHR_PARSE_sep('/')
          RHR_PARSE_d
          RHR_PARSE_sep('/')
          RHR_PARSE_y
          break;
        case 'F':
          RHR_PARSE_Y
          RHR_PARSE_sep('-')
          RHR_PARSE_m
          RHR_PARSE_sep('-')
          RHR_PARSE_d
          break;
        case 'r':
          RHR_PARSE_I
          RHR_PARSE_sep(':')
          RHR_PARSE_M
          RHR_PARSE_sep(':')
          RHR_PARSE_S
          RHR_PARSE_sep(' ')
          RHR_PARSE_p
          break;
        case 'R':
          RHR_PARSE_H
          RHR_PARSE_sep(':')
          RHR_PARSE_M
          break;
        case 'X':
        case 'T':
          RHR_PARSE_H
          RHR_PARSE_sep(':')
          RHR_PARSE_M
          RHR_PARSE_sep(':')
          RHR_PARSE_S
          break;
        case 'v':
          RHR_PARSE_d
          RHR_PARSE_sep('-')
          RHR_PARSE_b
          RHR_PARSE_sep('-')
          RHR_PARSE_Y
          break;
        case '+':
          RHR_PARSE_a
          RHR_PARSE_sep(' ')
          RHR_PARSE_b
          RHR_PARSE_sep(' ')
          RHR_PARSE_d
          RHR_PARSE_sep(' ')
          RHR_PARSE_H
          RHR_PARSE_sep(':')
          RHR_PARSE_M
          RHR_PARSE_sep(':')
          RHR_PARSE_S
          RHR_PARSE_sep(' ')
          RHR_PARSE_Z
          RHR_PARSE_sep(' ')
          RHR_PARSE_Y
          break;
        default:
          if (str[pos] != fmt_str[fmt_pos]) {
            return Qnil;
          }
          pos++;
          break;
      }
      pos += scan_len;
      mod = 0;
    } else if (fmt_str[fmt_pos] == '%') {
      mod = 1;
    } else {
      pos++;
    }
  }

  hash = rb_hash_new();
  if(state & RHRR_YEAR_SET) {
    if (state & RHRR_CENTURY_SET && year < 100) {
      year += century * 100;
    }
    rb_hash_aset(hash, rhrd_sym_year, LONG2NUM(year));
  } 
  if(state & RHRR_MONTH_SET) {
    rb_hash_aset(hash, rhrd_sym_mon, LONG2NUM(month));
  } 
  if(state & RHRR_DAY_SET) {
    rb_hash_aset(hash, rhrd_sym_mday, LONG2NUM(day));
  } 
  if(state & RHRR_YDAY_SET) {
    rb_hash_aset(hash, rhrd_sym_yday, LONG2NUM(yday));
  } 
  if(state & RHRR_WDAY_SET) {
    rb_hash_aset(hash, rhrd_sym_wday, LONG2NUM(wday));
  } 
  if(state & RHRR_CWYEAR_SET) {
    if (state & RHRR_CENTURY_SET && cwyear < 100) {
      cwyear += century * 100;
    }
    rb_hash_aset(hash, rhrd_sym_cwyear, LONG2NUM(cwyear));
  } 
  if(state & RHRR_CWEEK_SET) {
    rb_hash_aset(hash, rhrd_sym_cweek, LONG2NUM(cweek));
  } 
  if(state & RHRR_CWDAY_SET) {
    rb_hash_aset(hash, rhrd_sym_cwday, LONG2NUM(cwday));
  } 
  if(state & RHRR_HOUR_SET) {
    if (state & RHRR_MERIDIAN_SET) {
      if (meridian) {
        if (hour < 12) {
          hour += 12;
        }
      }
      else if (hour == 12) {
        hour = 0;
      }
    }
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
  if(state & RHRR_UNIX_SET) {
    rb_hash_aset(hash, rhrd_sym_seconds, LONG2NUM(seconds));
  } 
  if(RTEST(zone)) {
    rb_hash_aset(hash, rhrd_sym_zone, zone);
    zone = rhrd_s_zone_to_diff(rstr, zone);
    if(RTEST(zone)) {
      rb_hash_aset(hash, rhrd_sym_offset, zone);
    }
  } 
  if(state & RHRR_WNUM0_SET) {
    rb_hash_aset(hash, rhrd_sym_wnum0, LONG2NUM(wnum0));
  } 
  if(state & RHRR_WNUM1_SET) {
    rb_hash_aset(hash, rhrd_sym_wnum1, LONG2NUM(wnum1));
  } 
#ifdef RUBY19  
  if(pos < len) {
    rb_hash_aset(hash, rhrd_sym_leftover, rb_str_new(str + pos, len - pos));
  }
#endif
  return hash;
}

#include "date_parser.c"

/* Ruby Class Methods */

/* call-seq:
 *   _load(string) -> Date
 *
 * Unmarshal a dumped +Date+ object. Note that this does not handle 
 * the marshalling format used by the stdlib's +Date+, it only handles
 * marshalled versions of this library's +Date+ objects.
 */
static VALUE rhrd_s__load(VALUE klass, VALUE string) {
  rhrd_t * d;
  VALUE jd, rd;
  jd = rb_marshal_load(string);
  rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
  d->jd = NUM2LONG(jd);
  RHR_CHECK_JD(d)
  d->flags = RHR_HAVE_JD;
  return rd;
}


/* call-seq:
 *   _ragel_parse(string) -> Hash
 *
 * Attemps to parse the string with Ragel, returning a hash
 * if there is a match (or +nil+ if no match), similar to +_parse+.
 */
static VALUE rhrd_s__ragel_parse(VALUE klass, VALUE s) {
  s = rb_str_to_str(s);
  return rhrd__ragel_parse(RSTRING_PTR(s), RSTRING_LEN(s));
}

/* call-seq:
 *   _strptime(string, format='%F') -> Hash
 *
 * Attemps to parse the string using the given format, returning
 * a hash if there is a match (or +nil+ if no match), similar to +_parse+.
 *
 * +_strptime+ supports the same formats that <tt>Date#strftime</tt> does.
 */
static VALUE rhrd_s__strptime(int argc, VALUE *argv, VALUE klass) {
  char * fmt_str = "%F";
  long fmt_len = 2;
  VALUE r;

  switch(argc) {
    case 2:
      r = rb_str_to_str(argv[1]);
      fmt_str = RSTRING_PTR(r);
      fmt_len = RSTRING_LEN(r);
    case 1:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }

  return rhrd__strptime(argv[0], fmt_str, fmt_len);
}

/* call-seq:
 *   civil() -> Date <br />
 *   civil(year, month=1, day=1, sg=nil) -> Date
 *
 * If no arguments are given, returns a +Date+ for julian day 0,
 * Otherwise, returns a +Date+ for the year, month, and day given.
 * Ignores the 4th argument.
 */
static VALUE rhrd_s_civil(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  long year;
  long month = 1;
  long day = 1;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);

  switch(argc) {
    case 3:
    case 4:
      day = NUM2LONG(argv[2]);
    case 2:
      month = NUM2LONG(argv[1]);
    case 1:
      year = NUM2LONG(argv[0]);
      break;
    case 0:
      d->flags = RHR_HAVE_JD;
      return rd;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 4", argc);
      break;
  }

  if (!rhrd__valid_civil(d, year, month, day)) {
    RHR_CHECK_CIVIL(d)
    rb_raise(rb_eArgError, "invalid date (year: %li, month: %li, day: %li)", year, month, day);
  }
  return rd;
}

/* call-seq:
 *   commercial() -> Date <br />
 *   commercial(cwyear, cweek=41, cwday=5, sg=nil) -> Date [ruby 1-8] <br />
 *   commercial(cwyear, cweek=1, cwday=1, sg=nil) -> Date [ruby 1-9]
 *
 * If no arguments are given:
 * * ruby 1.8: returns a +Date+ for 1582-10-15 (the Day of Calendar Reform in Italy)
 * * ruby 1.9: returns a +Date+ for julian day 0
 *
 * Otherwise, returns a +Date+ for the commercial week year, commercial week, and
 * commercial week day given. Ignores the 4th argument.
 */
static VALUE rhrd_s_commercial(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  long cwyear = RHR_DEFAULT_CWYEAR;
  long cweek = RHR_DEFAULT_CWEEK;
  long cwday = RHR_DEFAULT_CWDAY;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);

  switch(argc) {
    case 3:
    case 4:
      cwday = NUM2LONG(argv[2]);
    case 2:
      cweek = NUM2LONG(argv[1]);
    case 1:
      cwyear = NUM2LONG(argv[0]);
#ifdef RUBY19
      break;
    case 0:
      d->flags = RHR_HAVE_JD;
      return rd;
#else
    case 0:
      break;
#endif
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 4", argc);
      break;
  }

  if(!rhrd__valid_commercial(d, cwyear, cweek, cwday)) {
    RHR_CHECK_JD(d)
    rb_raise(rb_eArgError, "invalid date (cwyear: %li, cweek: %li, cwday: %li)", cwyear, cweek, cwday);
  }
  return rd;
}

/* call-seq:
 *   gregorian_leap?(year) -> true or false
 *
 * Returns true if the given year is a leap year in the Gregorian
 * calendar, or false if not.
 */
static VALUE rhrd_s_gregorian_leap_q(VALUE klass, VALUE year) {
  return rhrd__leap_year(NUM2LONG(year)) ? Qtrue : Qfalse;
}

/* call-seq:
 *   jd(jd=0, sg=nil) -> Date
 *
 * Returns a +Date+ for the julian day number given.
 * Ignores the 2nd argument.
 */
static VALUE rhrd_s_jd (int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      d->jd = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 2", argc);
      break;
  }
  RHR_CHECK_JD(d)
  d->flags = RHR_HAVE_JD;
  return rd;
}

/* call-seq:
 *   julian_leap?(year) -> true or false
 *
 * Returns true if the given year is a leap year in the Julian
 * calendar (i.e. divisible by 4), or false if not.
 */
static VALUE rhrd_s_julian_leap_q(VALUE klass, VALUE y) {
  return NUM2LONG(y) % 4 == 0 ? Qtrue : Qfalse;
}

/* call-seq:
 *   new!(jd=0, offset=nil, sg=nil) -> Date
 *
 * Returns a +Date+ for the julian day number given.
 * Ignores the 2nd and 3rd arguments.
 */
static VALUE rhrd_s_new_b(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
    case 3:
      d->jd = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }
  RHR_CHECK_JD(d)
  d->flags = RHR_HAVE_JD;
  return rd;
}

static VALUE rhrd_s_ordinal(int argc, VALUE *argv, VALUE klass) {
  long year;
  long day = 1;
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);

  switch(argc) {
    case 2:
    case 3:
      day = NUM2LONG(argv[1]);
    case 1:
      year = NUM2LONG(argv[0]);
      if(!rhrd__valid_ordinal(d, year, day)) {
        RHR_CHECK_JD(d)
        rb_raise(rb_eArgError, "invalid date (year: %li, yday: %li)", year, day);
      }
      break;
    case 0:
      d->flags = RHR_HAVE_JD;
      return rd;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }

  return rd;
}

static VALUE rhrd_s_parse(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
      return rhrd__from_hash(rb_funcall(klass, rhrd_id__parse, 1, argv[0]));
    case 2:
    case 3:
      return rhrd__from_hash(rb_funcall(klass, rhrd_id__parse, 2, argv[0], argv[1]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 3)", argc);
      break;
  }
}

static VALUE rhrd_s_strptime(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      break;
    case 3:
      argc = 2;
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 3)", argc);
      break;
  }

  rd = rhrd_s__strptime(argc, argv, klass);
  if (RTEST(rd)) {
    return rhrd__from_hash(rd);
  }
  rb_raise(rb_eArgError, "invalid date");
}

static VALUE rhrd_s_today(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);

  switch(argc) {
    case 0:
    case 1:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  rhrd__today(d);
  return rd;
}

static VALUE rhrd_s_valid_civil_q(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 3:
    case 4:
      if (!rhrd__valid_civil(&d, NUM2LONG(argv[0]), NUM2LONG(argv[1]), NUM2LONG(argv[2]))) {
#ifdef RUBY19
        return Qfalse;
#else
        return Qnil;
#endif
      }
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 4", argc);
      break;
  }

#ifdef RUBY19
  return Qtrue;
#else
  RHR_FILL_JD(&d)
  return LONG2NUM(d.jd);
#endif
}

static VALUE rhrd_s_valid_commercial_q(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 3:
    case 4:
      if (!rhrd__valid_commercial(&d, NUM2LONG(argv[0]), NUM2LONG(argv[1]), NUM2LONG(argv[2]))) {
#ifdef RUBY19
        return Qfalse;
#else
        return Qnil;
#endif
      }
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 4", argc);
      break;
  }

#ifdef RUBY19
  return Qtrue;
#else
  return LONG2NUM(d.jd);
#endif
}

static VALUE rhrd_s_valid_jd_q(int argc, VALUE *argv, VALUE klass) {
  switch(argc) {
    case 1:
    case 2:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 2", argc);
      break;
  }

#ifdef RUBY19
  return Qtrue;
#else
  return argv[0];
#endif
}

static VALUE rhrd_s_valid_ordinal_q(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 2:
    case 3:
      if (!rhrd__valid_ordinal(&d, NUM2LONG(argv[0]), NUM2LONG(argv[1]))) {
#ifdef RUBY19
        return Qfalse;
#else
        return Qnil;
#endif
      }
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }

#ifdef RUBY19
  return Qtrue;
#else
  RHR_FILL_JD(&d)
  return LONG2NUM(d.jd);
#endif
}

static VALUE rhrd_s_zone_to_diff(VALUE klass, VALUE str) {
  long offset = 0;
  long len, i, j;
  char *s;
  VALUE v, e;

  str = rb_funcall(str, rhrd_id_downcase, 0);
  if(RTEST(rb_funcall(str, rhrd_id_sub_b, 2, rhrd_zone_dst_re, rhrd_empty_string))) {
    if (!RTEST(rb_reg_nth_match(1, rb_gv_get("$~")))) { 
      offset += 3600;
    }
  }

  if(RTEST(v = rb_hash_aref(rhrd_zones_hash, str))) {
    return LONG2NUM(offset + NUM2LONG(v));
  }

  if(RTEST(rb_funcall(str, rhrd_id_sub_b, 2, rhrd_zone_sign_re, rhrd_empty_string))) {
    if (RTEST(rb_reg_nth_match(1, rb_gv_get("$~")))) { 
      offset = -1;
    } else {
      offset = 1;
    }
    str = rb_str_to_str(str);
    s = RSTRING_PTR(str);
    len = RSTRING_LEN(str);
    for(i=0; i < len; i++) {
      if(s[i] == ':') {
        v = rb_funcall(str, rhrd_id_split, 1, rhrd_string_colon);
        return LONG2NUM((NUM2LONG(rb_funcall(rb_ary_entry(v, 0), rhrd_id_to_i, 0)) * 3600
               + NUM2LONG(rb_funcall(rb_ary_entry(v, 1), rhrd_id_to_i, 0)) * 60
               + NUM2LONG(rb_funcall(rb_ary_entry(v, 2), rhrd_id_to_i, 0))) * offset);
      }
    }
    for(i=0; i < len; i++) {
      if((s[i] == ',') || (s[i] == '.')) {
        v = rb_funcall(str, rhrd_id_split, 1, rhrd_re_comma_period);
        e = rb_funcall(rb_ary_entry(v, 1), rhrd_id_to_i, 0); 
        return LONG2NUM((NUM2LONG(rb_funcall(rb_ary_entry(v, 0), rhrd_id_to_i, 0)) * 3600
               + NUM2LONG(e * 3600) / (long)pow(10, RSTRING_LEN(e)) * offset));
      }
    }
    switch (len) {
      case 0:
        return LONG2NUM(0);
      case 1:
      case 2:
        return LONG2NUM(atol(s) * 3600 * offset);
      case 3:
        i = atol(s + 1);
        s[1] = '\0';
        len = atol(s);
        return LONG2NUM((len * 3600 + i * 60) * offset);
      case 4:
        i = atol(s + 2);
        s[2] = '\0';
        len = atol(s);
        return LONG2NUM((len * 3600 + i * 60) * offset);
      default:
        s[6] = '\0';
      case 6:
      case 5:
        j = atol(s + 4);
        s[4] = '\0';
        i = atol(s + 2);
        s[2] = '\0';
        len = atol(s);
        return LONG2NUM((len * 3600 + i * 60 + j) * offset);
    }
  }

  return LONG2NUM(0);
}

/* Ruby Instance Methods */

static VALUE rhrd__dump(VALUE self, VALUE limit) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return rb_marshal_dump(LONG2NUM(d->jd), LONG2NUM(NUM2LONG(limit) - 1));
}

static VALUE rhrd_asctime(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s %s %2hhi 00:00:00 %04li", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        rhrd__abbr_month_names[d->month],
        d->day,
        d->year);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#asctime (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrd_cwday(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return LONG2NUM(n.day);
}

static VALUE rhrd_cweek(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return LONG2NUM(n.month);
}

static VALUE rhrd_cwyear(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return LONG2NUM(n.year);
}

static VALUE rhrd_day(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return LONG2NUM(d->day);
}

static VALUE rhrd_day_fraction(VALUE self) {
  return LONG2NUM(0);
}

static VALUE rhrd_downto(VALUE self, VALUE other) {
  VALUE argv[2];
  argv[0] = other;
  argv[1] = INT2FIX(-1);
  return rhrd_step(2, argv, self);
}

static VALUE rhrd_eql_q(VALUE self, VALUE other) {
  rhrd_t *d, *o;
  rhrdt_t *odt;
  long diff;
  Data_Get_Struct(self, rhrd_t, d);

  if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
    Data_Get_Struct(other, rhrdt_t, odt);
    RHR_FILL_JD(d)
    RHRDT_FILL_JD(odt)
    RHR_SPACE_SHIP(diff, d->jd, odt->jd)
    if (diff == 0) {
      RHRDT_FILL_NANOS(odt)
      RHR_SPACE_SHIP(diff, 0, odt->nanos)
    }
    return diff == 0 ? Qtrue : Qfalse;
  } else if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(other, rhrd_t, o);
    return rhrd__spaceship(d, o) == 0 ? Qtrue : Qfalse;
  }
  return Qfalse;
}

static VALUE rhrd_gregorian(VALUE self) {
  return self;
}

static VALUE rhrd_gregorian_q(VALUE self) {
  return Qtrue;
}

static VALUE rhrd_hash(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return rb_funcall(LONG2NUM(d->jd), rhrd_id_hash, 0);
}

static VALUE rhrd_inspect(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "#<Date %04li-%02hhi-%02hhi>",
        d->year, d->month, d->day);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#inspect (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrd_jd(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return LONG2NUM(d->jd);
}

static VALUE rhrd_julian_q(VALUE self) {
  return Qfalse;
}

static VALUE rhrd_ld(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return LONG2NUM(d->jd - RHR_JD_LD);
}

static VALUE rhrd_leap_q(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return rhrd__leap_year(d->year) ? Qtrue : Qfalse;
}

static VALUE rhrd_mjd(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return LONG2NUM(d->jd - RHR_JD_MJD);
}

static VALUE rhrd_month(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return LONG2NUM(d->month);
}

static VALUE rhrd_next(VALUE self) {
   return rhrd__add_days(self, 1);
}

static VALUE rhrd_new_start(int argc, VALUE *argv, VALUE self) {
  switch(argc) {
    case 0:
    case 1:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  return self;
}

static VALUE rhrd_start(VALUE self) {
  return LONG2NUM(RHR_JD_MIN);
}

static VALUE rhrd_step(int argc, VALUE *argv, VALUE self) {
  rhrd_t *d, *n, *newd;
  rhrdt_t *ndt;
  long step, limit, current;
  VALUE rlimit, new;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)

  rb_need_block();
  switch(argc) {
    case 1:
      step = 1;
      break;
    case 2:
      step = NUM2LONG(argv[1]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 2", argc);
      break;
  }

  rlimit = argv[0];
  if (RTEST(rb_obj_is_kind_of(rlimit, rb_cNumeric))) {
    limit = NUM2LONG(rlimit);
  } else if (RTEST((rb_obj_is_kind_of(rlimit, rhrdt_class)))) {
    Data_Get_Struct(rlimit, rhrdt_t, ndt);
    RHRDT_FILL_JD(ndt)
    limit = ndt->jd;
  } else if (RTEST((rb_obj_is_kind_of(rlimit, rhrd_class)))) {
    Data_Get_Struct(rlimit, rhrd_t, n);
    RHR_FILL_JD(n)
    limit = n->jd;
  } else {
    rb_raise(rb_eTypeError, "expected numeric or date");
  }

  current = d->jd;
  if (limit > current) {
    if (step > 0) {
      while(limit >= current) {
        new = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, newd);
        newd->jd = current;
        RHR_CHECK_JD(newd)
        newd->flags = RHR_HAVE_JD;
        current += step;
        rb_yield(new);
      }
    }
  } else if (limit < current) {
    if (step < 0) {
      while(limit <= current) {
        new = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, newd);
        newd->jd = current;
        RHR_CHECK_JD(newd)
        newd->flags = RHR_HAVE_JD;
        current += step;
        rb_yield(new);
      }
    }
  } else {
    rb_yield(self);
  }

  return self;
}

static VALUE rhrd_strftime(int argc, VALUE *argv, VALUE self) {
  rhrd_t *d;
  rhrdt_t dt;
  VALUE r;

  switch(argc) {
    case 0:
      return rhrd_to_s(self);
    case 1:
      r = rb_str_to_str(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)
  memset(&dt, 0, sizeof(rhrdt_t));
  dt.jd = d->jd;
  dt.year = d->year;
  dt.month = d->month;
  dt.day = d->day;
  dt.flags = RHR_HAVE_CIVIL | RHR_HAVE_JD;
  return rhrd__strftime(&dt, RSTRING_PTR(r), RSTRING_LEN(r));
}

static VALUE rhrd_to_s(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%04li-%02hhi-%02hhi",
        d->year, d->month, d->day);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#to_s (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrd_upto(VALUE self, VALUE other) {
  VALUE argv[1];
  argv[0] = other;
  return rhrd_step(1, argv, self);
}

static VALUE rhrd_wday(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return LONG2NUM(rhrd__jd_to_wday(d->jd));
}

static VALUE rhrd_yday(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return LONG2NUM(rhrd__ordinal_day(d->year, d->month, d->day));
}

static VALUE rhrd_year(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return LONG2NUM(d->year);
}

/* Ruby Instance Operator Methods */

static VALUE rhrd_op_right_shift(VALUE self, VALUE other) {
  return rhrd__add_months(self, NUM2LONG(other));
}

static VALUE rhrd_op_left_shift(VALUE self, VALUE other) {
  return rhrd__add_months(self, -NUM2LONG(other));
}

static VALUE rhrd_op_plus(VALUE self, VALUE other) {
   return rhrd__add_days(self, NUM2LONG(other));
}

static VALUE rhrd_op_minus(VALUE self, VALUE other) {
  rhrd_t *d;
  rhrd_t *newd;
  rhrdt_t *newdt;
  Data_Get_Struct(self, rhrd_t, d);

  if (RTEST(rb_obj_is_kind_of(other, rb_cNumeric))) {
    return rhrd__add_days(self, -NUM2LONG(other));
  }
  if (RTEST((rb_obj_is_kind_of(other, rhrdt_class)))) {
    Data_Get_Struct(other, rhrdt_t, newdt);
    RHR_FILL_JD(d)
    RHRDT_FILL_JD(newdt)
    RHRDT_FILL_NANOS(newdt)
    return rb_float_new(d->jd - (newdt->jd + newdt->nanos/RHR_NANOS_PER_DAYD));
  }
  if (RTEST((rb_obj_is_kind_of(other, rhrd_class)))) {
    Data_Get_Struct(other, rhrd_t, newd);
    RHR_FILL_JD(d)
    RHR_FILL_JD(newd)
    return LONG2NUM(rhrd__safe_add_long(d->jd, -newd->jd));
  }
  rb_raise(rb_eTypeError, "expected numeric or date");
}

static VALUE rhrd_op_relationship(VALUE self, VALUE other) {
  rhrd_t *d, *o;
  rhrdt_t *odt;
  long diff = 1;
  Data_Get_Struct(self, rhrd_t, d);

  if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
    Data_Get_Struct(other, rhrdt_t, odt);
    RHR_FILL_JD(d)
    RHRDT_FILL_JD(odt)
    diff = d->jd != odt->jd;
  } else if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(other, rhrd_t, o);
    diff = rhrd__spaceship(d, o);
  } else if (RTEST((rb_obj_is_kind_of(other, rb_cNumeric)))) {
    diff = NUM2LONG(other);
    RHR_FILL_JD(d)
    RHR_SPACE_SHIP(diff, d->jd, diff)
  }
  return diff == 0 ? Qtrue : Qfalse;
}

static VALUE rhrd_op_spaceship(VALUE self, VALUE other) {
  rhrd_t *d, *o;
  rhrdt_t *odt;
  long diff;
  Data_Get_Struct(self, rhrd_t, d);

  if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
    Data_Get_Struct(other, rhrdt_t, odt);
    RHR_FILL_JD(d)
    RHRDT_FILL_JD(odt)
    RHR_SPACE_SHIP(diff, d->jd, odt->jd)
    if (diff == 0) {
      RHRDT_FILL_NANOS(odt)
      RHR_SPACE_SHIP(diff, 0, odt->nanos)
    }
    return LONG2NUM(diff);
  } else if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(other, rhrd_t, o);
    return LONG2NUM(rhrd__spaceship(d, o));
  } else if (RTEST((rb_obj_is_kind_of(other, rb_cNumeric)))) {
    diff = NUM2LONG(other);
    RHR_FILL_JD(d)
    RHR_SPACE_SHIP(diff, d->jd, diff)
    return LONG2NUM(diff);
  }
  return Qnil;
}

#ifdef RUBY19

/* 1.9 helper methods */

VALUE rhrd__add_years(VALUE self, long n) {
  rhrd_t *d;
  rhrd_t *newd;
  VALUE new;
  Data_Get_Struct(self, rhrd_t, d);

  new = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, newd);
  RHR_FILL_CIVIL(d)
  newd->year = rhrd__safe_add_long(n, d->year);
  newd->month = d->month;
  if(d->month == 2 && d->day == 29) {
    newd->day = rhrd__leap_year(newd->year) ? 29 : 28;
  } else {
    newd->day = d->day;
  }

  RHR_CHECK_CIVIL(newd)
  newd->flags = RHR_HAVE_CIVIL;
  return new;
}

VALUE rhrd__day_q(VALUE self, long day) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return rhrd__jd_to_wday(d->jd) == day ? Qtrue : Qfalse;
}

/* 1.9 class methods */

static VALUE rhrd_s_httpdate(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(rb_funcall(klass, rhrd_id__httpdate, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrd_s_iso8601(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(rb_funcall(klass, rhrd_id__iso8601, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrd_s_jisx0301(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(rb_funcall(klass, rhrd_id__jisx0301, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrd_s_rfc2822(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(rb_funcall(klass, rhrd_id__rfc2822, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrd_s_rfc3339(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(rb_funcall(klass, rhrd_id__rfc3339, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrd_s_xmlschema(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(rb_funcall(klass, rhrd_id__xmlschema, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

/* 1.9 instance methods */

static VALUE rhrd_httpdate(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s, %02hhi %s %04li 00:00:00 GMT", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        d->day,
        rhrd__abbr_month_names[d->month],
        d->year);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#httpdate (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrd_jisx0301(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  char c;
  long year;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)

  s = rb_str_buf_new(128);
  if (d->jd < 2405160) {
    len = snprintf(RSTRING_PTR(s), 128, "%04li-%02hhi-%02hhi", d->year, d->month, d->day);
  } else {
    if (d->jd >= 2447535) {
      c = 'H';
      year = d->year - 1988;
    } else if (d->jd >= 2424875) {
      c = 'S';
      year = d->year - 1925;
    } else if (d->jd >= 2419614) {
      c = 'T';
      year = d->year - 1911;
    } else {
      c = 'M';
      year = d->year - 1867;
    }
    len = snprintf(RSTRING_PTR(s), 128, "%c%02li.%02hhi.%02hhi", c, year, d->month, d->day);
  }
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#jisx0301 (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrd_next_day(int argc, VALUE *argv, VALUE self) {
  long i;

  switch(argc) {
    case 0:
      i = 1;
      break;
    case 1:
      i = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

   return rhrd__add_days(self, i);
}

static VALUE rhrd_next_month(int argc, VALUE *argv, VALUE self) {
  long i;

  switch(argc) {
    case 0:
      i = 1;
      break;
    case 1:
      i = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  return rhrd__add_months(self, i);
}

static VALUE rhrd_next_year(int argc, VALUE *argv, VALUE self) {
  long i;

  switch(argc) {
    case 0:
      i = 1;
      break;
    case 1:
      i = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  return rhrd__add_years(self, i);
}

static VALUE rhrd_prev_day(int argc, VALUE *argv, VALUE self) {
  long i;

  switch(argc) {
    case 0:
      i = -1;
      break;
    case 1:
      i = -NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

   return rhrd__add_days(self, i);
}

static VALUE rhrd_prev_month(int argc, VALUE *argv, VALUE self) {
  long i;

  switch(argc) {
    case 0:
      i = -1;
      break;
    case 1:
      i = -NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  return rhrd__add_months(self, i);
}

static VALUE rhrd_prev_year(int argc, VALUE *argv, VALUE self) {
  long i;

  switch(argc) {
    case 0:
      i = -1;
      break;
    case 1:
      i = -NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  return rhrd__add_years(self, i);
}

static VALUE rhrd_rfc2822(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s, %hhi %s %04li 00:00:00 +0000", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        d->day,
        rhrd__abbr_month_names[d->month],
        d->year);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#rfc2822 (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrd_rfc3339(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%04li-%02hhi-%02hhiT00:00:00+00:00", d->year, d->month, d->day);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#rfc3339 (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrd_to_datetime(VALUE self) {
  rhrd_t *d;
  rhrdt_t *dt;
  VALUE rdt = Data_Make_Struct(rhrdt_class, rhrdt_t, NULL, free, dt);
  Data_Get_Struct(self, rhrd_t, d);

  if (RHR_HAS_CIVIL(d)) {
    dt->year = d->year;
    dt->month = d->month;
    dt->day = d->day;
    dt->flags |= RHR_HAVE_CIVIL;
  }
  if (RHR_HAS_JD(d)) {
    dt->jd = d->jd;
    dt->flags |= RHR_HAVE_JD;
  }

  return rdt;
}

static VALUE rhrd_to_time(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return rb_funcall(rb_cTime, rhrd_id_local, 3, LONG2NUM(d->year), LONG2NUM(d->month), LONG2NUM(d->day));
}

static VALUE rhrd_time_to_date(VALUE self) {
  rhrd_t *d;
  VALUE rd;
  rd = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, d);
  d->jd = rhrd__unix_to_jd(NUM2LONG(rb_funcall(self, rhrd_id_to_i, 0)) + NUM2LONG(rb_funcall(self, rhrd_id_utc_offset, 0)));
  d->flags = RHR_HAVE_JD;
  RHR_CHECK_JD(d)
  return rd;
}

static VALUE rhrd_time_to_time(VALUE self) {
  return rb_funcall(self, rhrd_id_getlocal, 0);
}

/* 1.9 day? methods */

static VALUE rhrd_sunday_q(VALUE self) {
  return rhrd__day_q(self, 0);
}

static VALUE rhrd_monday_q(VALUE self) {
  return rhrd__day_q(self, 1);
}

static VALUE rhrd_tuesday_q(VALUE self) {
  return rhrd__day_q(self, 2);
}

static VALUE rhrd_wednesday_q(VALUE self) {
  return rhrd__day_q(self, 3);
}

static VALUE rhrd_thursday_q(VALUE self) {
  return rhrd__day_q(self, 4);
}

static VALUE rhrd_friday_q(VALUE self) {
  return rhrd__day_q(self, 5);
}

static VALUE rhrd_saturday_q(VALUE self) {
  return rhrd__day_q(self, 6);
}

#else

/* 1.8 class methods */

static VALUE rhrd_s_ajd_to_amjd(VALUE klass, VALUE ajd) {
  return LONG2NUM(rhrd__safe_add_long(-RHR_JD_MJD, NUM2LONG(ajd)));
}

static VALUE rhrd_s_ajd_to_jd(int argc, VALUE *argv, VALUE klass) {
  switch(argc) {
    case 1:
    case 2:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 2", argc);
      break;
  }

  return rb_ary_new3(2, argv[0], rb_float_new(0.5));
}

static VALUE rhrd_s_amjd_to_ajd(VALUE klass, VALUE amjd) {
  return LONG2NUM(rhrd__safe_add_long(RHR_JD_MJD - 1, NUM2LONG(amjd)));
}

static VALUE rhrd_s_civil_to_jd(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 3:
    case 4:
      d.year = NUM2LONG(argv[0]);
      d.month = (unsigned char)NUM2LONG(argv[1]);
      d.day = (unsigned char)NUM2LONG(argv[2]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 4", argc);
      break;
  }
  d.flags = RHR_HAVE_CIVIL;
  RHR_FILL_JD(&d)

  return LONG2NUM(d.jd);
}

static VALUE rhrd_s_commercial_to_jd(int argc, VALUE *argv, VALUE klass) {
  long jd;

  switch(argc) {
    case 3:
    case 4:
      jd = rhrd__commercial_to_jd(NUM2LONG(argv[0]), NUM2LONG(argv[1]), NUM2LONG(argv[2]));
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 4", argc);
      break;
  }

  return LONG2NUM(jd);
}

static VALUE rhrd_s_day_fraction_to_time(VALUE klass, VALUE rf) {
  double f;
  int h, m, s;
 
  f = NUM2DBL(rf) * 24;
  h = floor(f);
  f = (f - h) * 60;
  m = floor(f);
  f = (f - m) * 60;
  s = floor(f);
  f = (f - s)/86400;
  return rb_ary_new3(4, LONG2NUM(h), LONG2NUM(m), LONG2NUM(s), rb_float_new(f));
}

static VALUE rhrd_s_gregorian_q(VALUE klass, VALUE jd, VALUE sg) {
  if (RTEST((rb_obj_is_kind_of(sg, rb_cNumeric)))) {
    return rb_funcall(jd, rhrd_id_op_gte, 1, sg);
  } else {
    return RTEST(sg) ? Qtrue : Qfalse;
  }
}

static VALUE rhrd_s_jd_to_ajd(int argc, VALUE *argv, VALUE klass) {
  switch(argc) {
    case 2:
    case 3:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }
  return argv[0];
}

static VALUE rhrd_s_jd_to_civil(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 1:
    case 2:
      d.jd = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }
  RHR_FILL_CIVIL(&d)
  return rb_ary_new3(3, LONG2NUM(d.year), LONG2NUM(d.month), LONG2NUM(d.day));
}

static VALUE rhrd_s_jd_to_commercial(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 1:
    case 2:
      d.jd = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }
  rhrd__fill_commercial(&d);
  return rb_ary_new3(3, LONG2NUM(d.year), LONG2NUM(d.month), LONG2NUM(d.day));
}

static VALUE rhrd_s_jd_to_ld(VALUE klass, VALUE jd) {
  return LONG2NUM(rhrd__safe_add_long(-RHR_JD_LD, NUM2LONG(jd)));
}

static VALUE rhrd_s_jd_to_mjd(VALUE klass, VALUE jd) {
  return LONG2NUM(rhrd__safe_add_long(-RHR_JD_MJD, NUM2LONG(jd)));
}

static VALUE rhrd_s_jd_to_ordinal(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 1:
    case 2:
      d.jd = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }
  RHR_FILL_CIVIL(&d)

  return rb_ary_new3(2, LONG2NUM(d.year), LONG2NUM(rhrd__ordinal_day(d.year, d.month, d.day)));
}

static VALUE rhrd_s_jd_to_wday(VALUE klass, VALUE jd) {
  return LONG2NUM(rhrd__jd_to_wday(NUM2LONG(jd)));
}

static VALUE rhrd_s_julian_q(VALUE klass, VALUE jd, VALUE sg) {
  if (RTEST((rb_obj_is_kind_of(sg, rb_cNumeric)))) {
    return rb_funcall(jd, rhrd_id_op_lt, 1, sg);
  } else {
    return RTEST(sg) ? Qfalse : Qtrue;
  }
}

static VALUE rhrd_s_ld_to_jd(VALUE klass, VALUE ld) {
  return LONG2NUM(rhrd__safe_add_long(RHR_JD_LD, NUM2LONG(ld)));
}

static VALUE rhrd_s_mjd_to_jd(VALUE klass, VALUE mjd) {
  return LONG2NUM(rhrd__safe_add_long(RHR_JD_MJD, NUM2LONG(mjd)));
}

static VALUE rhrd_s_ordinal_to_jd(int argc, VALUE *argv, VALUE klass) {
  switch(argc) {
    case 2:
    case 3:
      return LONG2NUM(rhrd__ymd_to_jd(NUM2LONG(argv[0]), 1, NUM2LONG(argv[1])));
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }
}

static VALUE rhrd_s_time_to_day_fraction(VALUE klass, VALUE h, VALUE m, VALUE s) {
  return rb_float_new(NUM2DBL(h)/24.0 + NUM2DBL(m)/1440.0 + NUM2DBL(s)/86400.0);
}

static VALUE rhrd_s_valid_time_q(VALUE klass, VALUE rh, VALUE rm, VALUE rs) {
  long h, m, s;
  h = NUM2LONG(rh);
  m = NUM2LONG(rm);
  s = NUM2LONG(rs);
  if (h < 0) {
    h += 24;
  }
  if (m < 0) {
    m += 60;
  }
  if (s < 0) {
    s += 60;
  }
  if (h < 0 || m < 0 || s < 0 || h > 24 || m > 59 || s > 59 || (h == 24 && m != 0 && s != 0)) {
    return Qnil;
  }
  return rb_float_new(h/24.0 + m/1440.0 + s/86400.0);
}

#endif

#include "datetime.c"

/* Ruby Library Initialization */

/* +Date+ is used to store a single date in the gregorian calendar.
 *
 * In general, +Date+ objects are created by calling one of the class
 * methods: +civil+, +parse+, +strptime+, +today+.  Once created,
 * +Date+ objects are immutable.  Operations that result in a separate
 * date (such as adding a number of days), always return a new +Date+
 * object.
 * */

void Init_date(void) {
  int i;

  rhrd_id_op_array = rb_intern("[]");
  rhrd_id_op_gte = rb_intern(">=");
  rhrd_id_op_lt = rb_intern("<");
  rhrd_id__parse = rb_intern("_parse");
  rhrd_id_downcase = rb_intern("downcase");
  rhrd_id_getlocal = rb_intern("getlocal");
  rhrd_id_hash = rb_intern("hash");
  rhrd_id_length = rb_intern("length");
  rhrd_id_include_q = rb_intern("include?");
  rhrd_id_local = rb_intern("local");
  rhrd_id_localtime = rb_intern("localtime");
  rhrd_id_match = rb_intern("match");
  rhrd_id_now = rb_intern("now");
  rhrd_id_slice = rb_intern("slice");
  rhrd_id_split = rb_intern("split");
  rhrd_id_sub_b = rb_intern("sub!");
  rhrd_id_to_i = rb_intern("to_i");
#ifdef RUBY19
  rhrd_id_nsec = rb_intern("nsec");
#else
  rhrd_id_usec = rb_intern("usec");
#endif
  rhrd_id_utc = rb_intern("utc");
  rhrd_id_utc_offset = rb_intern("utc_offset");

#ifdef RUBY19
  rhrd_id__httpdate = rb_intern("_httpdate");
  rhrd_id__iso8601 = rb_intern("_iso8601");
  rhrd_id__jisx0301 = rb_intern("_jisx0301");
  rhrd_id__rfc2822 = rb_intern("_rfc2822");
  rhrd_id__rfc3339 = rb_intern("_rfc3339");
  rhrd_id__xmlschema = rb_intern("_xmlschema");
#endif

  rhrd_sym_cwday = ID2SYM(rb_intern("cwday"));
  rhrd_sym_cweek = ID2SYM(rb_intern("cweek"));
  rhrd_sym_cwyear = ID2SYM(rb_intern("cwyear"));
  rhrd_sym_hour = ID2SYM(rb_intern("hour"));
#ifdef RUBY19
  rhrd_sym_leftover = ID2SYM(rb_intern("leftover"));
#endif
  rhrd_sym_mday = ID2SYM(rb_intern("mday"));
  rhrd_sym_min = ID2SYM(rb_intern("min"));
  rhrd_sym_mon = ID2SYM(rb_intern("mon"));
  rhrd_sym_offset = ID2SYM(rb_intern("offset"));
  rhrd_sym_sec = ID2SYM(rb_intern("sec"));
  rhrd_sym_sec_fraction = ID2SYM(rb_intern("sec_fraction"));
  rhrd_sym_seconds = ID2SYM(rb_intern("seconds"));
  rhrd_sym_wday = ID2SYM(rb_intern("wday"));
  rhrd_sym_wnum0 = ID2SYM(rb_intern("wnum0"));
  rhrd_sym_wnum1 = ID2SYM(rb_intern("wnum1"));
  rhrd_sym_yday = ID2SYM(rb_intern("yday"));
  rhrd_sym_year = ID2SYM(rb_intern("year"));
  rhrd_sym_zone = ID2SYM(rb_intern("zone"));

  rhrd_class = rb_define_class("Date", rb_cObject);
  rb_undef_alloc_func(rhrd_class);
  rhrd_s_class = rb_singleton_class(rhrd_class);

  rb_define_const(rhrd_class, "ITALY", LONG2NUM(RHR_JD_ITALY));
  rb_define_const(rhrd_class, "ENGLAND", LONG2NUM(RHR_JD_ENGLAND));
  rb_define_const(rhrd_class, "JULIAN", LONG2NUM(RHR_JD_MIN - 1));
  rb_define_const(rhrd_class, "GREGORIAN", LONG2NUM(RHR_JD_MAX + 1));

  /* All ruby versions */
  rb_define_method(rhrd_s_class, "_load", rhrd_s__load, 1);
  rb_define_method(rhrd_s_class, "_strptime", rhrd_s__strptime, -1);
  rb_define_method(rhrd_s_class, "civil", rhrd_s_civil, -1);
  rb_define_method(rhrd_s_class, "commercial", rhrd_s_commercial, -1);
  rb_define_method(rhrd_s_class, "gregorian_leap?", rhrd_s_gregorian_leap_q, 1);
  rb_define_method(rhrd_s_class, "jd", rhrd_s_jd, -1);
  rb_define_method(rhrd_s_class, "julian_leap?", rhrd_s_julian_leap_q, 1);
  rb_define_method(rhrd_s_class, "new!", rhrd_s_new_b, -1);
  rb_define_method(rhrd_s_class, "ordinal", rhrd_s_ordinal, -1);
  rb_define_method(rhrd_s_class, "parse", rhrd_s_parse, -1);
  rb_define_method(rhrd_s_class, "strptime", rhrd_s_strptime, -1);
  rb_define_method(rhrd_s_class, "today", rhrd_s_today, -1);
  rb_define_method(rhrd_s_class, "valid_civil?", rhrd_s_valid_civil_q, -1);
  rb_define_method(rhrd_s_class, "valid_commercial?", rhrd_s_valid_commercial_q, -1);
  rb_define_method(rhrd_s_class, "valid_jd?", rhrd_s_valid_jd_q, -1);
  rb_define_method(rhrd_s_class, "valid_ordinal?", rhrd_s_valid_ordinal_q, -1);

  rb_define_alias(rhrd_s_class, "leap?", "gregorian_leap?");
  rb_define_alias(rhrd_s_class, "new", "civil");
  rb_define_alias(rhrd_s_class, "valid_date?", "valid_civil?");

  rb_define_private_method(rhrd_s_class, "_ragel_parse", rhrd_s__ragel_parse, 1);

  rb_define_method(rhrd_class, "_dump", rhrd__dump, 1);
  rb_define_method(rhrd_class, "asctime", rhrd_asctime, 0);
  rb_define_method(rhrd_class, "cwday", rhrd_cwday, 0);
  rb_define_method(rhrd_class, "cweek", rhrd_cweek, 0);
  rb_define_method(rhrd_class, "cwyear", rhrd_cwyear, 0);
  rb_define_method(rhrd_class, "day", rhrd_day, 0);
  rb_define_method(rhrd_class, "day_fraction", rhrd_day_fraction, 0);
  rb_define_method(rhrd_class, "downto", rhrd_downto, 1);
  rb_define_method(rhrd_class, "eql?", rhrd_eql_q, 1);
  rb_define_method(rhrd_class, "gregorian", rhrd_gregorian, 0);
  rb_define_method(rhrd_class, "gregorian?", rhrd_gregorian_q, 0);
  rb_define_method(rhrd_class, "hash", rhrd_hash, 0);
  rb_define_method(rhrd_class, "inspect", rhrd_inspect, 0);
  rb_define_method(rhrd_class, "jd", rhrd_jd, 0);
  rb_define_method(rhrd_class, "julian?", rhrd_julian_q, 0);
  rb_define_method(rhrd_class, "ld", rhrd_ld, 0);
  rb_define_method(rhrd_class, "leap?", rhrd_leap_q, 0);
  rb_define_method(rhrd_class, "mjd", rhrd_mjd, 0);
  rb_define_method(rhrd_class, "month", rhrd_month, 0);
  rb_define_method(rhrd_class, "next", rhrd_next, 0);
  rb_define_method(rhrd_class, "new_start", rhrd_new_start, -1);
  rb_define_method(rhrd_class, "start", rhrd_start, 0);
  rb_define_method(rhrd_class, "step", rhrd_step, -1);
  rb_define_method(rhrd_class, "strftime", rhrd_strftime, -1);
  rb_define_method(rhrd_class, "to_s", rhrd_to_s, 0);
  rb_define_method(rhrd_class, "upto", rhrd_upto, 1);
  rb_define_method(rhrd_class, "wday", rhrd_wday, 0);
  rb_define_method(rhrd_class, "yday", rhrd_yday, 0);
  rb_define_method(rhrd_class, "year", rhrd_year, 0);

  rb_define_alias(rhrd_class, "ajd", "jd");
  rb_define_alias(rhrd_class, "amjd", "mjd");
  rb_define_alias(rhrd_class, "ctime", "asctime");
  rb_define_alias(rhrd_class, "england", "gregorian");
  rb_define_alias(rhrd_class, "italy", "gregorian");
  rb_define_alias(rhrd_class, "julian", "gregorian");
  rb_define_alias(rhrd_class, "mday", "day");
  rb_define_alias(rhrd_class, "mon", "month");
  rb_define_alias(rhrd_class, "succ", "next");

  rb_define_method(rhrd_class, ">>", rhrd_op_right_shift, 1);
  rb_define_method(rhrd_class, "<<", rhrd_op_left_shift, 1);
  rb_define_method(rhrd_class, "+", rhrd_op_plus, 1);
  rb_define_method(rhrd_class, "-", rhrd_op_minus, 1);
  rb_define_method(rhrd_class, "===", rhrd_op_relationship, 1);
  rb_define_method(rhrd_class, "<=>", rhrd_op_spaceship, 1);

  rb_funcall(rhrd_class, rb_intern("include"), 1, rb_mComparable);

  rhrd_monthnames = rb_ary_new2(13);
  rhrd_abbr_monthnames = rb_ary_new2(13);
  rb_ary_push(rhrd_monthnames, Qnil);
  rb_ary_push(rhrd_abbr_monthnames, Qnil);
  for(i = 1; i < 13; i++) {
    rb_ary_push(rhrd_monthnames, rb_str_new2(rhrd__month_names[i]));
    rb_ary_push(rhrd_abbr_monthnames, rb_str_new2(rhrd__abbr_month_names[i]));
  }

  rhrd_daynames = rb_ary_new2(7);
  rhrd_abbr_daynames = rb_ary_new2(7);
  for(i = 0; i < 7; i++) {
    rb_ary_push(rhrd_daynames, rb_str_new2(rhrd__day_names[i]));
    rb_ary_push(rhrd_abbr_daynames, rb_str_new2(rhrd__abbr_day_names[i]));
  }

  rhrd_zones_hash = rb_hash_new();
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("a"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("adt"), LONG2NUM(-10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("afghanistan"), LONG2NUM(16200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("ahst"), LONG2NUM(-36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("akdt"), LONG2NUM(-28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("akst"), LONG2NUM(-32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("alaskan"), LONG2NUM(-32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("arab"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("arabian"), LONG2NUM(14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("arabic"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("art"), LONG2NUM(-10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("ast"), LONG2NUM(-14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("at"), LONG2NUM(-7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("atlantic"), LONG2NUM(-14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("aus central"), LONG2NUM(34200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("aus eastern"), LONG2NUM(36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("azores"), LONG2NUM(-3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("b"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("brst"), LONG2NUM(-7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("brt"), LONG2NUM(-10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("bst"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("bt"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("c"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("canada central"), LONG2NUM(-21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("cape verde"), LONG2NUM(-3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("cat"), LONG2NUM(-36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("caucasus"), LONG2NUM(14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("cct"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("cdt"), LONG2NUM(-18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("cen. australia"), LONG2NUM(34200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("central"), LONG2NUM(-21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("central america"), LONG2NUM(-21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("central asia"), LONG2NUM(21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("central europe"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("central european"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("central pacific"), LONG2NUM(39600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("cest"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("cet"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("china"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("clst"), LONG2NUM(-10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("clt"), LONG2NUM(-14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("cst"), LONG2NUM(-21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("d"), LONG2NUM(14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("dateline"), LONG2NUM(-43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("e"), LONG2NUM(18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("e. africa"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("e. australia"), LONG2NUM(36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("e. europe"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("e. south america"), LONG2NUM(-10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("eadt"), LONG2NUM(39600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("east"), LONG2NUM(36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("eastern"), LONG2NUM(-18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("eat"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("edt"), LONG2NUM(-14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("eest"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("eet"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("egypt"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("ekaterinburg"), LONG2NUM(18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("est"), LONG2NUM(-18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("f"), LONG2NUM(21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("fiji"), LONG2NUM(43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("fle"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("fst"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("fwt"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("g"), LONG2NUM(25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("gmt"), LONG2NUM(0));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("greenland"), LONG2NUM(-10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("greenwich"), LONG2NUM(0));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("gst"), LONG2NUM(36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("gtb"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("h"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("hadt"), LONG2NUM(-32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("hast"), LONG2NUM(-36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("hawaiian"), LONG2NUM(-36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("hdt"), LONG2NUM(-32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("hst"), LONG2NUM(-36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("i"), LONG2NUM(32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("idle"), LONG2NUM(43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("idlw"), LONG2NUM(-43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("india"), LONG2NUM(19800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("iran"), LONG2NUM(12600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("ist"), LONG2NUM(19800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("jerusalem"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("jst"), LONG2NUM(32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("k"), LONG2NUM(36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("korea"), LONG2NUM(32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("kst"), LONG2NUM(32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("l"), LONG2NUM(39600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("m"), LONG2NUM(43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("malay peninsula"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mdt"), LONG2NUM(-21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mest"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mesz"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("met"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mewt"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mexico"), LONG2NUM(-21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mez"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mid-atlantic"), LONG2NUM(-7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mountain"), LONG2NUM(-25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("msd"), LONG2NUM(14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("msk"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("mst"), LONG2NUM(-25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("myanmar"), LONG2NUM(23400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("n"), LONG2NUM(-3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("n. central asia"), LONG2NUM(21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("ndt"), LONG2NUM(-9000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("nepal"), LONG2NUM(20700));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("new zealand"), LONG2NUM(43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("newfoundland"), LONG2NUM(-12600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("north asia"), LONG2NUM(25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("north asia east"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("nst"), LONG2NUM(-12600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("nt"), LONG2NUM(-39600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("nzdt"), LONG2NUM(46800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("nzst"), LONG2NUM(43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("nzt"), LONG2NUM(43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("o"), LONG2NUM(-7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("p"), LONG2NUM(-10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("pacific"), LONG2NUM(-28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("pacific sa"), LONG2NUM(-14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("pdt"), LONG2NUM(-25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("pst"), LONG2NUM(-28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("q"), LONG2NUM(-14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("r"), LONG2NUM(-18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("romance"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("russian"), LONG2NUM(10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("s"), LONG2NUM(-21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("sa eastern"), LONG2NUM(-10800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("sa pacific"), LONG2NUM(-18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("sa western"), LONG2NUM(-14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("samoa"), LONG2NUM(-39600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("sast"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("se asia"), LONG2NUM(25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("sgt"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("south africa"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("sri lanka"), LONG2NUM(21600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("sst"), LONG2NUM(7200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("swt"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("t"), LONG2NUM(-25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("taipei"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("tasmania"), LONG2NUM(36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("tokyo"), LONG2NUM(32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("tonga"), LONG2NUM(46800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("u"), LONG2NUM(-28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("us eastern"), LONG2NUM(-18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("us mountain"), LONG2NUM(-25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("ut"), LONG2NUM(0));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("utc"), LONG2NUM(0));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("v"), LONG2NUM(-32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("vladivostok"), LONG2NUM(36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("w"), LONG2NUM(-36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("w. australia"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("w. central africa"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("w. europe"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("wadt"), LONG2NUM(28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("wast"), LONG2NUM(25200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("wat"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("west"), LONG2NUM(3600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("west asia"), LONG2NUM(18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("west pacific"), LONG2NUM(36000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("wet"), LONG2NUM(0));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("x"), LONG2NUM(-39600));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("y"), LONG2NUM(-43200));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("yakutsk"), LONG2NUM(32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("ydt"), LONG2NUM(-28800));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("yst"), LONG2NUM(-32400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("z"), LONG2NUM(0));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("zp4"), LONG2NUM(14400));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("zp5"), LONG2NUM(18000));
  rb_hash_aset(rhrd_zones_hash, rb_str_new2("zp6"), LONG2NUM(21600));
  rb_define_const(rhrd_s_class, "ZONES", rhrd_zones_hash);

  rhrd_empty_string = rb_str_new("", 0);
  rb_define_const(rhrd_s_class, "EMPTY_STRING", rhrd_empty_string);
  rhrd_string_colon = rb_str_new(":", 1);
  rb_define_const(rhrd_s_class, "COLON", rhrd_string_colon);
  rhrd_re_comma_period = rb_reg_new("[,.]", 4, 0);
  rb_define_const(rhrd_s_class, "COMMA_PERIOD", rhrd_re_comma_period);

  /* 1 is option ignore case */
  rhrd_zone_re = rb_reg_new(rhrd__zone_re_str, strlen(rhrd__zone_re_str), 1);
  rb_define_const(rhrd_s_class, "ZONE_RE", rhrd_zone_re);
  rhrd_zone_dst_re = rb_reg_new(rhrd__zone_dst_re_str, strlen(rhrd__zone_dst_re_str), 1);
  rb_define_const(rhrd_s_class, "ZONE_DST_RE", rhrd_zone_dst_re);
  rhrd_zone_sign_re = rb_reg_new(rhrd__zone_sign_re_str, strlen(rhrd__zone_sign_re_str), 1);
  rb_define_const(rhrd_s_class, "ZONE_SIGN_RE", rhrd_zone_sign_re);

  rb_define_const(rhrd_class, "MONTHNAMES", rhrd_monthnames);
  rb_define_const(rhrd_class, "ABBR_MONTHNAMES", rhrd_abbr_monthnames);
  rb_define_const(rhrd_class, "DAYNAMES", rhrd_daynames);
  rb_define_const(rhrd_class, "ABBR_DAYNAMES", rhrd_abbr_daynames);

#ifdef RUBY19
  rb_define_method(rhrd_s_class, "httpdate", rhrd_s_httpdate, -1);
  rb_define_method(rhrd_s_class, "iso8601", rhrd_s_iso8601, -1);
  rb_define_method(rhrd_s_class, "jisx0301", rhrd_s_jisx0301, -1);
  rb_define_method(rhrd_s_class, "rfc2822", rhrd_s_rfc2822, -1);
  rb_define_method(rhrd_s_class, "rfc3339", rhrd_s_rfc3339, -1);
  rb_define_method(rhrd_s_class, "xmlschema", rhrd_s_xmlschema, -1);

  rb_define_alias(rhrd_s_class, "rfc822", "rfc2822");

  rb_define_method(rhrd_class, "httpdate", rhrd_httpdate, 0);
  rb_define_method(rhrd_class, "jisx0301", rhrd_jisx0301, 0);
  rb_define_method(rhrd_class, "next_day", rhrd_next_day, -1);
  rb_define_method(rhrd_class, "next_month", rhrd_next_month, -1);
  rb_define_method(rhrd_class, "next_year", rhrd_next_year, -1);
  rb_define_method(rhrd_class, "prev_day", rhrd_prev_day, -1);
  rb_define_method(rhrd_class, "prev_month", rhrd_prev_month, -1);
  rb_define_method(rhrd_class, "prev_year", rhrd_prev_year, -1);
  rb_define_method(rhrd_class, "rfc2822", rhrd_rfc2822, 0);
  rb_define_method(rhrd_class, "rfc3339", rhrd_rfc3339, 0);
  rb_define_method(rhrd_class, "to_datetime", rhrd_to_datetime, 0);
  rb_define_method(rhrd_class, "to_time", rhrd_to_time, 0);

  rb_define_alias(rhrd_class, "to_date", "gregorian");
  rb_define_alias(rhrd_class, "iso8601", "to_s");
  rb_define_alias(rhrd_class, "rfc822", "rfc2822");
  rb_define_alias(rhrd_class, "xmlschema", "to_s");

  rb_define_method(rhrd_class, "sunday?", rhrd_sunday_q, 0);
  rb_define_method(rhrd_class, "monday?", rhrd_monday_q, 0);
  rb_define_method(rhrd_class, "tuesday?", rhrd_tuesday_q, 0);
  rb_define_method(rhrd_class, "wednesday?", rhrd_wednesday_q, 0);
  rb_define_method(rhrd_class, "thursday?", rhrd_thursday_q, 0);
  rb_define_method(rhrd_class, "friday?", rhrd_friday_q, 0);
  rb_define_method(rhrd_class, "saturday?", rhrd_saturday_q, 0);

  rb_define_private_method(rhrd_s_class, "zone_to_diff", rhrd_s_zone_to_diff, 1);

  rb_define_method(rb_cTime, "to_date", rhrd_time_to_date, 0);
  rb_define_method(rb_cTime, "to_time", rhrd_time_to_time, 0);
#else
  rb_define_method(rhrd_s_class, "ajd_to_amjd", rhrd_s_ajd_to_amjd, 1);
  rb_define_method(rhrd_s_class, "ajd_to_jd", rhrd_s_ajd_to_jd, -1);
  rb_define_method(rhrd_s_class, "amjd_to_ajd", rhrd_s_amjd_to_ajd, 1);
  rb_define_method(rhrd_s_class, "civil_to_jd", rhrd_s_civil_to_jd, -1);
  rb_define_method(rhrd_s_class, "commercial_to_jd", rhrd_s_commercial_to_jd, -1);
  rb_define_method(rhrd_s_class, "day_fraction_to_time", rhrd_s_day_fraction_to_time, 1);
  rb_define_method(rhrd_s_class, "gregorian?", rhrd_s_gregorian_q, 2);
  rb_define_method(rhrd_s_class, "jd_to_ajd", rhrd_s_jd_to_ajd, -1);
  rb_define_method(rhrd_s_class, "jd_to_civil", rhrd_s_jd_to_civil, -1);
  rb_define_method(rhrd_s_class, "jd_to_commercial", rhrd_s_jd_to_commercial, -1);
  rb_define_method(rhrd_s_class, "jd_to_ld", rhrd_s_jd_to_ld, 1);
  rb_define_method(rhrd_s_class, "jd_to_mjd", rhrd_s_jd_to_mjd, 1);
  rb_define_method(rhrd_s_class, "jd_to_ordinal", rhrd_s_jd_to_ordinal, -1);
  rb_define_method(rhrd_s_class, "jd_to_wday", rhrd_s_jd_to_wday, 1);
  rb_define_method(rhrd_s_class, "julian?", rhrd_s_julian_q, 2);
  rb_define_method(rhrd_s_class, "ld_to_jd", rhrd_s_ld_to_jd, 1);
  rb_define_method(rhrd_s_class, "mjd_to_jd", rhrd_s_mjd_to_jd, 1);
  rb_define_method(rhrd_s_class, "ordinal_to_jd", rhrd_s_ordinal_to_jd, -1);
  rb_define_method(rhrd_s_class, "time_to_day_fraction", rhrd_s_time_to_day_fraction, 3);
  rb_define_method(rhrd_s_class, "valid_time?", rhrd_s_valid_time_q, 3);
  rb_define_method(rhrd_s_class, "zone_to_diff", rhrd_s_zone_to_diff, 1);

  rb_define_alias(rhrd_s_class, "exist?", "valid_civil?");
  rb_define_alias(rhrd_s_class, "exist1?", "valid_jd?");
  rb_define_alias(rhrd_s_class, "exist2?", "valid_ordinal?");
  rb_define_alias(rhrd_s_class, "exist3?", "valid_civil?");
  rb_define_alias(rhrd_s_class, "existw?", "valid_commercial?");
  rb_define_alias(rhrd_s_class, "new0", "new!");
  rb_define_alias(rhrd_s_class, "new1", "jd");
  rb_define_alias(rhrd_s_class, "new2", "ordinal");
  rb_define_alias(rhrd_s_class, "new3", "civil");
  rb_define_alias(rhrd_s_class, "neww", "commercial");
  rb_define_alias(rhrd_s_class, "ns?", "gregorian?");
  rb_define_alias(rhrd_s_class, "os?", "julian?");

  rb_define_alias(rhrd_class, "newsg", "new_start");
  rb_define_alias(rhrd_class, "ns?", "gregorian?");
  rb_define_alias(rhrd_class, "os?", "julian?");
  rb_define_alias(rhrd_class, "sg", "start");
#endif

  Init_datetime();
}

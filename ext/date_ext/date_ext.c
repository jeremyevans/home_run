#include <ctype.h>
#include "date_ext.h"

#ifdef RHR_ENCODING
int rhrd_encoding_index;
#endif

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
const char rhrd__strptime_num_patterns[] = "CDdeFGgHIjkLlMmNQRrSsTUuVvWwXxYy0123456789";

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
VALUE rhrd_empty_string;
VALUE rhrd_start_num;
VALUE rhrd_string_colon;
VALUE rhrd_re_comma_period;

ID rhrd_id_op_array;
ID rhrd_id_op_gte;
ID rhrd_id_op_lt;
ID rhrd_id__parse;
ID rhrd_id_cwday;
ID rhrd_id_cweek;
ID rhrd_id_cwyear;
ID rhrd_id_downcase;
ID rhrd_id_getlocal;
ID rhrd_id_hash;
ID rhrd_id_include_q;
ID rhrd_id_length;
ID rhrd_id_local;
ID rhrd_id_localtime;
ID rhrd_id_match;
ID rhrd_id_now;
ID rhrd_id_offset;
ID rhrd_id_slice;
ID rhrd_id_split;
ID rhrd_id_sub_b;
ID rhrd_id_to_enum;
ID rhrd_id_to_i;
#ifdef RUBY19
ID rhrd_id_nsec;
#else
ID rhrd_id_usec;
#endif
ID rhrd_id_utc;
ID rhrd_id_utc_offset;
ID rhrd_id_Format;
ID rhrd_id_ZONES;

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
VALUE rhrd_sym_step;
VALUE rhrd_sym_wday;
VALUE rhrd_sym_wnum0;
VALUE rhrd_sym_wnum1;
VALUE rhrd_sym_yday;
VALUE rhrd_sym_year;
VALUE rhrd_sym_zone;

static VALUE rhrd_step(int argc, VALUE *argv, VALUE self);
static VALUE rhrd_to_s(VALUE self);

/* C helper methods */

/* C % operator can (must in C99) return negative number for negative
 * dividend, this always returns a positive number, similar to ruby. */
long rhrd__mod(long a, long b) {
  long c;
  c = a % b;
  if (c < 0) {
    c += b;
  }
  return c;
}

/* Same as above but for long long dividend. */
long rhrd__modll(long long a, long b) {
  long c;
  c = (long)(a % b);
  if (c < 0) {
    c += b;
  }
  return c;
}

/* Return 1 if the year, month, day provided are within
 * the allowed limits, 0 otherwise. */
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

/* Add two numbers, raising a RangeError on
 * overflow. */
long rhrd__safe_add_long(long a, long b) {
  if ((a > 0 && b > 0 && (a > LONG_MAX - b)) ||
      (a < 0 && b < 0 && (a < LONG_MIN - b))) {
    rb_raise(rb_eRangeError, "addition would overflow");
  }
  return a + b;
}

/* Convert a year, month, day to a julian date. */
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

/* Fill the jd field rhrd_t with the info from the year,
 * month, and day fields.  This assumes the year,
 * month, and day fields have already been filled in. */
void rhrd__civil_to_jd(rhrd_t *d) {
  d->jd = rhrd__ymd_to_jd(d->year, d->month, d->day);
  d->flags |= RHR_HAVE_JD;
}

/* Fill in the year, month, and day fields for an
 * rhrd_t, using the jd field.  This assumes the jd
 * field has already been filled in. */
void rhrd__jd_to_civil(rhrd_t *date) {
  long x, a, b, c, d, e;
  x = (long)floor((date->jd - 1867216.25) / 36524.25);
  a = date->jd + 1 + x - (long)floor(x / 4.0);
  b = a + 1524;
  c = (long)floor((b - 122.1) / 365.25);
  d = (long)floor(365.25 * c);
  e = (long)floor((b - d) / 30.6001);
  date->day = (unsigned char)(b - d - (long)floor(30.6001 * e));
  if (e <= 13) {
    date->month = (unsigned char)(e - 1);
    date->year = c - 4716;
  } else {
    date->month = (unsigned char)(e - 13);
    date->year = c - 4715;
  }
  date->flags |= RHR_HAVE_CIVIL;
}

/* Returns 1 if the given year is a leap year in the Gregorian
 * calendar, 0 if not. */
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

/* Check if the year, month, and day given are a valid date
 * in the Gregorian calendar, filling in the appropriate
 * fields in the rhrd_t and returning 1 if so.  If the fields
 * given are not a valid date, return 0. 
 * This also handles wrap around if the month or day is negative. */
int rhrd__valid_civil(rhrd_t *d, long year, long month, long day, int overlimit_raise) {
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
    if (overlimit_raise == RHR_OVERLIMIT_RAISE) {
      rb_raise(rb_eRangeError, "date out of range: year = %li, month = %li, day = %li", year, month, day);
    }
    return 0;
  } 

  d->year = year;
  d->month = (unsigned char)month;
  d->day = (unsigned char)day;
  d->flags |= RHR_HAVE_CIVIL;
  return 1;
}

/* Return the number of days in the given month in the given year */
unsigned char rhrd__days_in_month(long year, unsigned char month) {
  if (month == 2) {
    return rhrd__leap_year(year) ? 29 : 28;
  } else {
    return rhrd_days_in_month[month];
  }
}

/* Add n days to the the given Date object.  n can be negative if
 * you want to subtract days from the object.  Returns a new ruby
 * Date object. */
VALUE rhrd__add_days(VALUE self, long n) {
  rhrd_t *d;
  rhrd_t *newd;
  VALUE new;
  long x;
  Data_Get_Struct(self, rhrd_t, d);
  new = Data_Make_Struct(rb_obj_class(self), rhrd_t, NULL, -1, newd);

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

/* Add n months to the given ruby Date object.  n can be negative
 * if you want to subtract months from the object.  Returns a new
 * ruby Date object. */
VALUE rhrd__add_months(VALUE self, long n) {
  rhrd_t *d;
  rhrd_t *newd;
  VALUE new;
  long x;
  Data_Get_Struct(self, rhrd_t, d);

  new = Data_Make_Struct(rb_obj_class(self), rhrd_t, NULL, -1, newd);
  RHR_FILL_CIVIL(d)
  n = rhrd__safe_add_long(n, (long)(d->month));
  if(n > 1 && n <= 12) {
    newd->year = d->year;
    newd->month = (unsigned char)n;
  } else {
    x = n / 12;
    n = n % 12;
    if (n <= 0) {
      newd->year = d->year + x - 1;
      newd->month = (unsigned char)(n + 12);
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

/* Return 0 if the given two rhrd_t objects have the
 * same julian or civil dates, -1 if the first is
 * less than the second, or 1 otherwise. */
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

/* Convert the given cwyear, cweek, and cwday arguments to
 * a julian date. */
long rhrd__commercial_to_jd(long cwyear, long cweek, long cwday) {
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));

  n.year = cwyear;
  n.month = 1;
  n.day = 4;
  rhrd__civil_to_jd(&n);
  return n.jd - rhrd__mod(n.jd, 7) + 7 * (cweek - 1) + (cwday - 1);
}

/* Convert the given julian date to a cwday (range: [1-7]). */
long rhrd__jd_to_cwday(long jd) {
  long day;
  day = (jd + 1) % 7; 
  if (day <= 0) {
    day += 7;
  }
  return day;
}

/* Convert the given julian date to a weekday number
 * (range: [0-6], 0 is Sunday). */
long rhrd__jd_to_wday(long jd) {
  return rhrd__mod(jd + 1, 7);
}


/* Fill the given rhrd_t with the commercial week
 * information specified by its julian date.
 * Abuses the year, month, and day fields to store
 * cwyear, cweek, and cwday, so it should not be
 * used on a ruby Date object that has its civil
 * date filled in. */
void rhrd__fill_commercial(rhrd_t *d) {
  long a;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));

  n.jd = d->jd - 3;
  rhrd__jd_to_civil(&n);
  a = n.year;
  d->year = d->jd >= rhrd__commercial_to_jd(a + 1, 1, 1) ? a + 1 : a;
  d->month = (unsigned char)(1 + (d->jd - rhrd__commercial_to_jd(d->year, 1, 1)) / 7);
  d->day = (unsigned char)rhrd__jd_to_cwday(d->jd);
}

/* Fill in the jd field for the given rhrd_t using the cwyear, cweek,
 * and cwday arguments, if the date is valid, and return 1.  If the
 * date is not valid, return 0.  This also handles wrap around if
 * the cweek or cwday arguments is negative. */
int rhrd__valid_commercial(rhrd_t *d, long cwyear, long cweek, long cwday, int overlimit_raise) {
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
    if (overlimit_raise == RHR_OVERLIMIT_RAISE) {
      rb_raise(rb_eRangeError, "date out of range");
    }
    return 0;
  }

  d->jd = n.jd;
  d->flags = RHR_HAVE_JD;
  return 1;
}

/* Return the ordinal day number for the given civil day fields. */
long rhrd__ordinal_day(long year, unsigned char month, unsigned char day) {
  long yday;
  yday = rhrd_cumulative_days_in_month[month] + day;
  if(month > 2 && rhrd__leap_year(year)) {
    yday += 1;
  }
  return yday;
}

/* Fill in the civil date fields in the rhrd_t with the
 * given ordinal day fields, if they are valid.  Returns
 * 1 if the date is valid, 0 if not.  This also handles
 * wrap around for a negative yday argument. */
int rhrd__valid_ordinal(rhrd_t *d, long year, long yday, int overlimit_raise) {
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
    if (overlimit_raise == RHR_OVERLIMIT_RAISE) {
      rb_raise(rb_eRangeError, "date out of range");
    }
    return 0;
  } 

  d->year = year;
  d->month = (unsigned char)month;
  d->day = (unsigned char)day;
  d->flags |= RHR_HAVE_CIVIL;
  return 1;
}

/* Convert the given jd to the unix time integer
 * for the given day at midnight UTC. */
long long rhrd__jd_to_unix(long long jd) {
  return (jd - RHR_UNIX_EPOCH) * RHR_SECONDS_PER_DAY;
}

/* Convert the given unix time integer into a
 * julian date, losing any information about
 * fractional days. */
long rhrd__unix_to_jd(long long t) {
  return (long)(t/RHR_SECONDS_PER_DAY + RHR_UNIX_EPOCH);
}

/* Fill the given rhrt_d's jd field based on the
 * current local time. */
void rhrd__today(rhrd_t * d) {
  VALUE t;
  t = rb_funcall(rb_cTime, rhrd_id_now, 0);
  d->jd = rhrd__unix_to_jd((long long)NUM2LONG(rb_funcall(t, rhrd_id_to_i, 0)) + NUM2LONG(rb_funcall(t, rhrd_id_utc_offset, 0)));
  d->flags |= RHR_HAVE_JD;
  RHR_CHECK_JD(d);
}

/* Return the current year. */
long rhrd__current_year(void) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));
  rhrd__today(&d);
  RHR_FILL_CIVIL(&d);
  return d.year;
}

/* Return the current month. */
long rhrd__current_month(void) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));
  rhrd__today(&d);
  RHR_FILL_CIVIL(&d);
  return d.month;
}

/* Return the julian date of the first day of the
 * given year. */
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

/* Convert the given julian date to a week number given that
 * f is a number representing a week day (Sunday = 0) such that
 * the week 1 of the year starts on first week day of that year
 * (with days before then as week 0).
 * So if January 1st is a Monday and f = 1, January 1-7 would be
 * in week 1.  However, if f = 0, January 1-6 would be in week 0,
 * and January 7-13 would be in week 1. */
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

/* Convert a given week number and week day to a julian day given
 * that f means the same as it does in the prior function. */
long rhrd__weeknum_to_jd(long year, long week, long wday, int f) {
  long yday1_jd;
  yday1_jd = rhrd__yday1_jd(year) + 6;
  return (yday1_jd - rhrd__mod(yday1_jd - f + 1, 7) - 7) + 7 * week + wday;
}

/* Using the values in the given ruby hash, fill the appropriate date fields
 * in the given rhrd_t. Returns:
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
    d->jd = rhrd__unix_to_jd(NUM2LL(runix));
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
      if(!rhrd__valid_commercial(d, d->year, 1, NUM2LONG(rwday), RHR_NO_RAISE)) {
        return 1;
      }
      d->flags &= (unsigned char)~RHR_HAVE_CIVIL;
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
    d->flags &= (unsigned char)~RHR_HAVE_CIVIL;
    return 0;
  } else {
    return -1;
  }

  if (yday && rhrd__valid_ordinal(d, year, yday, RHR_OVERLIMIT_RAISE)) {
    return 0;
  }
  if (cweek && cwday && rhrd__valid_commercial(d, cwyear, cweek, cwday, RHR_OVERLIMIT_RAISE)) {
    return 0;
  }
  if (rhrd__valid_civil(d, year, month, day, RHR_OVERLIMIT_RAISE)) {
    return 0;
  }
  return 1;
}

/* Returns a new ruby object filled with information from
 * the given hash, or raises an error if the given hash
 * did not contain valid date information. */
VALUE rhrd__from_hash(VALUE klass, VALUE hash) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
  if(rhrd__fill_from_hash(d, hash)) {
    rb_raise(rb_eArgError, "invalid date");
  } else {
    RHR_FILL_JD(d)
    RHR_CHECK_JD(d)
    return rd;
  }
}

/* Return a ruby string using the information in the given
 * rhrdt_t and the given format string information. */
VALUE rhrd__strftime(rhrdt_t *d, const char * fmt, int fmt_len) {
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
          cp += sprintf(str + cp, "%s %s %2i %02i:%02i:%02i %04li", rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)], rhrd__abbr_month_names[d->month], (int)d->day, (int)d->hour, (int)d->minute, (int)d->second, d->year);
          break;
        case 'C':
          cp += sprintf(str + cp, "%02li", d->year / 100);
          break;
        case 'd':
          cp += sprintf(str + cp, "%02i", (int)d->day);
          break;
        case 'e':
          cp += sprintf(str + cp, "%2i", (int)d->day);
          break;
        case 'F':
          cp += sprintf(str + cp, "%04li-%02i-%02i", d->year, (int)d->month, (int)d->day);
          break;
        case 'g':
          cp += sprintf(str + cp, "%02li", cd.year % 100);
          break;
        case 'G':
          cp += sprintf(str + cp, "%04li", cd.year);
          break;
        case 'H':
          cp += sprintf(str + cp, "%02i", (int)d->hour);
          break;
        case 'I':
          cp += sprintf(str + cp, "%02i", (int)((d->hour == 12 || d->hour == 0) ? 12 : d->hour % 12));
          break;
        case 'j':
          cp += sprintf(str + cp, "%03li", rhrd__ordinal_day(d->year, d->month, d->day));
          break;
        case 'k':
          cp += sprintf(str + cp, "%2i", (int)(d->hour));
          break;
        case 'l':
          cp += sprintf(str + cp, "%2i", (int)((d->hour == 12 || d->hour == 0) ? 12 : d->hour % 12));
          break;
        case 'L':
          cp += sprintf(str + cp, "%03" PRId64, (d->nanos % RHR_NANOS_PER_SECOND)/1000000);
          break;
        case 'm':
          cp += sprintf(str + cp, "%02i", (int)(d->month));
          break;
        case 'M':
          cp += sprintf(str + cp, "%02i", (int)(d->minute));
          break;
        case 'N':
          cp += sprintf(str + cp, "%09" PRId64, (d->nanos % RHR_NANOS_PER_SECOND));
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
          cp += sprintf(str + cp, "%" PRId64, rhrd__jd_to_unix((long long)d->jd) * 1000 + d->nanos/RHR_NANOS_PER_MILLISECOND - d->offset * 60000);
          break;
        case 'r':
          cp += sprintf(str + cp, "%2i:%02i:%02i %s", (int)((d->hour == 12 || d->hour == 0) ? 12 : d->hour % 12), (int)d->minute, (int)d->second, d->hour >= 12 ? "PM" : "AM");
          break;
        case 'R':
          cp += sprintf(str + cp, "%02i:%02i", (int)d->hour, (int)d->minute);
          break;
        case 's':
          cp += sprintf(str + cp, "%" PRId64, rhrd__jd_to_unix((long long)d->jd) + d->nanos/RHR_NANOS_PER_SECOND - d->offset * 60);
          break;
        case 'S':
          cp += sprintf(str + cp, "%02i", (int)d->second);
          break;
        case 't':
          cp += sprintf(str + cp, "\t");
          break;
        case 'X':
        case 'T':
          cp += sprintf(str + cp, "%02i:%02i:%02i", (int)d->hour, (int)d->minute, (int)d->second);
          break;
        case 'u':
          cp += sprintf(str + cp, "%i", (int)cd.day);
          break;
        case 'U':
          cp += sprintf(str + cp, "%li", rhrd__jd_to_weeknum(d->jd, 0));
          break;
        case 'v':
          cp += sprintf(str + cp, "%2i-%s-%04li", (int)d->day, rhrd__abbr_month_names[d->month], d->year);
          break;
        case 'V':
          cp += sprintf(str + cp, "%02i", (int)cd.month);
          break;
        case 'w':
          cp += sprintf(str + cp, "%li", rhrd__jd_to_wday(d->jd));
          break;
        case 'W':
          cp += sprintf(str + cp, "%li", rhrd__jd_to_weeknum(d->jd, 1));
          break;
        case 'D':
        case 'x':
          cp += sprintf(str + cp, "%02i/%02i/%02li", (int)d->month, (int)d->day, d->year % 100);
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
          cp += sprintf(str + cp, "%s %s %2i %02i:%02i:%02i %+03i:%02i %04li", rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)], rhrd__abbr_month_names[d->month], (int)d->day, (int)d->hour, (int)d->minute, (int)d->second, d->offset/60, abs(d->offset % 60), d->year);
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

/* Return a ruby hash using the given ruby string and the format
 * string information. */
VALUE rhrd__strptime(VALUE rstr, const char *fmt_str, long fmt_len) {
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
  long long seconds = 0;
  long long milliseconds = 0;
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
  int year_only4 = 0;
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
            scan_len = (int)strlen(rhrd__day_names[i]);
            if (pos + scan_len <= len) {
              if(strncasecmp(str + pos, rhrd__day_names[i], (size_t)scan_len) == 0) {
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
            scan_len = (int)strlen(rhrd__month_names[i]);
            if (pos + scan_len <= len) {
              if(strncasecmp(str + pos, rhrd__month_names[i], (size_t)scan_len) == 0) {
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
#define RHR_PARSE_year_only4 year_only4 = 0;\
          if (fmt_pos + 1 < fmt_len) {\
            if(isdigit(fmt_str[fmt_pos + 1])) {\
              year_only4 = 1;\
            } else if ((fmt_pos + 2 < fmt_len) && (fmt_str[fmt_pos + 1] == '%') && strchr(rhrd__strptime_num_patterns, fmt_str[fmt_pos + 2])) {\
              year_only4 = 1;\
            }\
          }
          RHR_PARSE_year_only4 
          if (sscanf(str + pos, year_only4 ? "%4ld%n" : "%ld%n", &cwyear, &scan_len) != 1) {
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
          sec_fraction = sec_fraction_num/pow(10.0, (double)scan_len);
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
          sec_fraction = sec_fraction_num/pow(10.0, (double)scan_len);
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
          if (sscanf(str + pos, "%" SCNd64 "%n", &milliseconds, &scan_len) != 1) {
            return Qnil;
          }
          state |= RHRR_UNIXM_SET;
          break;
        case 's':
          if (sscanf(str + pos, "%" SCNd64 "%n", &seconds, &scan_len) != 1) {
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
#define RHR_PARSE_Y if (sscanf(str + pos, year_only4 ? "%4ld%n" : "%ld%n", &year, &scan_len) != 1) {\
            return Qnil;\
          }\
          state |= RHRR_YEAR_SET + RHRR_CENTURY_SET;
          RHR_PARSE_year_only4 
          RHR_PARSE_Y
          break;
        case 'z':
        case 'Z':
#define RHR_PARSE_Z zone = rb_funcall(rhrd_zone_re, rhrd_id_match, 1, rb_funcall(rstr, rhrd_id_slice, 2, LONG2NUM(pos), LONG2NUM(len)));\
          if (RTEST(zone)) {\
            zone = rb_funcall(zone, rhrd_id_op_array, 1, INT2FIX(0));\
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
          RHR_PARSE_year_only4 
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
          RHR_PARSE_year_only4 
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
          RHR_PARSE_year_only4 
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
    rb_hash_aset(hash, rhrd_sym_seconds, LL2NUM(seconds));
  } 
  if(state & RHRR_UNIXM_SET) {
    rb_hash_aset(hash, rhrd_sym_seconds, LL2NUM(milliseconds/1000));
    rb_hash_aset(hash, rhrd_sym_sec_fraction, rb_float_new((double)(milliseconds % 1000)/1000.0));
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

/* Set the commercial week cached instance variables. */
void rhrd__set_cw_ivs(VALUE self, rhrd_t *d) {
  rb_ivar_set(self, rhrd_id_cwyear, LONG2NUM(d->year));
  rb_ivar_set(self, rhrd_id_cweek, LONG2NUM(d->month));
  rb_ivar_set(self, rhrd_id_cwday, LONG2NUM(d->day));
}

/* Ruby class methods */

/* call-seq:
 *   _load(string) -> Date
 *
 * Unmarshal a dumped +Date+ object. Not generally called directly,
 * usually called by <tt>Marshal.load</tt>.
 *
 * Note that this does not handle the marshalling format used by
 * the stdlib's +Date+, it only handles marshalled versions of
 * this library's +Date+ objects.
 */
static VALUE rhrd_s__load(VALUE klass, VALUE string) {
  rhrd_t * d;
  VALUE jd, rd;
  jd = rb_marshal_load(string);
  rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
  d->jd = NUM2LONG(jd);
  RHR_CHECK_JD(d)
  d->flags = RHR_HAVE_JD;
  return rd;
}


/* call-seq:
 *   _ragel_parse(string) -> Hash or nil
 *
 * Attemps to parse the string with Ragel, returning a hash
 * if there is a match (or +nil+ if no match).
 */
static VALUE rhrd_s__ragel_parse(VALUE klass, VALUE s) {
  s = rb_str_to_str(s);
  return rhrd__ragel_parse(RSTRING_PTR(s), RSTRING_LEN(s));
}

/* call-seq:
 *   _strptime(string, format='%F') -> Hash or nil
 *
 * Attemps to parse the string using the given format, returning
 * a hash if there is a match (or +nil+ if no match).
 *
 * +_strptime+ supports the same formats that <tt>Date#strftime</tt> does.
 */
static VALUE rhrd_s__strptime(int argc, VALUE *argv, VALUE klass) {
  const char * fmt_str = "%F";
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
 *   allocate() -> Date <br />
 *
 * Returns a +Date+ object for julian day 0.
 */
static VALUE rhrd_s_allocate(VALUE klass) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
  d->flags = RHR_HAVE_JD;
  return rd;
}

/* call-seq:
 *   civil() -> Date <br />
 *   civil(year, month=1, day=1, sg=nil) -> Date
 *
 * If no arguments are given, returns a +Date+ for julian day 0.
 * Otherwise, returns a +Date+ for the year, month, and day given.
 * Raises an +ArgumentError+ for invalid dates.
 * Ignores the 4th argument.
 */
static VALUE rhrd_s_civil(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  long year;
  long month = 1;
  long day = 1;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);

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

  if (!rhrd__valid_civil(d, year, month, day, RHR_OVERLIMIT_RAISE)) {
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
 * commercial week day given.
 * Raises an +ArgumentError+ for invalid dates.
 * Ignores the 4th argument.
 */
static VALUE rhrd_s_commercial(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  long cwyear = RHR_DEFAULT_CWYEAR;
  long cweek = RHR_DEFAULT_CWEEK;
  long cwday = RHR_DEFAULT_CWDAY;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);

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

  if(!rhrd__valid_commercial(d, cwyear, cweek, cwday, RHR_OVERLIMIT_RAISE)) {
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
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);

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
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);

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

/* call-seq:
 *   ordinal() -> Date <br />
 *   ordinal(year, yday=1, sg=nil) -> Date
 *
 * If no arguments are given, returns a +Date+ for julian day 0.
 * Otherwise, returns a +Date+ for the year and day of year given.
 * Raises an +ArgumentError+ for invalid dates.
 * Ignores the 3rd argument.
 */
static VALUE rhrd_s_ordinal(int argc, VALUE *argv, VALUE klass) {
  long year;
  long day = 1;
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);

  switch(argc) {
    case 2:
    case 3:
      day = NUM2LONG(argv[1]);
    case 1:
      year = NUM2LONG(argv[0]);
      if(!rhrd__valid_ordinal(d, year, day, RHR_OVERLIMIT_RAISE)) {
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

/* call-seq:
 *   parse() -> Date <br />
 *   parse(string, comp=true, sg=nil) -> Date
 *
 * If no arguments are given, returns a +Date+ for julian day 0.
 * Otherwise returns a +Date+ for the date represented by the given
 * +string+.  Raises an +ArgumentError+ if the string was not in
 * a recognized format, or if the recognized format represents
 * an invalid date.
 * If +comp+ is true, expands 2-digit years to 4-digits years.
 * Ignores the 3rd argument.
 */
static VALUE rhrd_s_parse(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
      return rhrd__from_hash(klass, rb_funcall(klass, rhrd_id__parse, 1, argv[0]));
    case 2:
    case 3:
      return rhrd__from_hash(klass, rb_funcall(klass, rhrd_id__parse, 2, argv[0], argv[1]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 3)", argc);
      break;
  }
}

/* call-seq:
 *   strptime() -> Date <br />
 *   strptime(string, format="%F", sg=nil) -> Date
 *
 * If no arguments are given, returns a +Date+ for julian day 0.
 * Other returns a +Date+ for the date represented by the given
 * +string+, parsed using the given +format+.
 * Raises an +ArgumentError+ if the string did not match the format
 * given, or if it did match and it represented an invalid date.
 * Ignores the 3rd argument.
 */
static VALUE rhrd_s_strptime(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
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
    return rhrd__from_hash(klass, rd);
  }
  rb_raise(rb_eArgError, "invalid date");
}

/* call-seq:
 *   today(sg=nil) -> Date
 *
 * Returns a +Date+ representing the current local date.
 * Ignores an argument if given.
 */
static VALUE rhrd_s_today(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);

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

/* call-seq:
 *   valid_civil?(year, month, day, sg=nil) -> Integer or nil [ruby 1-8] <br />
 *   valid_civil?(year, month, day, sg=nil) -> true or false [ruby 1-9]
 *
 * On ruby 1.8, returns the julian date +Integer+ for the given +year+, +month+,
 * and +day+, or nil if the given values are not a valid date.
 *
 * On ruby 1.9, returns true if the given +year+, +month+, and +day+ represent
 * a valid date, or false otherwise.
 *
 * Ignores the 4th argument.
 */
static VALUE rhrd_s_valid_civil_q(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 3:
    case 4:
      if (!rhrd__valid_civil(&d, NUM2LONG(argv[0]), NUM2LONG(argv[1]), NUM2LONG(argv[2]), RHR_NO_RAISE)) {
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

/* call-seq:
 *   valid_commercial?(cwyear, cweek, cwday, sg=nil) -> Integer or nil [ruby 1-8] <br />
 *   valid_commercial?(cwyear, cweek, cwday, sg=nil) -> true or false [ruby 1-9]
 *
 * On ruby 1.8, returns the julian date +Integer+ for the given +cwyear+, +cweek+,
 * and +cwday+, or nil if the given values are not a valid date.
 *
 * On ruby 1.9, returns true if the given +cwyear+, +cweek+, and +cwday+ represent
 * a valid date, or false otherwise.
 *
 * Ignores the 4th argument.
 */
static VALUE rhrd_s_valid_commercial_q(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 3:
    case 4:
      if (!rhrd__valid_commercial(&d, NUM2LONG(argv[0]), NUM2LONG(argv[1]), NUM2LONG(argv[2]), RHR_NO_RAISE)) {
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

/* call-seq:
 *   valid_jd?(jd, sg=nil) -> Object [ruby 1-8] <br />
 *   valid_jd?(jd, sg=nil) -> true [ruby 1-9]
 *
 * On ruby 1.8, returns the first argument.
 * On ruby 1.9, returns true.
 * Ignores the 2nd argument.
 */
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

/* call-seq:
 *   valid_ordinal?(year, yday, sg=nil) -> Integer or nil [ruby 1-8] <br />
 *   valid_ordinal?(year, yday, sg=nil) -> true or false [ruby 1-9]
 *
 * On ruby 1.8, returns the julian date +Integer+ for the given +year+ and +yday+,
 * or nil if the given values are not a valid date.
 *
 * On ruby 1.9, returns true if the given +year+ and +yday+ represent
 * a valid date, or false otherwise.
 *
 * Ignores the 3rd argument.
 */
static VALUE rhrd_s_valid_ordinal_q(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;
  memset(&d, 0, sizeof(rhrd_t));

  switch(argc) {
    case 2:
    case 3:
      if (!rhrd__valid_ordinal(&d, NUM2LONG(argv[0]), NUM2LONG(argv[1]), RHR_NO_RAISE)) {
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

/* call-seq:
 *   zone_to_diff(zone) -> Integer <br />
 *
 * Returns an +Integer+ representing the number of seconds that the given
 * zone string is offset from UTC.  For example, 'PDT' is Pacific Daylight Time, which
 * 7 hours before UTC, so <tt>Date.zone_to_diff('PDT')</tt> will return <tt>-25200</tt>.
 * 
 * In addition to handling time zone names, this also handles time zones specified
 * numerically, such as <tt>"+08:00"</tt> and <tt>"-0800"</tt>.
 * 
 * If the time zone is not recognized, returns 0.
 *
 * On ruby 1.9, this method is private.
 */
VALUE rhrd_s_zone_to_diff(VALUE klass, VALUE str) {
  long offset = 0;
  long len, i, j;
  char *s;
  VALUE v, e, zones;

  str = rb_funcall(str, rhrd_id_downcase, 0);
  if(RTEST(rb_funcall(str, rhrd_id_sub_b, 2, rhrd_zone_dst_re, rhrd_empty_string))) {
    if (!RTEST(rb_reg_nth_match(1, rb_gv_get("$~")))) { 
      offset += RHR_SECONDS_PER_HOUR;
    }
  }

  zones = rb_const_get(rhrd_class, rhrd_id_Format);
  if (RTEST(zones) && RTEST(rb_obj_is_kind_of(zones, rb_cModule))) {
    zones = rb_const_get(zones, rhrd_id_ZONES);
    if(RTEST(zones) && RTEST(rb_obj_is_kind_of(zones, rb_cHash)) && RTEST(v = rb_hash_aref(zones, str))) {
      return LONG2NUM(offset + NUM2LONG(v));
    }
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
        return LONG2NUM((NUM2LONG(rb_funcall(rb_ary_entry(v, 0), rhrd_id_to_i, 0)) * RHR_SECONDS_PER_HOUR
               + NUM2LONG(rb_funcall(rb_ary_entry(v, 1), rhrd_id_to_i, 0)) * 60
               + NUM2LONG(rb_funcall(rb_ary_entry(v, 2), rhrd_id_to_i, 0))) * offset);
      }
    }
    for(i=0; i < len; i++) {
      if((s[i] == ',') || (s[i] == '.')) {
        v = rb_funcall(str, rhrd_id_split, 1, rhrd_re_comma_period);
        e = rb_ary_entry(v, 1);
        return LONG2NUM(((NUM2LONG(rb_funcall(rb_ary_entry(v, 0), rhrd_id_to_i, 0)) * RHR_SECONDS_PER_HOUR)
               + ((NUM2LONG(rb_funcall(e, rhrd_id_to_i, 0)) * RHR_SECONDS_PER_HOUR) / (long)pow(10.0, (double)RSTRING_LEN(rb_str_to_str(e))))) * offset);
      }
    }
    switch (len) {
      case 0:
        return INT2FIX(0);
      case 1:
      case 2:
        return LONG2NUM(atol(s) * RHR_SECONDS_PER_HOUR * offset);
      case 3:
        i = atol(s + 1);
        s[1] = '\0';
        len = atol(s);
        return LONG2NUM((len * RHR_SECONDS_PER_HOUR + i * 60) * offset);
      case 4:
        i = atol(s + 2);
        s[2] = '\0';
        len = atol(s);
        return LONG2NUM((len * RHR_SECONDS_PER_HOUR + i * 60) * offset);
      default:
        s[6] = '\0';
      case 6:
      case 5:
        j = atol(s + 4);
        s[4] = '\0';
        i = atol(s + 2);
        s[2] = '\0';
        len = atol(s);
        return LONG2NUM((len * RHR_SECONDS_PER_HOUR + i * 60 + j) * offset);
    }
  }

  return INT2FIX(0);
}

/* Ruby instance methods */

/* call-seq:
 *   _dump(limit) -> String
 *
 * Returns a marshalled representation of the receiver as a +String+.
 * Generally not called directly, usually called by <tt>Marshal.dump</tt>.
 */
static VALUE rhrd__dump(VALUE self, VALUE limit) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return rb_marshal_dump(LONG2NUM(d->jd), LONG2NUM(NUM2LONG(limit) - 1));
}

/* call-seq:
 *   asctime() -> String
 *
 * Returns a string representation of the receiver.  Example:
 * 
 *   Date.civil(2009, 1, 2).asctime
 *   # => "Fri Jan  2 00:00:00 2009"
 */
static VALUE rhrd_asctime(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s %s %2i 00:00:00 %04li", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        rhrd__abbr_month_names[d->month],
        (int)d->day,
        d->year);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#asctime (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

/* call-seq:
 *   clone() -> Date
 *
 * Returns a clone of the receiver.
 */
static VALUE rhrd_clone(VALUE self) {
  rhrd_t *d, *nd;
  VALUE rd = rb_call_super(0, NULL);
  if (!rb_obj_is_kind_of(self, rhrdt_class)) {
    Data_Get_Struct(self, rhrd_t, d);
    Data_Get_Struct(rd, rhrd_t, nd);
    memcpy(nd, d, sizeof(rhrd_t));
  }
  return rd;
}

/* call-seq:
 *   cwday() -> Integer
 *
 * Returns the commercial week day as an +Integer+. Example:
 * 
 *   Date.civil(2009, 1, 2).cwday
 *   # => 5
 *   Date.civil(2010, 1, 2).cwday
 *   # => 6
 */
static VALUE rhrd_cwday(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  RHR_CACHED_IV(self, rhrd_id_cwday)
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  rhrd__set_cw_ivs(self, &n);
  return LONG2NUM(n.day);
}

/* call-seq:
 *   cweek() -> Integer
 *
 * Returns the commercial week as an +Integer+. Example:
 * 
 *   Date.civil(2009, 1, 2).cweek
 *   # => 1
 *   Date.civil(2010, 1, 2).cweek
 *   # => 53
 */
static VALUE rhrd_cweek(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  RHR_CACHED_IV(self, rhrd_id_cweek)
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  rhrd__set_cw_ivs(self, &n);
  return LONG2NUM(n.month);
}

/* call-seq:
 *   cwyear() -> Integer
 *
 * Returns the commercial week year as an +Integer+. Example:
 * 
 *   Date.civil(2009, 1, 2).cwyear
 *   # => 2009
 *   Date.civil(2010, 1, 2).cwyear
 *   # => 2009
 */
static VALUE rhrd_cwyear(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  RHR_CACHED_IV(self, rhrd_id_cwyear)
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  rhrd__set_cw_ivs(self, &n);
  return LONG2NUM(n.year);
}

/* call-seq:
 *   day() -> Integer
 *
 * Returns the day of the month as an +Integer+. Example:
 * 
 *   Date.civil(2009, 1, 2).day
 *   # => 2
 */
static VALUE rhrd_day(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return LONG2NUM(d->day);
}

/* call-seq:
 *   day_fraction() -> 0
 *
 * +Date+ objects due not hold fractional days, so 0 is always returned.
 */
static VALUE rhrd_day_fraction(VALUE self) {
  return INT2FIX(0);
}

/* call-seq:
 *   downto(target){|date|} -> Date
 *
 * Equivalent to calling +step+ with the +target+ as the first argument
 * and <tt>-1</tt> as the second argument. Returns self.
 * 
 *   Date.civil(2009, 1, 2).downto(Date.civil(2009, 1, 1)) do |date|
 *     puts date
 *   end
 *   # Output:
 *   # 2009-01-02
 *   # 2009-01-01
 */
static VALUE rhrd_downto(VALUE self, VALUE other) {
  VALUE argv[2];
  argv[0] = other;
  argv[1] = INT2FIX(-1);
  return rhrd_step(2, argv, self);
}

/* call-seq:
 *   dup() -> Date
 *
 * Returns a dup of the receiver.
 */
static VALUE rhrd_dup(VALUE self) {
  rhrd_t *d, *nd;
  VALUE rd = rb_call_super(0, NULL);
  if (!rb_obj_is_kind_of(self, rhrdt_class)) {
    Data_Get_Struct(self, rhrd_t, d);
    Data_Get_Struct(rd, rhrd_t, nd);
    memcpy(nd, d, sizeof(rhrd_t));
  }
  return rd;
}

/* call-seq:
 *   eql?(date) -> true or false
 *
 * Returns true only if the +date+ given is the same date as the receiver.
 * If +date+ is an instance of +DateTime+, returns +true+ only if +date+ is
 * for the same date as the receiver and has no fractional component.
 * Otherwise, returns +false+. Example:
 *
 *   Date.civil(2009, 1, 2).eql?(Date.civil(2009, 1, 2))
 *   # => true
 *   Date.civil(2009, 1, 2).eql?(Date.civil(2009, 1, 1))
 *   # => false
 *   Date.civil(2009, 1, 2).eql?(DateTime.civil(2009, 1, 2))
 *   # => true
 *   Date.civil(2009, 1, 2).eql?(DateTime.civil(2009, 1, 2, 1))
 *   # => false
 */
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

/* call-seq:
 *   gregorian() -> Date
 *
 * This library does not support modifying the day of calendar
 * reform, so this always returns self.
 */
static VALUE rhrd_gregorian(VALUE self) {
  return self;
}

/* call-seq:
 *   gregorian?() -> true
 *
 * This library always uses the Gregorian calendar, so this
 * always returns +true+.
 */
static VALUE rhrd_gregorian_q(VALUE self) {
  return Qtrue;
}

/* call-seq:
 *   hash() -> Integer
 *
 * Return an Integer hash value for the receiver, such that an
 * equal date will have the same hash value.
 */
static VALUE rhrd_hash(VALUE self) {
  rhrd_t *d;
  RHR_CACHED_IV(self, rhrd_id_hash)
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return rb_ivar_set(self, rhrd_id_hash, rb_funcall(LONG2NUM(d->jd), rhrd_id_hash, 0));
}

/* call-seq:
 *   inspect() -> String
 *
 * Return a developer-friendly string containing the civil
 * date for the receiver.  Example:
 *
 *   Date.civil(2009, 1, 2).inspect
 *   # => "#<Date 2009-01-02>"
 */
static VALUE rhrd_inspect(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "#<Date %04li-%02i-%02i>",
        d->year, (int)d->month, (int)d->day);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#inspect (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

/* call-seq:
 *   jd() -> Integer
 *
 * Return the julian day number for the receiver as an +Integer+.
 *
 *   Date.civil(2009, 1, 2).jd
 *   # => 2454834
 */
static VALUE rhrd_jd(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return LONG2NUM(d->jd);
}

/* call-seq:
 *   julian?() -> false
 *
 * This library always uses the Gregorian calendar, so this
 * always returns +false+.
 */
static VALUE rhrd_julian_q(VALUE self) {
  return Qfalse;
}

/* call-seq:
 *   ld() -> Integer
 *
 * Return the number of days since the Lilian Date (the day of calendar reform
 * in Italy).
 *
 *   Date.civil(2009, 1, 2).ld
 *   # => 155674
 */
static VALUE rhrd_ld(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return LONG2NUM(d->jd - RHR_JD_LD);
}

/* call-seq:
 *   leap?() -> true or false
 *
 * Return +true+ if the current year for this date is a leap year
 * in the Gregorian calendar, +false+ otherwise.
 *
 *   Date.civil(2009, 1, 2).leap?
 *   # => false
 *   Date.civil(2008, 1, 2).leap?
 *   # => true
 */
static VALUE rhrd_leap_q(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return rhrd__leap_year(d->year) ? Qtrue : Qfalse;
}

/* call-seq:
 *   mjd() -> Integer
 *
 * Return the number of days since 1858-11-17.
 *
 *   Date.civil(2009, 1, 2).mjd
 *   # => 54833
 */
static VALUE rhrd_mjd(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return LONG2NUM(d->jd - RHR_JD_MJD);
}

/* call-seq:
 *   month() -> Integer
 *
 * Returns the number of the month as an +Integer+. Example:
 * 
 *   Date.civil(2009, 1, 2).month
 *   # => 1
 */
static VALUE rhrd_month(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return LONG2NUM(d->month);
}

/* call-seq:
 *   next() -> Date
 *
 * Returns the +Date+ after the receiver's date:
 * 
 *   Date.civil(2009, 1, 2).next
 *   # => #<Date 2009-01-03>
 */
static VALUE rhrd_next(VALUE self) {
   return rhrd__add_days(self, 1);
}

/* call-seq:
 *   new_start(sg=nil) -> Date
 *
 * Returns self. Ignores an argument if given.
 */
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

/* call-seq:
 *   start() -> Integer
 *
 * Returns a number lower than the lowest julian date that can be
 * correctly handled.  Because this library always uses the Gregorian
 * calendar, the day of calendar reform is chosen to be less than any
 * date that this library can handle.
 */
static VALUE rhrd_start(VALUE self) {
  return rhrd_start_num;
}

/* call-seq:
 *   step(target, step=1){|date|} -> Date
 *
 * Yields +Date+ objects between the receiver and the +target+ date
 * (inclusive), with +step+ integer days between each yielded date.
 * +step+ can be negative, in which case the dates are yielded in
 * reverse chronological order.  Returns self in all cases.
 *
 * If +target+ is equal to the receiver, yields self once regardless of
 * +step+. It +target+ is less than receiver and +step+ is nonnegative, or
 * +target+ is greater than receiver and +step+ is nonpositive, does not
 * yield.
 * 
 *   Date.civil(2009, 1, 2).step(Date.civil(2009, 1, 6), 2) do |date|
 *     puts date
 *   end
 *   # Output:
 *   # 2009-01-02
 *   # 2009-01-04
 *   # 2009-01-06
 */
static VALUE rhrd_step(int argc, VALUE *argv, VALUE self) {
  rhrd_t *d, *n, *newd;
  rhrdt_t *ndt;
  long step, limit, current;
  VALUE rlimit, new, rstep, klass;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)

  switch(argc) {
    case 1:
      step = 1;
      rstep = LONG2NUM(step);
      break;
    case 2:
      rstep = argv[1];
      step = NUM2LONG(rstep);
      if (step == 0) {
        rb_raise(rb_eArgError, "step can't be 0");
      }
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 2", argc);
      break;
  }
  rlimit = argv[0];
  klass = rb_obj_class(self);

#ifdef RUBY19
  if (!rb_block_given_p()) {
    return rb_funcall(self, rhrd_id_to_enum, 3, rhrd_sym_step, rlimit, rstep);
  }
#else
  rb_need_block();
#endif

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
        new = Data_Make_Struct(klass, rhrd_t, NULL, -1, newd);
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
        new = Data_Make_Struct(klass, rhrd_t, NULL, -1, newd);
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

/* call-seq:
 *   strftime() -> String <br />
 *   strftime(format) -> String
 *
 * If no argument is provided, returns a string in ISO8601 format, just like
 * +to_s+.  If an argument is provided, uses it as a format string and returns
 * a +String+ based on the format.  The following formats parts are supported:
 *
 * %a :: The abbreviated day name (e.g. Fri)
 * %A :: The full day name (e.g. Friday)
 * %b, %h :: The abbreviated month name (e.g. Jan)
 * %B :: The full month name (e.g. January)
 * %c :: A full date and time representation (e.g. Fri Jan 02 13:29:39 2009)
 * %C :: The century of the year (e.g. 20)
 * %d :: The day of the month, with a leading zero if necessary (e.g. 02)
 * %e :: The day of the month, with a leading space if necessary (e.g.  2)
 * %F :: An ISO8601 date representation (e.g.  2009-01-02)
 * %g :: The last 2 digits of the commercial week year (e.g. 09)
 * %G :: The commercial week year (e.g. 2009)
 * %H :: The hour of the day in 24 hour format, with a leading zero if necessary (e.g. 13)
 * %I :: The hour of the day in 12 hour format (e.g. 01)
 * %j :: The day of the year (e.g. 002)
 * %k :: The hour of the day in 24 hour format, with a leading space if necessary (e.g. 13)
 * %l :: The hour of the day in 12 hour format, with a leading space if necessary (e.g. 13)
 * %L :: The number of milliseconds in the fractional second, with leading zeros if necessary (e.g. 079) 
 * %m :: The month of the year (e.g. 01)
 * %M :: The minute of the hour (e.g. 29)
 * %n :: A newline (e.g. "\n")
 * %N :: The number of nanoseconds in the fractional second, with leading zeros if necessary (e.g. 079013023) 
 * %p :: The meridian indicator, upcased (e.g. PM)
 * %P :: The meridian indicator, downcased (e.g. pm)
 * %Q :: The number of milliseconds since the unix epoch (e.g. 1230902979079)
 * %r :: A full time representation in 12 hour format (e.g.  1:29:39 PM)
 * %R :: An hour and minute representation in 24 hour format (e.g. 13:29)
 * %s :: The number of seconds since the unix epoch (e.g. 1230902979)
 * %S :: The second of the minute (e.g. 39)
 * %t :: A tab (e.g. "\t")
 * %T, %X :: A full time representation in 24 hour format (e.g. 13:29:39)
 * %u :: The commercial week day (e.g. 5)
 * %U :: The week number of the current year, with Sunday as the first day of the first week (e.g. 0)
 * %v :: A full date representation (e.g.  2-Jan-2009)
 * %V :: The commercial week (e.g. 01)
 * %w :: The day of the week, with Sunday as 0 and Saturday as 6 (e.g. 5)
 * %W :: The week number of the current year, with Monday as the first day of the first week (e.g. 0)
 * %x, %D :: A full date representation in month/day/year format (e.g. 01/02/09)
 * %y :: The last two digits of the year (e.g. 09)
 * %Y :: The year (e.g. 2009)
 * %z :: The offset from UTC, without a colon (e.g. +0000)
 * %Z :: The offset from UTC, with a colon (e.g. +00:00)
 * %+ :: A full date and time representation, including the offset (e.g. Fri Jan  2 13:29:39 +00:00 2009)
 *
 * All other formats (e.g. %f, %%) are handled by removing the leading percent sign.  All other text is
 * passed through literally.
 */
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

/* call-seq:
 *   to_s() -> String
 *
 * Returns the receiver as an ISO8601 formatted string.
 * 
 *   Date.civil(2009, 1, 2).to_s
 *   # => "2009-01-02"
 */
static VALUE rhrd_to_s(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%04li-%02i-%02i",
        d->year, (int)d->month, (int)d->day);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#to_s (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

/* call-seq:
 *   upto(target){|date|} -> Date
 *
 * Equivalent to calling +step+ with the +target+ as the first argument.
 * Returns self.
 * 
 *   Date.civil(2009, 1, 1).upto(Date.civil(2009, 1, 2)) do |date|
 *     puts date
 *   end
 *   # Output:
 *   # 2009-01-01
 *   # 2009-01-02
 */
static VALUE rhrd_upto(VALUE self, VALUE other) {
  VALUE argv[1];
  argv[0] = other;
  return rhrd_step(1, argv, self);
}

/* call-seq:
 *   wday() -> Integer
 *
 * Returns the day of the week as an +Integer+, where Sunday
 * is 0 and Saturday is 6. Example:
 * 
 *   Date.civil(2009, 1, 2).wday
 *   # => 5
 */
static VALUE rhrd_wday(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return LONG2NUM(rhrd__jd_to_wday(d->jd));
}

/* call-seq:
 *   yday() -> Integer
 *
 * Returns the day of the year as an +Integer+, where January
 * 1st is 1 and December 31 is 365 (or 366 if the year is a leap
 * year). Example:
 * 
 *   Date.civil(2009, 2, 2).yday
 *   # => 33
 */
static VALUE rhrd_yday(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return LONG2NUM(rhrd__ordinal_day(d->year, d->month, d->day));
}

/* call-seq:
 *   year() -> Integer
 *
 * Returns the year as an +Integer+. Example:
 * 
 *   Date.civil(2009, 1, 2).year
 *   # => 2009
 */
static VALUE rhrd_year(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return LONG2NUM(d->year);
}

/* Ruby instance operator methods */

/* call-seq:
 *   >>(n) -> Date
 *
 * Returns a +Date+ that is +n+ months after the receiver. +n+
 * can be negative, in which case it returns a +Date+ before
 * the receiver.
 * 
 *   Date.civil(2009, 1, 2) >> 2
 *   # => #<Date 2009-01-02>
 *   Date.civil(2009, 1, 2) >> -2
 *   # => #<Date 2008-11-02>
 */
static VALUE rhrd_op_right_shift(VALUE self, VALUE other) {
  return rhrd__add_months(self, NUM2LONG(other));
}

/* call-seq:
 *   <<(n) -> Date
 *
 * Returns a +Date+ that is +n+ months before the receiver. +n+
 * can be negative, in which case it returns a +Date+ after
 * the receiver.
 * 
 *   Date.civil(2009, 1, 2) << 2
 *   # => #<Date 2008-11-02>
 *   Date.civil(2009, 1, 2) << -2
 *   # => #<Date 2009-01-02>
 */
static VALUE rhrd_op_left_shift(VALUE self, VALUE other) {
  return rhrd__add_months(self, -NUM2LONG(other));
}

/* call-seq:
 *   +(n) -> Date
 *
 * Returns a +Date+ that is +n+ days after the receiver. +n+
 * can be negative, in which case it returns a +Date+ before
 * the receiver.
 * 
 *   Date.civil(2009, 1, 2) + 2
 *   # => #<Date 2009-01-04>
 *   Date.civil(2009, 1, 2) + -2
 *   # => #<Date 2008-12-31>
 */
static VALUE rhrd_op_plus(VALUE self, VALUE other) {
   return rhrd__add_days(self, NUM2LONG(other));
}

/* call-seq:
 *   -(n) -> Date <br />
 *   -(date) -> Integer <br />
 *   -(datetime) -> Float
 *
 * If a +Numeric+ argument is given, it is treated as an +Integer+,
 * and the number of days it represents is substracted from the
 * receiver to return a new +Date+ object. +n+ can be negative, in
 * which case the +Date+ returned will be after the receiver.
 *
 * If a +Date+ argument is given, returns the number of days
 * between the current date and the argument as an +Integer+.
 *
 * If a +DateTime+ argument is given, returns the number of days
 * between the current date and the argument as a +Float+, and
 * considers the receiver to be in the same time zone as the
 * argument.
 * 
 * Other types of arguments raise a +TypeError+.
 * 
 *   Date.civil(2009, 1, 2) - 2
 *   # => #<Date 2008-12-31>
 *   Date.civil(2009, 1, 2) - Date.civil(2009, 1, 1)
 *   # => 1
 *   Date.civil(2009, 1, 2) - DateTime.civil(2009, 1, 3, 12)
 *   # => -1.5
 */
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
    return rb_float_new(d->jd - (newdt->jd + (double)newdt->nanos/RHR_NANOS_PER_DAYD));
  }
  if (RTEST((rb_obj_is_kind_of(other, rhrd_class)))) {
    Data_Get_Struct(other, rhrd_t, newd);
    RHR_FILL_JD(d)
    RHR_FILL_JD(newd)
    return LONG2NUM(rhrd__safe_add_long(d->jd, -newd->jd));
  }
  rb_raise(rb_eTypeError, "expected numeric or date");
}

/* call-seq:
 *   ===(other) -> true or false
 *
 * If +other+ is a +Date+, returns +true+ if +other+ is the
 * same date as the receiver, or +false+ otherwise.
 *
 * If +other+ is a +DateTime+, return +true+ if +other has the same
 * julian date as the receiver, or +false+ otherwise.
 *
 * If +other+ is a +Numeric+, convert it to an +Integer+ and return
 * +true+ if it is equal to the receiver's julian date, or +false+
 * otherwise. 
 */
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

/* call-seq:
 *   <=>(other) -> -1, 0, 1, or nil
 *
 * If +other+ is a +Date+, returns -1 if +other+ is before the
 * the receiver chronologically, 0 if +other+ is the same date as the receiver,
 * or 1 if +other+ is after the receiver chronologically.
 *
 * If +other+ is a +DateTime+, return 0 if +other+ has the same
 * julian date as the receiver and no fractional part, -1 if +other+ has a julian date
 * less than the receiver's, and 1 if +other+ has a julian date
 * greater than the receiver's or a julian date the same as the
 * receiver's and a fractional part. 
 *
 * If +other+ is a +Numeric+, convert it to an integer and compare
 * it to the receiver's julian date. 
 *
 * For an unrecognized type, return +nil+.
 */
static VALUE rhrd_op_spaceship(VALUE self, VALUE other) {
  if (RTEST(rb_obj_is_kind_of(self, rhrdt_class))) {
    rhrdt_t *dt, *odt;
    rhrd_t *od;
    double diff;
    int res;

    if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
      self = rhrdt__new_offset(self, 0.0);
      other = rhrdt__new_offset(other, 0.0);
      Data_Get_Struct(self, rhrdt_t, dt);
      Data_Get_Struct(other, rhrdt_t, odt);
      return LONG2NUM(rhrdt__spaceship(dt, odt));
    }
    if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
      Data_Get_Struct(self, rhrdt_t, dt);
      Data_Get_Struct(other, rhrd_t, od);
      RHRDT_FILL_JD(dt)
      RHR_FILL_JD(od)
      RHR_SPACE_SHIP(res, dt->jd, od->jd)
      if (res == 0) {
        RHRDT_FILL_NANOS(dt)
        RHR_SPACE_SHIP(res, dt->nanos, 0)
      }
      return LONG2NUM(res);
    }
    if (RTEST((rb_obj_is_kind_of(other, rb_cNumeric)))) {
      Data_Get_Struct(self, rhrdt_t, dt);
      diff = NUM2DBL(other);
      RHRDT_FILL_JD(dt)
      RHR_SPACE_SHIP(res, dt->jd, (long)diff)
      if (res == 0) {
        RHRDT_FILL_NANOS(dt)
        RHR_SPACE_SHIP(res, dt->nanos, llround((diff - floor(diff)) * RHR_NANOS_PER_DAY))
      }
      return LONG2NUM(res);
    }
  } else {
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
  }

  return Qnil;
}

#ifdef RUBY19

/* Ruby 1.9 helper methods */

/* Add n number of years to the given ruby Date object.  n can
 * be negative to subtract years.  Returns a new ruby Date
 * object */
VALUE rhrd__add_years(VALUE self, long n) {
  rhrd_t *d;
  rhrd_t *newd;
  VALUE new;
  Data_Get_Struct(self, rhrd_t, d);

  new = Data_Make_Struct(rb_obj_class(self), rhrd_t, NULL, -1, newd);
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

/* Return ruby true if the given date falls on the given
 * week day number, or ruby false otherwise. */
VALUE rhrd__day_q(VALUE self, long day) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return rhrd__jd_to_wday(d->jd) == day ? Qtrue : Qfalse;
}

/* Ruby 1.9 class methods */

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   httpdate() -> Date <br />
 *   httpdate(str, sg=nil) -> Date
 *
 * If no argument is given, returns a +Date+ for julian day 0.
 * If an argument is given, it should be a string that is
 * parsed using +_httpdate+, returning a +Date+ or raising
 * an +ArgumentError+ if the string is not in a valid format
 * or the date it represents is not a valid date.
 * Ignores the 2nd argument.
 * Example:
 * 
 *   Date.httpdate("Fri, 02 Jan 2009 00:00:00 GMT")
 *   # => #<Date 2009-01-02>
 */
static VALUE rhrd_s_httpdate(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(klass, rb_funcall(klass, rhrd_id__httpdate, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   iso8601() -> Date <br />
 *   iso8601(str, sg=nil) -> Date
 *
 * If no argument is given, returns a +Date+ for julian day 0.
 * If an argument is given, it should be a string that is
 * parsed using +_iso8601+, returning a +Date+ or raising
 * an +ArgumentError+ if the string is not in a valid format
 * or the date it represents is not a valid date.
 * Ignores the 2nd argument.
 * Example:
 * 
 *   Date.iso8601("2009-01-02")
 *   # => #<Date 2009-01-02>
 */
static VALUE rhrd_s_iso8601(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(klass, rb_funcall(klass, rhrd_id__iso8601, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   jisx0301() -> Date <br />
 *   jisx0301(str, sg=nil) -> Date
 *
 * If no argument is given, returns a +Date+ for julian day 0.
 * If an argument is given, it should be a string that is
 * parsed using +_jisx0301+, returning a +Date+ or raising
 * an +ArgumentError+ if the string is not in a valid format
 * or the date it represents is not a valid date.
 * Ignores the 2nd argument.
 * Example:
 * 
 *   Date.iso8601("H21.01.02")
 *   # => #<Date 2009-01-02>
 */
static VALUE rhrd_s_jisx0301(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(klass, rb_funcall(klass, rhrd_id__jisx0301, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   rfc2822() -> Date <br />
 *   rfc2822(str, sg=nil) -> Date
 *
 * If no argument is given, returns a +Date+ for julian day 0.
 * If an argument is given, it should be a string that is
 * parsed using +_rfc2822+, returning a +Date+ or raising
 * an +ArgumentError+ if the string is not in a valid format
 * or the date it represents is not a valid date.
 * Ignores the 2nd argument.
 * Example:
 * 
 *   Date.rfc2822("Fri, 2 Jan 2009 00:00:00 +0000")
 *   # => #<Date 2009-01-02>
 */
static VALUE rhrd_s_rfc2822(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(klass, rb_funcall(klass, rhrd_id__rfc2822, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   rfc3339() -> Date <br />
 *   rfc3339(str, sg=nil) -> Date
 *
 * If no argument is given, returns a +Date+ for julian day 0.
 * If an argument is given, it should be a string that is
 * parsed using +_rfc3339+, returning a +Date+ or raising
 * an +ArgumentError+ if the string is not in a valid format
 * or the date it represents is not a valid date.
 * Ignores the 2nd argument.
 * Example:
 * 
 *   Date.rfc3339("2009-01-02T00:00:00+00:00")
 *   # => #<Date 2009-01-02>
 */
static VALUE rhrd_s_rfc3339(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(klass, rb_funcall(klass, rhrd_id__rfc3339, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   xmlschema() -> Date <br />
 *   xmlschema(str, sg=nil) -> Date
 *
 * If no argument is given, returns a +Date+ for julian day 0.
 * If an argument is given, it should be a string that is
 * parsed using +_xmlschema+, returning a +Date+ or raising
 * an +ArgumentError+ if the string is not in a valid format
 * or the date it represents is not a valid date.
 * Ignores the 2nd argument.
 * Example:
 * 
 *   Date.xmlschema("2009-01-02")
 *   # => #<Date 2009-01-02>
 */
static VALUE rhrd_s_xmlschema(int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd;

  switch(argc) {
    case 0:
      rd = Data_Make_Struct(klass, rhrd_t, NULL, -1, d);
      d->flags = RHR_HAVE_JD;
      return rd;
    case 1:
    case 2:
      return rhrd__from_hash(klass, rb_funcall(klass, rhrd_id__xmlschema, 1, argv[0]));
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

/* Ruby 1.9 instance methods */

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   httpdate() -> String
 *
 * Returns the receiver as a +String+ in HTTP format. Example:
 * 
 *   Date.civil(2009, 1, 2).httpdate
 *   # => "Fri, 02 Jan 2009 00:00:00 GMT"
 */
static VALUE rhrd_httpdate(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s, %02i %s %04li 00:00:00 GMT", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        (int)d->day,
        rhrd__abbr_month_names[d->month],
        d->year);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#httpdate (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   jisx0301() -> String
 *
 * Returns the receiver as a +String+ in JIS X 0301 format. Example:
 * 
 *   Date.civil(2009, 1, 2).jisx0301
 *   # => "H21.01.02"
 */
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
    len = snprintf(RSTRING_PTR(s), 128, "%04li-%02i-%02i", d->year, (int)d->month, (int)d->day);
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
    len = snprintf(RSTRING_PTR(s), 128, "%c%02li.%02i.%02i", c, year, (int)d->month, (int)d->day);
  }
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#jisx0301 (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   next_day(n=1) -> Date
 *
 * Returns a +Date+ +n+ days after the receiver.  If +n+ is negative,
 * returns a +Date+ before the receiver.
 * 
 *   Date.civil(2009, 1, 2).next_day
 *   # => #<Date 2009-01-03>
 *   Date.civil(2009, 1, 2).next_day(2)
 *   # => #<Date 2009-01-04>
 */
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

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   next_month(n=1) -> Date
 *
 * Returns a +Date+ +n+ months after the receiver.  If +n+ is negative,
 * returns a +Date+ before the receiver.
 * 
 *   Date.civil(2009, 1, 2).next_month
 *   # => #<Date 2009-02-02>
 *   Date.civil(2009, 1, 2).next_month(2)
 *   # => #<Date 2009-03-02>
 */
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

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   next_year(n=1) -> Date
 *
 * Returns a +Date+ +n+ years after the receiver.  If +n+ is negative,
 * returns a +Date+ before the receiver.
 * 
 *   Date.civil(2009, 1, 2).next_year
 *   # => #<Date 2010-01-02>
 *   Date.civil(2009, 1, 2).next_year(2)
 *   # => #<Date 2011-01-02>
 */
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

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   prev_day(n=1) -> Date
 *
 * Returns a +Date+ +n+ days before the receiver.  If +n+ is negative,
 * returns a +Date+ after the receiver.
 * 
 *   Date.civil(2009, 1, 2).prev_day
 *   # => #<Date 2009-01-01>
 *   Date.civil(2009, 1, 2).prev_day(2)
 *   # => #<Date 2008-12-31>
 */
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

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   prev_month(n=1) -> Date
 *
 * Returns a +Date+ +n+ months before the receiver.  If +n+ is negative,
 * returns a +Date+ after the receiver.
 * 
 *   Date.civil(2009, 1, 2).prev_month
 *   # => #<Date 2008-12-02>
 *   Date.civil(2009, 1, 2).prev_month(2)
 *   # => #<Date 2008-11-02>
 */
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

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   prev_year(n=1) -> Date
 *
 * Returns a +Date+ +n+ years before the receiver.  If +n+ is negative,
 * returns a +Date+ after the receiver.
 * 
 *   Date.civil(2009, 1, 2).prev_year
 *   # => #<Date 2008-01-02>
 *   Date.civil(2009, 1, 2).prev_year(2)
 *   # => #<Date 2007-01-02>
 */
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

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   rfc2822() -> String
 *
 * Returns the receiver as a +String+ in RFC2822 format. Example:
 * 
 *   Date.civil(2009, 1, 2).rfc2822
 *   # => "Fri, 2 Jan 2009 00:00:00 +0000"
 */
static VALUE rhrd_rfc2822(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s, %i %s %04li 00:00:00 +0000", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        (int)d->day,
        rhrd__abbr_month_names[d->month],
        d->year);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#rfc2822 (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   rfc3339() -> String
 *
 * Returns the receiver as a +String+ in RFC3339 format. Example:
 * 
 *   Date.civil(2009, 1, 2).rfc3339
 *   # => "2009-01-02T00:00:00+00:00"
 */
static VALUE rhrd_rfc3339(VALUE self) {
  VALUE s;
  rhrd_t *d;
  int len;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%04li-%02i-%02iT00:00:00+00:00", d->year, (int)d->month, (int)d->day);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#rfc3339 (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   to_datetime() -> DateTime
 *
 * Returns a +DateTime+ equal to the receiver.
 * 
 *   Date.civil(2009, 1, 2).to_datetime
 *   # => #<DateTime 2009-01-02T00:00:00+00:00>
 */
static VALUE rhrd_to_datetime(VALUE self) {
  rhrd_t *d;
  rhrdt_t *dt;
  VALUE rdt = Data_Make_Struct(rhrdt_class, rhrdt_t, NULL, -1, dt);
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

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   to_time() -> Time
 *
 * Returns a +Time+ in local time with the same year, month, and day
 * as the receiver.
 * 
 *   Date.civil(2009, 1, 2).to_time
 *   # => 2009-01-02 00:00:00 -0800
 */
static VALUE rhrd_to_time(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return rb_funcall(rb_cTime, rhrd_id_local, 3, LONG2NUM(d->year), LONG2NUM(d->month), LONG2NUM(d->day));
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   to_date() -> Date
 *
 * Returns a +Date+ with the same year, month, and day
 * as the receiver in local time.
 * 
 *   Time.local(2009, 1, 2).to_date
 *   # => #<Date 2009-01-02>
 */
static VALUE rhrd_time_to_date(VALUE self) {
  rhrd_t *d;
  VALUE rd;
  rd = Data_Make_Struct(rhrd_class, rhrd_t, NULL, -1, d);
  d->jd = rhrd__unix_to_jd(NUM2LONG(rb_funcall(self, rhrd_id_to_i, 0)) + NUM2LONG(rb_funcall(self, rhrd_id_utc_offset, 0)));
  d->flags = RHR_HAVE_JD;
  RHR_CHECK_JD(d)
  return rd;
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   to_time() -> Time
 *
 * Returns a copy of the receiver in local time.
 * 
 *   Time.local(2009, 1, 2).to_time
 *   # => 2009-01-02 00:00:00 -0800
 *   Time.local(2009, 1, 2).getutc.to_time
 *   # => 2009-01-02 00:00:00 -0800
 */
static VALUE rhrd_time_to_time(VALUE self) {
  return rb_funcall(self, rhrd_id_getlocal, 0);
}

/* Ruby 1.9 *day? instance methods */

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   sunday?() -> true or false
 *
 * Returns +true+ if the receiver is a Sunday, +false+ otherwise.
 */
static VALUE rhrd_sunday_q(VALUE self) {
  return rhrd__day_q(self, 0);
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   monday?() -> true or false
 *
 * Returns +true+ if the receiver is a Monday, +false+ otherwise.
 */
static VALUE rhrd_monday_q(VALUE self) {
  return rhrd__day_q(self, 1);
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   tuesday?() -> true or false
 *
 * Returns +true+ if the receiver is a Tuesday, +false+ otherwise.
 */
static VALUE rhrd_tuesday_q(VALUE self) {
  return rhrd__day_q(self, 2);
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   wednesday?() -> true or false
 *
 * Returns +true+ if the receiver is a Wednesday, +false+ otherwise.
 */
static VALUE rhrd_wednesday_q(VALUE self) {
  return rhrd__day_q(self, 3);
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   thursday?() -> true or false
 *
 * Returns +true+ if the receiver is a Thursday, +false+ otherwise.
 */
static VALUE rhrd_thursday_q(VALUE self) {
  return rhrd__day_q(self, 4);
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   friday?() -> true or false
 *
 * Returns +true+ if the receiver is a Friday, +false+ otherwise.
 */
static VALUE rhrd_friday_q(VALUE self) {
  return rhrd__day_q(self, 5);
}

/* call-seq:
 *   [ruby 1-9 only] <br />
 *   saturday?() -> true or false
 *
 * Returns +true+ if the receiver is a Saturday, +false+ otherwise.
 */
static VALUE rhrd_saturday_q(VALUE self) {
  return rhrd__day_q(self, 6);
}

#else

/* Ruby 1.8 class methods */

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   ajd_to_amjd(ajd) -> Integer
 *
 * Converts the given astronomical julian date (+Integer+) into an
 * astronomical modified julian date.
 */
static VALUE rhrd_s_ajd_to_amjd(VALUE klass, VALUE ajd) {
  return LONG2NUM(rhrd__safe_add_long(-RHR_JD_MJD, NUM2LONG(ajd)));
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   ajd_to_jd(ajd) -> [jd, Float(1)/2]
 *
 * Converts the given astronomical julian date (+Integer+) into an
 * an array of two elements, where the first element is the julian
 * date +Integer+ and the second is a +Float+ with value 0.5.
 */
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

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   amjd_to_ajd(ajd) -> Integer
 *
 * Converts the given astronomical modified julian date (+Integer+) into an
 * astronomical julian date.
 */
static VALUE rhrd_s_amjd_to_ajd(VALUE klass, VALUE amjd) {
  return LONG2NUM(rhrd__safe_add_long(RHR_JD_MJD - 1, NUM2LONG(amjd)));
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   civil_to_jd(year, month, day, sg=nil) -> Integer
 *
 * Converts the given year, month, and day into a julian date +Integer+.
 * Ignores the 4th argument.
 */
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

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   civil_to_jd(cwyear, cweek, cwday, sg=nil) -> Integer
 *
 * Converts the given cwyear, cweek, and cwday into a julian date +Integer+.
 * Ignores the 4th argument.
 */
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

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   day_fraction_to_time(float) -> [hour, minute, second, sec_fraction]
 *
 * Converts the given float (which should be in the range [0.0, 1.0))
 * into an array of 4 elements: +hour+ (+Integer+), +minute+ (+Integer+),
 * +second+ (+Integer+), and +sec_fraction+ (+Float+).  Note that
 * +sec_fraction+ is the fraction of the second as a fraction of the day,
 * so it will be in the range [0.0, 1.0/86400.0).
 */
static VALUE rhrd_s_day_fraction_to_time(VALUE klass, VALUE rf) {
  double f;
  int h, m, s;
 
  f = NUM2DBL(rf) * 24;
  h = (int)floor(f);
  f = (f - h) * 60;
  m = (int)floor(f);
  f = (f - m) * 60;
  s = (int)floor(f);
  f = (f - s)/RHR_SECONDS_PER_DAY;
  return rb_ary_new3(4, LONG2NUM(h), LONG2NUM(m), LONG2NUM(s), rb_float_new(f));
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   gregorian?(jd, sg) -> true or false
 *
 * If +sg+ is +nil+ or +false+, returns +false+.  If +sg+ is +Numeric+,
 * returns +true+ if +jd+ is greater than or equal to +sg+, and +false+
 * otherwise.  If +sg+ is not +Numeric+, +nil+, or +false+, returns +true+.
 */
static VALUE rhrd_s_gregorian_q(VALUE klass, VALUE jd, VALUE sg) {
  if (RTEST((rb_obj_is_kind_of(sg, rb_cNumeric)))) {
    return rb_funcall(jd, rhrd_id_op_gte, 1, sg);
  } else {
    return RTEST(sg) ? Qtrue : Qfalse;
  }
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   jd_to_ajd(jd, rf, of=nil) -> Integer
 *
 * Returns +jd+. Ignores the 2nd and 3rd arguments.
 */
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

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   jd_to_civil(jd, sg=nil) -> [year, month, day]
 *
 * Converts +jd+ to an array with 3 +Integer+ values: +year+, +month+,
 * and +day+.  Ignores the 2nd argument.
 */
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

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   jd_to_commercial(jd, sg=nil) -> [cwyear, cweek, cwday]
 *
 * Converts +jd+ to an array with 3 +Integer+ values: +cwyear+, +cweek+,
 * and +cwday+.  Ignores the 2nd argument.
 */
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

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   jd_to_ld(jd) -> Integer
 *
 * Converts +jd+ to a Lilian Date (the number of days since the day of calendar
 * reform in Italy).
 */
static VALUE rhrd_s_jd_to_ld(VALUE klass, VALUE jd) {
  return LONG2NUM(rhrd__safe_add_long(-RHR_JD_LD, NUM2LONG(jd)));
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   jd_to_mjd(jd) -> Integer
 *
 * Converts +jd+ to a modified julian date +Integer+.
 */
static VALUE rhrd_s_jd_to_mjd(VALUE klass, VALUE jd) {
  return LONG2NUM(rhrd__safe_add_long(-RHR_JD_MJD, NUM2LONG(jd)));
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   jd_to_ordinal(jd, sg=nil) -> [year, yday]
 *
 * Converts +jd+ to an array with 2 +Integer+ values: +year+ and +yday+ (day of year).
 * Ignores the 2nd argument.
 */
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

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   jd_to_wday(jd) -> Integer
 *
 * Converts +jd+ to an +Integer+ day of the week, where 0 represents Sunday
 * and 6 represents Saturday.
 */
static VALUE rhrd_s_jd_to_wday(VALUE klass, VALUE jd) {
  return LONG2NUM(rhrd__jd_to_wday(NUM2LONG(jd)));
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   julian?(jd, sg) -> true or false
 *
 * If +sg+ is +nil+ or +false+, returns +true+.  If +sg+ is +Numeric+,
 * returns +true+ if +jd+ is less than +sg+, and +false+
 * otherwise.  If +sg+ is not +Numeric+, +nil+, or +false+, returns +false+.
 */
static VALUE rhrd_s_julian_q(VALUE klass, VALUE jd, VALUE sg) {
  if (RTEST((rb_obj_is_kind_of(sg, rb_cNumeric)))) {
    return rb_funcall(jd, rhrd_id_op_lt, 1, sg);
  } else {
    return RTEST(sg) ? Qfalse : Qtrue;
  }
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   ld_to_jd(ld) -> Integer
 *
 * Converts +ld+ (a Lilian Date) to a julian day +Integer+.
 */
static VALUE rhrd_s_ld_to_jd(VALUE klass, VALUE ld) {
  return LONG2NUM(rhrd__safe_add_long(RHR_JD_LD, NUM2LONG(ld)));
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   mjd_to_jd(mjd) -> Integer
 *
 * Converts a modified julian date number to a julian day +Integer+.
 */
static VALUE rhrd_s_mjd_to_jd(VALUE klass, VALUE mjd) {
  return LONG2NUM(rhrd__safe_add_long(RHR_JD_MJD, NUM2LONG(mjd)));
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   ordinal_to_jd(year, yday, sg=nil) -> Integer
 *
 * Converts the given +year+ and +yday+ (day of year) into a julian day +Integer+.
 * Ignores the 3rd argument.
 */
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

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   time_to_day_fraction(hour, minute, second) -> Float
 *
 * Converts the given +hour+, +minute+, and +second+ into a single +Float+ representing
 * the fraction of the day, such that 12 hours, 0 minutes, and 0 seconds would return
 * 0.5.
 */
static VALUE rhrd_s_time_to_day_fraction(VALUE klass, VALUE h, VALUE m, VALUE s) {
  return rb_float_new(NUM2DBL(h)/24.0 + NUM2DBL(m)/RHR_MINUTES_PER_DAYD + NUM2DBL(s)/RHR_SECONDS_PER_DAYD);
}

/* call-seq:
 *   [ruby 1-8 only] <br />
 *   valid_time?(hour, minute, second) -> Float
 *
 * Checks that the hour, minute, and second are a valid time.  This handles negative
 * values for all 3 arguments, so that -10 minutes is treated as 50 minutes.  It returns
 * a +Float+ representing the fraction of the day for the given values.
 */
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
  return rb_float_new(h/24.0 + m/RHR_MINUTES_PER_DAYD + s/RHR_SECONDS_PER_DAYD);
}

#endif

/* Ruby Library Initialization */

/* +Date+ is used to store a single date in the gregorian calendar.
 *
 * In general, +Date+ objects are created by calling one of the class
 * methods: +civil+, +parse+, +strptime+, +today+.  Once created,
 * +Date+ objects are immutable.  Operations that result in a separate
 * date (such as adding a number of days), always return a new +Date+
 * object.
 * */
void Init_date_ext(void) {
  int i;
  VALUE monthnames[13];
  VALUE abbr_monthnames[13];
  VALUE daynames[7];
  VALUE abbr_daynames[7];

  /* Setup static IDs and symbols */

  rhrd_id_op_array = rb_intern("[]");
  rhrd_id_op_gte = rb_intern(">=");
  rhrd_id_op_lt = rb_intern("<");
  rhrd_id__parse = rb_intern("_parse");
  rhrd_id_cwday = rb_intern("cwday");
  rhrd_id_cweek = rb_intern("cweek");
  rhrd_id_cwyear = rb_intern("cwyear");
  rhrd_id_downcase = rb_intern("downcase");
  rhrd_id_getlocal = rb_intern("getlocal");
  rhrd_id_hash = rb_intern("hash");
  rhrd_id_length = rb_intern("length");
  rhrd_id_include_q = rb_intern("include?");
  rhrd_id_local = rb_intern("local");
  rhrd_id_localtime = rb_intern("localtime");
  rhrd_id_match = rb_intern("match");
  rhrd_id_now = rb_intern("now");
  rhrd_id_offset = rb_intern("offset");
  rhrd_id_slice = rb_intern("slice");
  rhrd_id_split = rb_intern("split");
  rhrd_id_sub_b = rb_intern("sub!");
  rhrd_id_to_enum = rb_intern("to_enum");
  rhrd_id_to_i = rb_intern("to_i");
#ifdef RUBY19
  rhrd_id_nsec = rb_intern("nsec");
#else
  rhrd_id_usec = rb_intern("usec");
#endif
  rhrd_id_utc = rb_intern("utc");
  rhrd_id_utc_offset = rb_intern("utc_offset");
  rhrd_id_Format = rb_intern("Format");
  rhrd_id_ZONES = rb_intern("ZONES");

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
  rhrd_sym_step = ID2SYM(rb_intern("step"));
  rhrd_sym_wday = ID2SYM(rb_intern("wday"));
  rhrd_sym_wnum0 = ID2SYM(rb_intern("wnum0"));
  rhrd_sym_wnum1 = ID2SYM(rb_intern("wnum1"));
  rhrd_sym_yday = ID2SYM(rb_intern("yday"));
  rhrd_sym_year = ID2SYM(rb_intern("year"));
  rhrd_sym_zone = ID2SYM(rb_intern("zone"));

#ifdef RHR_ENCODING
  rhrd_encoding_index = rb_enc_to_index(rb_usascii_encoding());
#endif

  /* Define classes*/

  rhrd_class = rb_define_class("Date", rb_cObject);
  rb_define_alloc_func(rhrd_class, rhrd_s_allocate);
  rhrd_s_class = rb_singleton_class(rhrd_class);

  /* Define methods for all ruby versions */

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
  rb_define_method(rhrd_class, "clone", rhrd_clone, 0);
  rb_define_method(rhrd_class, "cwday", rhrd_cwday, 0);
  rb_define_method(rhrd_class, "cweek", rhrd_cweek, 0);
  rb_define_method(rhrd_class, "cwyear", rhrd_cwyear, 0);
  rb_define_method(rhrd_class, "day", rhrd_day, 0);
  rb_define_method(rhrd_class, "day_fraction", rhrd_day_fraction, 0);
  rb_define_method(rhrd_class, "downto", rhrd_downto, 1);
  rb_define_method(rhrd_class, "dup", rhrd_dup, 0);
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

  /* Setup static constants */

  monthnames[0] = Qnil;
  abbr_monthnames[0] = Qnil;
  for(i = 1; i < 13; i++) {
    monthnames[i] =  rb_str_new2((const char *)rhrd__month_names[i]);
    abbr_monthnames[i] =  rb_str_new2(rhrd__abbr_month_names[i]);
  }
  rhrd_monthnames = rb_ary_new4(13, monthnames);
  rhrd_abbr_monthnames = rb_ary_new4(13, abbr_monthnames);

  for(i = 0; i < 7; i++) {
    daynames[i] =  rb_str_new2(rhrd__day_names[i]);
    abbr_daynames[i] =  rb_str_new2(rhrd__abbr_day_names[i]);
  }
  rhrd_daynames = rb_ary_new4(7, daynames);
  rhrd_abbr_daynames = rb_ary_new4(7, abbr_daynames);

  rhrd_start_num = LONG2NUM(RHR_JD_MIN - 1);
  rhrd_empty_string = rb_str_new("", 0);
  rhrd_string_colon = rb_str_new(":", 1);

  /* Define some static regexps. The 1 for the options makes
   * it case insensitive, as I don't want to deal with including
   * the ruby regex header just to get it. */
  rhrd_zone_re = rb_reg_new(rhrd__zone_re_str, (int)strlen(rhrd__zone_re_str), 1);
  rhrd_zone_dst_re = rb_reg_new(rhrd__zone_dst_re_str, (int)strlen(rhrd__zone_dst_re_str), 1);
  rhrd_zone_sign_re = rb_reg_new(rhrd__zone_sign_re_str, (int)strlen(rhrd__zone_sign_re_str), 1);
  rhrd_re_comma_period = rb_reg_new("[,.]", 4, 0);

  /* Register global variables with Garbage collector, so users
   * who remove constants can't crash the interpreter. */

  rb_global_variable(&rhrd_monthnames);
  rb_global_variable(&rhrd_abbr_monthnames);
  rb_global_variable(&rhrd_daynames);
  rb_global_variable(&rhrd_abbr_daynames);
  rb_global_variable(&rhrd_start_num);
  rb_global_variable(&rhrd_empty_string);
  rb_global_variable(&rhrd_string_colon);
  rb_global_variable(&rhrd_zone_re);
  rb_global_variable(&rhrd_zone_dst_re);
  rb_global_variable(&rhrd_zone_sign_re);
  rb_global_variable(&rhrd_re_comma_period);

  /* Define constants accessible from ruby */

  /* The julian day number for the day of calendar reform in Italy */
  rb_define_const(rhrd_class, "ITALY", LONG2NUM(RHR_JD_ITALY));
  /* The julian day number for the day of calendar reform in England */
  rb_define_const(rhrd_class, "ENGLAND", LONG2NUM(RHR_JD_ENGLAND));
  /* An integer lower than the lowest supported julian day number */
  rb_define_const(rhrd_class, "GREGORIAN", rhrd_start_num);
  /* An integer higher than the highest supported julian day number */
  rb_define_const(rhrd_class, "JULIAN", LONG2NUM(RHR_JD_MAX + 1));

  /* An array of month names<br />MONTHNAMES[1] => 'January' */
  rb_define_const(rhrd_class, "MONTHNAMES", rhrd_monthnames);
  /* An array of abbreviated month names<br />ABBR_MONTHNAMES[1] => 'Jan' */
  rb_define_const(rhrd_class, "ABBR_MONTHNAMES", rhrd_abbr_monthnames);
  /* An array of day names<br />DAYNAMES[0] => 'Sunday' */
  rb_define_const(rhrd_class, "DAYNAMES", rhrd_daynames);
  /* An array of abbreviated day names<br />ABBR_DAYNAMES[0] => 'Sun' */
  rb_define_const(rhrd_class, "ABBR_DAYNAMES", rhrd_abbr_daynames);

#ifdef RUBY19

  /* Define ruby 1.9 only methods */

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

  /* Define ruby 1.8 only methods */

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

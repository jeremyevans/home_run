
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ruby.h>

#define RHR_DEFAULT_JD 0
#define RHR_DEFAULT_YEAR -4712
#define RHR_DEFAULT_MONTH 1
#define RHR_DEFAULT_DAY 1
#define RHR_JD_MJD 2400001
#define RHR_JD_LD 2299160
#define RHR_JD_ITALY 2299161
#define RHR_JD_ENGLAND 2361222

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

#define RHR_HAVE_JD 0x1
#define RHR_HAVE_CIVIL 0x2

#define RHR_HAS_JD(d) ((d)->flags & RHR_HAVE_JD) == RHR_HAVE_JD
#define RHR_HAS_CIVIL(d) ((d)->flags & RHR_HAVE_CIVIL) == RHR_HAVE_CIVIL

#define RHR_FILL_JD(d) if (((d)->flags & RHR_HAVE_JD) == 0) { rhrd__civil_to_jd(d); }
#define RHR_FILL_CIVIL(d) if (((d)->flags & RHR_HAVE_CIVIL) == 0) { rhrd__jd_to_civil(d); }

#define RHR_SPACE_SHIP(x, l, r) if (l < r) { x = -1; } else if (l == r) { x = 0; } else { x = 1; } 

#define RHR_CHECK_JD(d) if ((d->jd > RHR_JD_MAX) || (d->jd < RHR_JD_MIN)) { rb_raise(rb_eRangeError, "date out of range: jd = %li", d->jd);}
#define RHR_CHECK_CIVIL(d) if (rhrd__check_civil(d)) { rb_raise(rb_eRangeError, "date out of range: year = %li, month = %hhi, day = %hhi", d->year, d->month, d->day);}

typedef struct rhrd_s {
  long jd;
  long year;
  unsigned char month;
  unsigned char day;
  unsigned char flags;
} rhrd_t;

unsigned char rhrd_days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
long rhrd_cumulative_days_in_month[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
char * rhrd_abbr_month_names[13] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
char * rhrd_abbr_day_names[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
VALUE rhrd_class;
VALUE rhrd_s_class;

ID rhrd_id_hash;
ID rhrd_id_mday;
ID rhrd_id_mon;
ID rhrd_id_now;
ID rhrd_id_year;

static VALUE rhrd_step(int argc, VALUE *argv, VALUE self);

/* C Helper Methods */

int rhrd__check_civil(rhrd_t *d) {
  if (d->year > RHR_YEAR_MAX || d->year < RHR_YEAR_MIN) {
    return 1;
  }
  else if (d->year == RHR_YEAR_MAX) {
    if (d->month > RHR_MONTH_MAX) {
      return 1;
    } else if (d->month == RHR_MONTH_MAX) {
      if (d->day > RHR_DAY_MAX) {
        return 1;
      }
    }
  } else if (d->year == RHR_YEAR_MIN) {
    if (d->month < RHR_MONTH_MIN) {
      return 1;
    } else if (d->month == RHR_MONTH_MIN) {
      if (d->day < RHR_DAY_MIN) {
        return 1;
      }
    }
  }
  return 0;
}

long rhrd__safe_add_long(long a, long b) {
  if ((a > 0 && b > 0 && (a > LONG_MAX - b)) ||
      (a < 0 && b < 0 && (a < LONG_MIN - b))) {
    rb_raise(rb_eRangeError, "addition would overflow");
  }
  return a + b;
}

void rhrd__civil_to_jd(rhrd_t *d) {
  long a;
  if (d->month <= 2) {
    a = (long)floor((d->year - 1)/100.0);
    d->jd = (long)floor(365.25 * (d->year + 4715)) + \
          (long)floor(30.6001 * (d->month + 13)) + \
          d->day - 1524 + (2 - a + (long)floor(a / 4.0));
  } else {
    a = (long)floor(d->year/100.0);
    d->jd = (long)floor(365.25 * (d->year + 4716)) + \
          (long)floor(30.6001 * (d->month + 1)) + \
          d->day - 1524 + (2 - a + (long)floor(a / 4.0));
  }
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
#ifdef DEBUG
  printf("x: %li, a: %li, b: %li, c: %li, d: %li, e: %li, day: %hhi, month: %hhi; year: %li\n", x, a, b, c, d, e, date->day, date->month, date->year);
#endif
}

unsigned char rhrd__num2month(VALUE obj) {
  int i = NUM2LONG(obj);
  if (i < 1 || i > 12) {
    rb_raise(rb_eArgError, "invalid Date: month %i", i);
  }
  return (unsigned char)i;
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

unsigned char rhrd__num2day(long year, unsigned char month, VALUE obj) {
  long i = NUM2LONG(obj);
  if (i < 1 || i <= 28) {
    return (unsigned char)i;
  } else if (i > 31) {
    rb_raise(rb_eArgError, "invalid Date: day %hhi", i);
  } else if (month == 2) {
    if (rhrd__leap_year(year)) {
      if (i <= 29) {
        return (unsigned char)i;
      } else {
        rb_raise(rb_eArgError, "invalid Date: year %li, month %hhi, day %hhi", year, month, i);
      }
    } else {
      if (i <= 28) {
        return (unsigned char)i;
      } else {
        rb_raise(rb_eArgError, "invalid Date: year %li, month %hhi, day %hhi", year, month, i);
      }
    }
  } else {
    if (i <= rhrd_days_in_month[month]) {
      return (unsigned char)i;
    } else {
      rb_raise(rb_eArgError, "invalid Date: month %hhi, day %hhi", month, i);
    }
  }
}

unsigned char rhrd__days_in_month(long year, unsigned char month) {
  if (month == 2) {
    return rhrd__leap_year(year) ? 29 : 28;
  } else {
    return rhrd_days_in_month[month];
  }
}

static VALUE rhrd__add_days(VALUE self, long n) {
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

static VALUE rhrd__add_months(VALUE self, long n) {
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

  n.year = cwyear;
  n.month = 1;
  n.day = 4;
  rhrd__civil_to_jd(&n);
  return n.jd - (n.jd % 7) + 7 * (cweek - 1) + (cwday - 1);
}

/* Abuses the year, month, and day fields to store
 * cwyear, cweek, and cwday */
void rhrd__fill_commercial(rhrd_t *d) {
  long a;
  rhrd_t n;

  n.jd = d->jd - 3;
  rhrd__jd_to_civil(&n);
  a = n.year;
  d->year = d->jd >= rhrd__commercial_to_jd(a + 1, 1, 1) ? a + 1 : a;
  d->month = 1 + floor((d->jd - rhrd__commercial_to_jd(d->year, 1, 1)) / 7);
  d->day = (d->jd + 1) % 7;
  if (d->day == 0) {
    d->day = 7;
  }
}

/* Ruby Class Methods */

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

static VALUE rhrd_s_ajd_to_amjd(VALUE klass, VALUE ajd) {
  return INT2NUM(rhrd__safe_add_long(-RHR_JD_MJD, NUM2LONG(ajd)));
}

static VALUE rhrd_s_ajd_to_jd(int argc, VALUE *argv, VALUE klass) {
  VALUE a;

  switch(argc) {
    case 1:
    case 2:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguements: %i for 2", argc);
      break;
  }

  a = rb_ary_new();
  rb_ary_push(a, argv[0]);
  rb_ary_push(a, rb_float_new(0.5));
  return a;
}

static VALUE rhrd_s_amjd_to_ajd(VALUE klass, VALUE amjd) {
  return INT2NUM(rhrd__safe_add_long(RHR_JD_MJD - 1, NUM2LONG(amjd)));
}

static VALUE rhrd_s_civil (int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->year = RHR_DEFAULT_YEAR;
      d->month = RHR_DEFAULT_MONTH;
      d->day = RHR_DEFAULT_DAY;
      break;
    case 1:
      d->year = NUM2LONG(argv[0]);
      d->month = RHR_DEFAULT_MONTH;
      d->day = RHR_DEFAULT_DAY;
      break;
    case 2:
      d->year = NUM2LONG(argv[0]);
      d->month = rhrd__num2month(argv[1]);
      d->day = RHR_DEFAULT_DAY;
      break;
    case 3:
    case 4:
      d->year = NUM2LONG(argv[0]);
      d->month = rhrd__num2month(argv[1]);
      d->day = rhrd__num2day(d->year, d->month, argv[2]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguements: %i for 4", argc);
      break;
  }
  RHR_CHECK_CIVIL(d)
  d->flags = RHR_HAVE_CIVIL;
  return rd;
}

static VALUE rhrd_s_civil_to_jd(int argc, VALUE *argv, VALUE klass) {
  rhrd_t d;

  switch(argc) {
    case 3:
    case 4:
      d.year = NUM2LONG(argv[0]);
      d.month = (unsigned char)NUM2LONG(argv[1]);
      d.day = (unsigned char)NUM2LONG(argv[2]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguements: %i for 4", argc);
      break;
  }
  d.flags = RHR_HAVE_CIVIL;
  RHR_FILL_JD(&d)

  return INT2NUM(d.jd);
}

static VALUE rhrd_s_jd (int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->jd = RHR_DEFAULT_JD;
      break;
    case 1:
    case 2:
      d->jd = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguements: %i for 2", argc);
      break;
  }
  RHR_CHECK_JD(d)
  d->flags = RHR_HAVE_JD;
  return rd;
}

static VALUE rhrd_s_today (int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d;
  VALUE rd = Data_Make_Struct(klass, rhrd_t, NULL, free, d);
  VALUE t;

  switch(argc) {
    case 0:
    case 1:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguements: %i for 1", argc);
      break;
  }
  t = rb_funcall(rb_cTime, rhrd_id_now, 0);
  d->year = NUM2LONG(rb_funcall(t, rhrd_id_year, 0));
  d->month = rhrd__num2month(rb_funcall(t, rhrd_id_mon, 0));
  d->day = rhrd__num2day(d->year, d->month, rb_funcall(t, rhrd_id_mday, 0));
  RHR_CHECK_CIVIL(d)
  d->flags = RHR_HAVE_CIVIL;
  return rd;
}

/* Ruby Instance Methods */

static VALUE rhrd__dump(VALUE self, VALUE limit) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return rb_marshal_dump(INT2NUM(d->jd), INT2NUM(NUM2LONG(limit) - 1));
}

static VALUE rhrd_asctime(VALUE self) {
  VALUE s;
  rhrd_t *d;
  char *str;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  RHR_FILL_JD(d)
  if (asprintf(&str, "%s %s %2hhi 00:00:00 %04li", 
        rhrd_abbr_day_names[(d->jd + 1) % 7],
        rhrd_abbr_month_names[d->month],
        d->day,
        d->year) == -1) {
    rb_raise(rb_eNoMemError, "in Date#asctime");
  }
  s = rb_str_new2(str);
  free(str);
  return s;
}

static VALUE rhrd_cwday(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return INT2NUM(n.day);
}

static VALUE rhrd_cweek(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return INT2NUM(n.month);
}

static VALUE rhrd_cwyear(VALUE self) {
  rhrd_t *d;
  rhrd_t n;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return INT2NUM(n.year);
}

static VALUE rhrd_day(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return INT2NUM(d->day);
}

static VALUE rhrd_day_fraction(VALUE self) {
  return INT2NUM(0);
}

static VALUE rhrd_downto(VALUE self, VALUE other) {
  VALUE argv[2];
  argv[0] = other;
  argv[1] = INT2FIX(-1);
  return rhrd_step(2, argv, self);
}

static VALUE rhrd_eql_q(VALUE self, VALUE other) {
  rhrd_t *d, *o;
  Data_Get_Struct(self, rhrd_t, d);

  if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
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
  return rb_funcall(INT2NUM(d->jd), rhrd_id_hash, 0);
}

static VALUE rhrd_inspect(VALUE self) {
  VALUE s;
  rhrd_t *d;
  char *str;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  if (asprintf(&str, "#<Date %04li-%02hhi-%02hhi>", d->year, d->month, d->day) == -1) {
    rb_raise(rb_eNoMemError, "in Date#inspect");
  }
  s = rb_str_new2(str);
  free(str);
  return s;
}

static VALUE rhrd_jd(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return INT2NUM(d->jd);
}

static VALUE rhrd_julian_q(VALUE self) {
  return Qfalse;
}

static VALUE rhrd_ld(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_JD(d)
  return INT2NUM(d->jd - RHR_JD_LD);
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
  return INT2NUM(d->jd - RHR_JD_MJD);
}

static VALUE rhrd_month(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return INT2NUM(d->month);
}

static VALUE rhrd_next(VALUE self) {
   return rhrd__add_days(self, 1);
}

static VALUE rhrd_new_start(VALUE self, VALUE other) {
  return self;
}

static VALUE rhrd_start(VALUE self) {
  return INT2NUM(RHR_JD_MAX);
}

static VALUE rhrd_step(int argc, VALUE *argv, VALUE self) {
  rhrd_t *d, *n, *newd;
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
      rb_raise(rb_eArgError, "wrong number of arguements: %i for 2", argc);
      break;
  }

  rlimit = argv[0];
  if (RTEST(rb_obj_is_kind_of(rlimit, rb_cNumeric))) {
    limit = NUM2LONG(rlimit);
  } else if (RTEST((rb_obj_is_kind_of(rlimit, rhrd_class)))) {
    Data_Get_Struct(rlimit, rhrd_t, n);
    RHR_FILL_JD(n)
    limit = n->jd;
  } else {
    rb_raise(rb_eTypeError, "expected numeric or date");
  }

  current = d->jd;
  if (limit > current) {
    while(limit >= current) {
      new = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, newd);
      newd->jd = current;
      RHR_CHECK_JD(newd)
      newd->flags = RHR_HAVE_JD;
      current += step;
      rb_yield(new);
    }
  } else if (limit < current) {
    while(limit <= current) {
      new = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, newd);
      newd->jd = current;
      RHR_CHECK_JD(newd)
      newd->flags = RHR_HAVE_JD;
      current += step;
      rb_yield(new);
    }
  } else {
    rb_yield(self);
  }

  return self;
}

static VALUE rhrd_to_s(VALUE self) {
  VALUE s;
  rhrd_t *d;
  char *str;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  if (asprintf(&str, "%04li-%02hhi-%02hhi", d->year, d->month, d->day) == -1) {
    rb_raise(rb_eNoMemError, "in Date#to_s");
  }
  s = rb_str_new2(str);
  free(str);
  return s;
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
  return INT2NUM((d->jd + 1) % 7);
}

static VALUE rhrd_yday(VALUE self) {
  rhrd_t *d;
  long yday;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  yday = rhrd_cumulative_days_in_month[d->month] + d->day;
  if(d->month > 2 && rhrd__leap_year(d->year)) {
    yday += 1;
  }
  return INT2NUM(yday);
}

static VALUE rhrd_year(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return INT2NUM(d->year);
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
  Data_Get_Struct(self, rhrd_t, d);

  if (RTEST(rb_obj_is_kind_of(other, rb_cNumeric))) {
    return rhrd__add_days(self, -NUM2LONG(other));
  }
  if (RTEST((rb_obj_is_kind_of(other, rhrd_class)))) {
    Data_Get_Struct(other, rhrd_t, newd);
    RHR_FILL_JD(d)
    RHR_FILL_JD(newd)
    return INT2NUM(rhrd__safe_add_long(d->jd, -newd->jd));
  }
  rb_raise(rb_eTypeError, "expected numeric or date");
}

static VALUE rhrd_op_relationship(VALUE self, VALUE other) {
  rhrd_t *d, *o;
  long diff = 1;
  Data_Get_Struct(self, rhrd_t, d);

  if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
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
  long diff;
  Data_Get_Struct(self, rhrd_t, d);

  if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(other, rhrd_t, o);
    return INT2NUM(rhrd__spaceship(d, o));
  } else if (RTEST((rb_obj_is_kind_of(other, rb_cNumeric)))) {
    diff = NUM2LONG(other);
    RHR_FILL_JD(d)
    RHR_SPACE_SHIP(diff, d->jd, diff)
    return INT2NUM(diff);
  }
  return Qnil;
}

/* Ruby Library Initialization */

void Init_home_run_date(void) {
  rhrd_id_hash = rb_intern("hash");
  rhrd_id_mday = rb_intern("mday");
  rhrd_id_mon = rb_intern("mon");
  rhrd_id_now = rb_intern("now");
  rhrd_id_year = rb_intern("year");

  rhrd_class = rb_define_class("Date", rb_cObject);
  rhrd_s_class = rb_singleton_class(rhrd_class);

  rb_define_const(rhrd_class, "ITALY", INT2NUM(RHR_JD_ITALY));
  rb_define_const(rhrd_class, "ENGLAND", INT2NUM(RHR_JD_ENGLAND));
  rb_define_const(rhrd_class, "JULIAN", INT2NUM(RHR_JD_MIN - 1));
  rb_define_const(rhrd_class, "GREGORIAN", INT2NUM(RHR_JD_MAX + 1));

  rb_define_method(rhrd_s_class, "_load", rhrd_s__load, 1);
  rb_define_method(rhrd_s_class, "ajd_to_amjd", rhrd_s_ajd_to_amjd, 1);
  rb_define_method(rhrd_s_class, "ajd_to_jd", rhrd_s_ajd_to_jd, -1);
  rb_define_method(rhrd_s_class, "amjd_to_ajd", rhrd_s_amjd_to_ajd, 1);
  rb_define_method(rhrd_s_class, "civil", rhrd_s_civil, -1);
  rb_define_method(rhrd_s_class, "civil_to_jd", rhrd_s_civil_to_jd, -1);
  rb_define_method(rhrd_s_class, "jd", rhrd_s_jd, -1);
  rb_define_method(rhrd_s_class, "today", rhrd_s_today, -1);

  rb_define_alias(rhrd_s_class, "new", "civil");

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
  rb_define_method(rhrd_class, "new_start", rhrd_new_start, 1);
  rb_define_method(rhrd_class, "start", rhrd_start, 0);
  rb_define_method(rhrd_class, "step", rhrd_step, -1);
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
  rb_define_alias(rhrd_class, "newsg", "new_start");
  rb_define_alias(rhrd_class, "ns?", "gregorian?");
  rb_define_alias(rhrd_class, "os?", "julian?");
  rb_define_alias(rhrd_class, "sg", "start");
  rb_define_alias(rhrd_class, "succ", "next");

  rb_define_method(rhrd_class, ">>", rhrd_op_right_shift, 1);
  rb_define_method(rhrd_class, "<<", rhrd_op_left_shift, 1);
  rb_define_method(rhrd_class, "+", rhrd_op_plus, 1);
  rb_define_method(rhrd_class, "-", rhrd_op_minus, 1);
  rb_define_method(rhrd_class, "===", rhrd_op_relationship, 1);
  rb_define_method(rhrd_class, "<=>", rhrd_op_spaceship, 1);

  rb_funcall(rhrd_class, rb_intern("include"), 1, rb_mComparable);
}

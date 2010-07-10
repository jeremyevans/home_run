
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ruby.h>

#define RHR_DEFAULT_JD 0
#define RHR_DEFAULT_YEAR -4712
#define RHR_DEFAULT_MONTH 1
#define RHR_DEFAULT_DAY 1

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
#define RHR_JD_MIN -2147441038
#define RHR_YEAR_MAX 5874773
#define RHR_MONTH_MAX 8
#define RHR_DAY_MAX 15
#define RHR_YEAR_MIN -5884206
#define RHR_MONTH_MIN 1
#define RHR_DAY_MIN 12
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
VALUE rhrd_class;
VALUE rhrd_s_class;

ID rhrd_id_now;
ID rhrd_id_year;
ID rhrd_id_mon;
ID rhrd_id_mday;

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

/* Ruby Class Methods */

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

static VALUE rhrd_day(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return INT2NUM(d->day);
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

static VALUE rhrd_month(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return INT2NUM(d->month);
}

static VALUE rhrd_year(VALUE self) {
  rhrd_t *d;
  Data_Get_Struct(self, rhrd_t, d);
  RHR_FILL_CIVIL(d)
  return INT2NUM(d->year);
}

/* Ruby Instance Operator Methods */

static VALUE rhrd_op_plus(VALUE self, VALUE other) {
  rhrd_t *d;
  rhrd_t *newd;
  VALUE new;
  Data_Get_Struct(self, rhrd_t, d);
  long n, x;

  if (!RTEST((rb_obj_is_kind_of(other, rb_cNumeric)))) {
    rb_raise(rb_eTypeError, "expected numeric");
  }

  n = NUM2LONG(other);
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

static VALUE rhrd_op_minus(VALUE self, VALUE other) {
  rhrd_t *d;
  rhrd_t *newd;
  VALUE new;
  Data_Get_Struct(self, rhrd_t, d);
  long n, x;

  if (RTEST(rb_obj_is_kind_of(other, rb_cNumeric))) {
    n = -NUM2LONG(other);
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
  if (RTEST((rb_obj_is_kind_of(other, rhrd_class)))) {
    Data_Get_Struct(other, rhrd_t, newd);
    RHR_FILL_JD(d)
    RHR_FILL_JD(newd)
    n = rhrd__safe_add_long(d->jd, -newd->jd);
    return INT2NUM(n);
  }
  rb_raise(rb_eTypeError, "expected numeric or date");
}

static VALUE rhrd_op_spaceship(VALUE self, VALUE other) {
  rhrd_t *d, *o;
  Data_Get_Struct(self, rhrd_t, d);
  long diff;

  if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(other, rhrd_t, o);
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
    return INT2NUM(diff);
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
  rhrd_id_now = rb_intern("now");
  rhrd_id_year = rb_intern("year");
  rhrd_id_mon = rb_intern("mon");
  rhrd_id_mday = rb_intern("mday");
  rhrd_class = rb_define_class("Date", rb_cObject);
  rhrd_s_class = rb_singleton_class(rhrd_class);

  rb_define_method(rhrd_s_class, "civil", rhrd_s_civil, -1);
  rb_define_method(rhrd_s_class, "jd", rhrd_s_jd, -1);
  rb_define_method(rhrd_s_class, "today", rhrd_s_today, -1);

  rb_define_alias(rhrd_s_class, "new", "civil");

  rb_define_method(rhrd_class, "day", rhrd_day, 0);
  rb_define_method(rhrd_class, "inspect", rhrd_inspect, 0);
  rb_define_method(rhrd_class, "jd", rhrd_jd, 0);
  rb_define_method(rhrd_class, "month", rhrd_month, 0);
  rb_define_method(rhrd_class, "year", rhrd_year, 0);

  rb_define_alias(rhrd_class, "mday", "day");
  rb_define_alias(rhrd_class, "mon", "month");

  rb_define_method(rhrd_class, "+", rhrd_op_plus, 1);
  rb_define_method(rhrd_class, "-", rhrd_op_minus, 1);
  rb_define_method(rhrd_class, "<=>", rhrd_op_spaceship, 1);
  rb_funcall(rhrd_class, rb_intern("include"), 1, rb_mComparable);
}

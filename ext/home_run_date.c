
#include <stdio.h>
#include <stdlib.h>
#include <ruby.h>

#define RHR_DEFAULT_JD 0
#define RHR_DEFAULT_YEAR -4712
#define RHR_DEFAULT_MONTH 1
#define RHR_DEFAULT_DAY 1

#define RHR_HAVE_JD 0x1
#define RHR_HAVE_CIVIL 0x2

#define RHR_FILL_JD(d) if (((d)->flags & RHR_HAVE_JD) == 0) { rhrd__civil_to_jd(d); }
#define RHR_FILL_CIVIL(d) if (((d)->flags & RHR_HAVE_CIVIL) == 0) { rhrd__jd_to_civil(d); }

typedef struct rhrd_s {
  long jd;
  long year;
  unsigned char month;
  unsigned char day;
  unsigned char flags;
} rhrd_t;

unsigned char rhrd_days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
VALUE rhrd_class;

ID rhrd_id_now;
ID rhrd_id_year;
ID rhrd_id_mon;
ID rhrd_id_mday;

/* C Helper Methods */

void rhrd__civil_to_jd(rhrd_t *d) {
  long a;
  if (d->month <= 2) {
    a = (d->year - 1)/100;
    d->jd = (long)(365.25 * (d->year + 4715)) + \
          (long)(30.6001 * (d->month + 13)) + \
          d->day - 1524 + (2 - a + (a / 4));
  } else {
    a = d->year/100;
    d->jd = (long)(365.25 * (d->year + 4716)) + \
          (long)(30.6001 * (d->month + 1)) + \
          d->day - 1524 + (2 - a + (a / 4));
  }
  d->flags |= RHR_HAVE_JD;
}

void rhrd__jd_to_civil(rhrd_t *date) {
  long x, a, b, c, d, e;
  x = (long)((date->jd - 1867216.25) / 36524.25);
  a = date->jd + 1 + x - (x / 4);
  b = a + 1524;
  c = (long)((b - 122.1) / 365.25);
  d = (long)(365.25 * c);
  e = (long)((b - d) / 30.6001);
  date->day = b - d - (long)(30.6001 * e);
  if (e <= 13) {
    date->month = e - 1;
    date->year = c - 4716;
  } else {
    date->month = e - 13;
    date->year = c - 4715;
  }
  date->flags |= RHR_HAVE_CIVIL;
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
    rb_raise(rb_eArgError, "invalid Date: day %i", i);
  } else if (month == 2) {
    if (rhrd__leap_year(year)) {
      if (i <= 29) {
        return (unsigned char)i;
      } else {
        rb_raise(rb_eArgError, "invalid Date: year %i, month %i, day %i", year, month, i);
      }
    } else {
      if (i <= 28) {
        return (unsigned char)i;
      } else {
        rb_raise(rb_eArgError, "invalid Date: year %i, month %i, day %i", year, month, i);
      }
    }
  } else {
    if (i <= rhrd_days_in_month[month]) {
      return (unsigned char)i;
    } else {
      rb_raise(rb_eArgError, "invalid Date: month %i, day %i", month, i);
    }
  }
}

/* Ruby Class Methods */

static VALUE rhrd_s_civil (int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d = NULL;
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
  d->flags = RHR_HAVE_CIVIL;
  return rd;
}

static VALUE rhrd_s_jd (int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d = NULL;
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
  d->flags = RHR_HAVE_JD;
  return rd;
}

static VALUE rhrd_s_today (int argc, VALUE *argv, VALUE klass) {
  rhrd_t *d = NULL;
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

/* Ruby Library Initialization */

void Init_home_run_date(void) {
  rhrd_id_now = rb_intern("now");
  rhrd_id_year = rb_intern("year");
  rhrd_id_mon = rb_intern("mon");
  rhrd_id_mday = rb_intern("mday");
  rhrd_class = rb_define_class("Date", rb_cObject);

  rb_define_singleton_method(rhrd_class, "civil", rhrd_s_civil, -1);
  rb_define_singleton_method(rhrd_class, "jd", rhrd_s_jd, -1);
  rb_define_singleton_method(rhrd_class, "today", rhrd_s_today, -1);

  rb_define_method(rhrd_class, "day", rhrd_day, 0);
  rb_define_method(rhrd_class, "inspect", rhrd_inspect, 0);
  rb_define_method(rhrd_class, "jd", rhrd_jd, 0);
  rb_define_method(rhrd_class, "month", rhrd_month, 0);
  rb_define_method(rhrd_class, "year", rhrd_year, 0);

  rb_define_alias(rhrd_class, "mday", "day");
  rb_define_alias(rhrd_class, "mon", "month");
}

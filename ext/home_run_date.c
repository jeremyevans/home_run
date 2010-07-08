
#include <stdio.h>
#include <stdlib.h>
#include <ruby.h>

#define RHR_DEFAULT_YEAR -4712
#define RHR_DEFAULT_MONTH 1
#define RHR_DEFAULT_DAY 1

#define RHR_HAVE_JD 0x1
#define RHR_HAVE_CIVIL 0x2

typedef struct rhrd_s {
  signed long jd;
  signed long year;
  unsigned char month;
  unsigned char day;
  unsigned char flags;
} rhrd_t;

unsigned char rhrd_days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
VALUE rhrd_class;

unsigned char rhrd_num2month(VALUE obj) {
  int i = NUM2LONG(obj);
  if (i < 1 || i > 12) {
    rb_raise(rb_eArgError, "invalid Date: month %i", i);
  }
  return (unsigned char)i;
}

int rhrd_leap_year(signed long year) {
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

unsigned char rhrd_num2day(signed long year, unsigned char month, VALUE obj) {
  long i = NUM2LONG(obj);
  if (i < 1 || i <= 28) {
    return (unsigned char)i;
  } else if (i > 31) {
    rb_raise(rb_eArgError, "invalid Date: day %i", i);
  } else if (month == 2) {
    if (rhrd_leap_year(year)) {
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

static VALUE rhrd_allocate(VALUE klass) {
  rhrd_t *d = NULL;
  return Data_Make_Struct(klass, rhrd_t, NULL, free, d);
}

static VALUE rhrd_s_civil (int argc, VALUE *argv, VALUE klass) {
  VALUE rd = rhrd_allocate(klass);
  rhrd_t *d;
  Data_Get_Struct(rd, rhrd_t, d);

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
      d->month = rhrd_num2month(argv[1]);
      d->day = RHR_DEFAULT_DAY;
      break;
    case 3:
    case 4:
      d->year = NUM2LONG(argv[0]);
      d->month = rhrd_num2month(argv[1]);
      d->day = rhrd_num2day(d->year, d->month, argv[2]);
      break;
  }
  d->flags = RHR_HAVE_CIVIL;
  return rd;
}

static VALUE rhrd_inspect(VALUE self) {
  VALUE s;
  rhrd_t *d;
  char *str;
  Data_Get_Struct(self, rhrd_t, d);
  asprintf(&str, "#<Date %04li-%02hhi-%02hhi>", d->year, d->month, d->day);
  s = rb_str_new2(str);
  free(str);
  return s;
}

void Init_home_run_date(void) {
  rhrd_class = rb_define_class("Date", rb_cObject);
  rb_define_singleton_method(rhrd_class, "civil", rhrd_s_civil, -1);
  rb_define_method(rhrd_class, "inspect", rhrd_inspect, 0);
}

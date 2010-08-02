
#define RHR_HAVE_FRACTION 4
#define RHR_HAVE_HMS 8

typedef struct rhrdt_s {
  double fraction; /* Fraction of the day, range: [0.0, 1.0) */
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

#define RHRDT_FILL_JD(d) if (!((d)->flags & RHR_HAVE_JD)) { rhrdt__civil_to_jd(d); }
#define RHRDT_FILL_CIVIL(d) if (!((d)->flags & RHR_HAVE_CIVIL)) { rhrdt__jd_to_civil(d); }
#define RHRDT_FILL_HMS(d) if (!((d)->flags & RHR_HAVE_HMS)) { rhrdt__fraction_to_hms(d); }
#define RHRDT_FILL_FRACTION(d) if (!((d)->flags & RHR_HAVE_FRACTION)) { rhrdt__hms_to_fraction(d); }

VALUE rhrdt_class;
VALUE rhrdt_s_class;

/* Helper methods */

int rhrdt__valid_civil(rhrdt_t *dt, long year, long month, long day) {
  if(!rhrd__check_valid_civil(year, month, day)) {
    return 0;
  }

  dt->year = year;
  dt->month = (unsigned char)month;
  dt->day = (unsigned char)day;
  dt->flags |= RHR_HAVE_CIVIL;
  return 1;
}

int rhrdt__valid_offset(rhrdt_t *dt, double offset) {
  if (offset < -0.6 || offset > 0.6) {
    return 0;
  }

  dt->offset = round(offset * 1440);
  return 1;
}

int rhrdt__valid_time(rhrdt_t *dt, long h, long m, long s, double offset) {
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
    return 0;
  }
  if(!rhrdt__valid_offset(dt, offset)) {
    return 0;
  }

  dt->hour = h;
  dt->minute = m;
  dt->second = s;
  dt->flags |= RHR_HAVE_HMS;
  return 1;
}

void rhrdt__civil_to_jd(rhrdt_t *d) {
  d->jd = rhrd__ymd_to_jd(d->year, d->month, d->day);
  d->flags |= RHR_HAVE_JD;
}

void rhrdt__jd_to_civil(rhrdt_t *date) {
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

void rhrdt__fraction_to_hms(rhrdt_t *d) {
  double f;
  f = d->fraction * 24;
  d->hour = floor(f);
  f = (f - d->hour) * 60;
  d->minute = floor(f);
  f = (f - d->minute) * 60;
  d->second = floor(f);
  d->flags |= RHR_HAVE_HMS;
}

void rhrdt__hms_to_fraction(rhrdt_t *d) {
  d->fraction = d->hour/24.0 + d->minute/1440.0 + d->second/86400.0;
  d->flags |= RHR_HAVE_FRACTION;
}

int rhrdt__valid_commercial(rhrdt_t *d, long cwyear, long cweek, long cwday) {
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

int rhrdt__valid_ordinal(rhrdt_t *d, long year, long yday) {
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

void rhrdt__now(rhrdt_t * dt) {
  long t;
  long offset;
  offset = NUM2LONG(rb_funcall(rb_funcall(rb_cTime, rhrd_id_now, 0), rhrd_id_utc_offset, 0));
  t = time(NULL) + offset;
  dt->jd = rhrd__unix_to_jd(t);
  dt->fraction = rhrd__mod(t, 86400)/86400.0;
  dt->offset = offset/60;
  dt->flags |= RHR_HAVE_JD | RHR_HAVE_FRACTION;
  RHR_CHECK_JD(dt);
}

/* Class methods */

static VALUE rhrdt_s_civil(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *dt;
  long year = RHR_DEFAULT_YEAR;
  long month = 1;
  long day = 1;
  long hour = 0;
  long minute = 0;
  long second = 0;
  double offset = 0.0;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 0:
      dt->flags = RHR_HAVE_JD | RHR_HAVE_FRACTION | RHR_HAVE_HMS;
      return rdt;
    case 8:
    case 7:
      offset = NUM2DBL(argv[6]);
    case 6:
      second = NUM2LONG(argv[5]);
    case 5:
      minute = NUM2LONG(argv[4]);
    case 4:
      hour = NUM2LONG(argv[3]);
    case 3:
      day = NUM2LONG(argv[2]);
    case 2:
      month = NUM2LONG(argv[1]);
    case 1:
      year = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 8", argc);
      break;
  }

  if (!rhrdt__valid_civil(dt, year, month, day)) {
    RHR_CHECK_CIVIL(dt)
    rb_raise(rb_eArgError, "invalid date (year: %li, month: %li, day: %li)", year, month, day);
  }
  if (!rhrdt__valid_time(dt, hour, minute, second, offset)) {
    rb_raise(rb_eArgError, "invalid time (hour: %li, minute: %li, second: %li, offset: %f)", hour, minute, second, offset);
  }

  return rdt;
}

static VALUE rhrdt_s_commercial(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *dt;
  long cwyear = RHR_DEFAULT_CWYEAR;
  long cweek = RHR_DEFAULT_CWEEK;
  long cwday = RHR_DEFAULT_CWDAY;
  long hour = 0;
  long minute = 0;
  long second = 0;
  double offset = 0.0;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 8:
    case 7:
      offset = NUM2DBL(argv[6]);
    case 6:
      second = NUM2LONG(argv[5]);
    case 5:
      minute = NUM2LONG(argv[4]);
    case 4:
      hour = NUM2LONG(argv[3]);
    case 3:
      cwday = NUM2LONG(argv[2]);
    case 2:
      cweek = NUM2LONG(argv[1]);
    case 1:
      cwyear = NUM2LONG(argv[0]);
    case 0:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 8", argc);
      break;
  }
  if(!rhrdt__valid_commercial(dt, cwyear, cweek, cwday)) {
    RHR_CHECK_JD(dt)
    rb_raise(rb_eArgError, "invalid date (cwyear: %li, cweek: %li, cwday: %li)", cwyear, cweek, cwday);
  }
  if (!rhrdt__valid_time(dt, hour, minute, second, offset)) {
    rb_raise(rb_eArgError, "invalid time (hour: %li, minute: %li, second: %li, offset: %f)", hour, minute, second, offset);
  }

  return rdt;
}

static VALUE rhrdt_s_jd(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *dt;
  long hour = 0;
  long minute = 0;
  long second = 0;
  double offset = 0.0;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 0:
      dt->flags = RHR_HAVE_JD | RHR_HAVE_FRACTION | RHR_HAVE_HMS;
      return rdt;
    case 6:
    case 5:
      offset = NUM2DBL(argv[4]);
    case 4:
      second = NUM2LONG(argv[3]);
    case 3:
      minute = NUM2LONG(argv[2]);
    case 2:
      hour = NUM2LONG(argv[1]);
    case 1:
      dt->jd = NUM2LONG(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 6", argc);
      break;
  }

  RHR_CHECK_JD(dt)
  dt->flags = RHR_HAVE_JD;
  if (!rhrdt__valid_time(dt, hour, minute, second, offset)) {
    rb_raise(rb_eArgError, "invalid time (hour: %li, minute: %li, second: %li, offset: %f)", hour, minute, second, offset);
  }

  return rdt;
}

static VALUE rhrdt_s_new_b(int argc, VALUE *argv, VALUE klass) {
  double offset = 0;
  rhrdt_t *dt;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 0:
      dt->flags = RHR_HAVE_JD | RHR_HAVE_FRACTION | RHR_HAVE_HMS;
      return rdt; 
    case 2:
    case 3:
      offset = NUM2DBL(argv[1]);
      if (!rhrdt__valid_offset(dt, offset)) {
        rb_raise(rb_eArgError, "invalid offset (%f)", offset);
      }
    case 1:
      offset += NUM2DBL(argv[0]) + 0.5;
      dt->jd = offset;
      dt->fraction = offset - dt->jd;
      dt->flags = RHR_HAVE_JD | RHR_HAVE_FRACTION;
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 3", argc);
      break;
  }

  RHR_CHECK_JD(dt)
  return rdt;
}

static VALUE rhrdt_s_now(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *dt;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 0:
    case 1:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  rhrdt__now(dt);
  return rdt;
}

static VALUE rhrdt_s_ordinal(int argc, VALUE *argv, VALUE klass) {
  long year = RHR_DEFAULT_ORDINAL_YEAR;
  long day = RHR_DEFAULT_ORDINAL_DAY;
  long hour = 0;
  long minute = 0;
  long second = 0;
  double offset = 0.0;
  rhrdt_t *dt;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 7:
    case 6:
      offset = NUM2DBL(argv[5]);
    case 5:
      second = NUM2LONG(argv[4]);
    case 4:
      minute = NUM2LONG(argv[3]);
    case 3:
      hour = NUM2LONG(argv[2]);
    case 2:
      day = NUM2LONG(argv[1]);
    case 1:
      year = NUM2LONG(argv[0]);
      break;
    case 0:
      dt->flags = RHR_HAVE_JD | RHR_HAVE_FRACTION | RHR_HAVE_HMS;
      return rdt;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 7", argc);
      break;
  }

  if(!rhrdt__valid_ordinal(dt, year, day)) {
    RHR_CHECK_JD(dt)
    rb_raise(rb_eArgError, "invalid date (year: %li, yday: %li)", year, day);
  }
  if (!rhrdt__valid_time(dt, hour, minute, second, offset)) {
    rb_raise(rb_eArgError, "invalid time (hour: %li, minute: %li, second: %li, offset: %f)", hour, minute, second, offset);
  }
  return rdt;
}

/* Instance methods */

static VALUE rhrdt_inspect(VALUE self) {
  VALUE s;
  rhrdt_t *dt;
  int len;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  RHRDT_FILL_HMS(dt)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "#<DateTime %04li-%02hhi-%02hhiT%02hhi:%02hhi:%02hhi%+03i:%02li>",
        dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second, dt->offset/60, rhrd__mod(dt->offset, 60));
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#inspect (in snprintf)");
  }

  return rb_str_resize(s, len);
}

static VALUE rhrdt_to_s(VALUE self) {
  VALUE s;
  rhrdt_t *dt;
  int len;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  RHRDT_FILL_HMS(dt)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%04li-%02hhi-%02hhiT%02hhi:%02hhi:%02hhi%+03i:%02li",
        dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second, dt->offset/60, rhrd__mod(dt->offset, 60));
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#to_s (in snprintf)");
  }

  return rb_str_resize(s, len);
}


/* Library initialization */

void Init_datetime(void) {
  rhrdt_class = rb_define_class("DateTime", rhrd_class);
  rhrdt_s_class = rb_singleton_class(rhrdt_class);

  rb_define_method(rhrdt_s_class, "civil", rhrdt_s_civil, -1);
  rb_define_method(rhrdt_s_class, "commercial", rhrdt_s_commercial, -1);
  rb_define_method(rhrdt_s_class, "jd", rhrdt_s_jd, -1);
  rb_define_method(rhrdt_s_class, "new!", rhrdt_s_new_b, -1);
  rb_define_method(rhrdt_s_class, "now", rhrdt_s_now, -1);
  rb_define_method(rhrdt_s_class, "ordinal", rhrdt_s_ordinal, -1);

  rb_define_alias(rhrdt_s_class, "new", "civil");

  rb_define_method(rhrdt_class, "inspect", rhrdt_inspect, 0);
  rb_define_method(rhrdt_class, "to_s", rhrdt_to_s, 0);
  
#ifdef RUBY19
#else
  rb_define_alias(rhrdt_s_class, "new0", "new!");
  rb_define_alias(rhrdt_s_class, "new1", "jd");
  rb_define_alias(rhrdt_s_class, "new2", "ordinal");
  rb_define_alias(rhrdt_s_class, "new3", "civil");
  rb_define_alias(rhrdt_s_class, "neww", "commercial");
#endif
}


/* Helper methods */

int rhrdt__valid_civil(rhrdt_t *dt, long year, long month, long day) {
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

  if (h == 24 && m == 0 && s == 0) {
    RHRDT_FILL_JD(dt)
    dt->jd++;
    dt->flags &= ~RHR_HAVE_CIVIL;
    h = 0;
  } else if (h < 0 || m < 0 || s < 0 || h > 23 || m > 59 || s > 59) {
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
  double f;
  VALUE rt; 
  rt = rb_funcall(rb_cTime, rhrd_id_now, 0);
  offset = NUM2LONG(rb_funcall(rt, rhrd_id_utc_offset, 0));
  f = NUM2DBL(rb_funcall(rt, rhrd_id_to_f, 0)) + offset;
  t = floor(f);
  f -= t;
  dt->jd = rhrd__unix_to_jd(t);
  dt->fraction = (rhrd__mod(t, 86400) + f)/86400.0;
  dt->offset = offset/60;
  dt->flags |= RHR_HAVE_JD | RHR_HAVE_FRACTION;
  RHR_CHECK_JD(dt);
}

double rhrdt__to_double_offset(rhrdt_t *d) {
  RHRDT_FILL_JD(d)
  RHRDT_FILL_FRACTION(d)
  return d->jd + d->fraction + d->offset/1440;
}

double rhrdt__to_double(rhrdt_t *d) {
  RHRDT_FILL_JD(d)
  RHRDT_FILL_FRACTION(d)
  return d->jd + d->fraction;
}

VALUE rhrdt__from_double(double d, short offset) {
  rhrdt_t *dt;
  VALUE new;
  new = Data_Make_Struct(rhrdt_class, rhrdt_t, NULL, free, dt);
  
  dt->jd = floor(d);
  dt->fraction = d - dt->jd;
  dt->offset = offset;
  dt->flags = RHR_HAVE_JD | RHR_HAVE_FRACTION;
  return new;
}

long rhrdt__spaceship(rhrdt_t *dt, rhrdt_t *odt) {
  double d, o;
  d = rhrdt__to_double_offset(dt);
  o = rhrdt__to_double_offset(odt);
  /* Consider anything within a millisecond as equal */
  if (fabs(d - o) < 0.000000011574074) {
    return 0;
  } else if (d < o) {
    return -1;
  } 
  return 1;
}

VALUE rhrdt__add_days(VALUE self, double n) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  return rhrdt__from_double(rhrdt__to_double(dt) + n, dt->offset);
}

VALUE rhrdt__add_months(VALUE self, long n) {
  rhrdt_t *d;
  rhrdt_t *newd;
  VALUE new;
  long x;
  Data_Get_Struct(self, rhrdt_t, d);

  new = Data_Make_Struct(rhrdt_class, rhrdt_t, NULL, free, newd);
  RHRDT_FILL_CIVIL(d)
  memcpy(newd, d, sizeof(rhrdt_t));

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
  newd->flags &= ~RHR_HAVE_JD;
  return new;
}

/* Class methods */

static VALUE rhrdt_s__load(VALUE klass, VALUE string) {
  rhrdt_t * d;
  long x;
  VALUE ary, rd;
  rd = Data_Make_Struct(klass, rhrdt_t, NULL, free, d);

  ary = rb_marshal_load(string);
  if (!RTEST(rb_obj_is_kind_of(ary, rb_cArray)) || RARRAY_LEN(ary) != 3) {
    rb_raise(rb_eTypeError, "incompatible marshal file format");
  }

  d->jd = NUM2LONG(rb_ary_entry(ary, 0));
  RHR_CHECK_JD(d)

  d->fraction = NUM2DBL(rb_ary_entry(ary, 1));
  if (d->fraction < 0 || d->fraction >= 1.0) {
    rb_raise(rb_eArgError, "invalid day fraction: %f", d->fraction);
  }

  x = NUM2LONG(rb_ary_entry(ary, 2));
  if (x > 864 || x < -864) {
    rb_raise(rb_eArgError, "invalid offset: %ld minutes", x);
  }
  d->offset = x;
  
  d->flags = RHR_HAVE_JD | RHR_HAVE_FRACTION;
  return rd;
}

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

static VALUE rhrdt__dump(VALUE self, VALUE limit) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  RHRDT_FILL_FRACTION(d)
  return rb_marshal_dump(rb_ary_new3(3, INT2NUM(d->jd), rb_float_new(d->fraction), INT2NUM(d->offset)), INT2NUM(NUM2LONG(limit) - 1));
}

static VALUE rhrdt_asctime(VALUE self) {
  VALUE s;
  rhrdt_t *d;
  int len;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_CIVIL(d)
  RHRDT_FILL_JD(d)
  RHRDT_FILL_HMS(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s %s %2hhi %02hhi:%02hhi:%02hhi %04li", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        rhrd__abbr_month_names[d->month],
        d->day, d->hour, d->minute, d->second,
        d->year);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#asctime (in snprintf)");
  }

  return rb_str_resize(s, len);
}

static VALUE rhrdt_cwday(VALUE self) {
  rhrdt_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return INT2NUM(n.day);
}

static VALUE rhrdt_cweek(VALUE self) {
  rhrdt_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return INT2NUM(n.month);
}

static VALUE rhrdt_cwyear(VALUE self) {
  rhrdt_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return INT2NUM(n.year);
}

static VALUE rhrdt_day(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  return INT2NUM(dt->day);
}

static VALUE rhrdt_day_fraction(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_FRACTION(dt)
  return rb_float_new(dt->fraction);
}

static VALUE rhrdt_eql_q(VALUE self, VALUE other) {
  rhrdt_t *dt, *odt;
  rhrd_t *o;
  long diff;
  Data_Get_Struct(self, rhrdt_t, dt);

  if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
    Data_Get_Struct(other, rhrdt_t, odt);
    return rhrdt__spaceship(dt, odt) == 0 ? Qtrue : Qfalse;
  } else if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(other, rhrd_t, o);
    RHRDT_FILL_JD(dt)
    RHR_FILL_JD(o)
    RHR_SPACE_SHIP(diff, dt->jd, o->jd)
    if (diff == 0) {
      RHRDT_FILL_FRACTION(dt)
      RHR_SPACE_SHIP(diff, dt->fraction, 0)
    }
    return diff == 0 ? Qtrue : Qfalse;
  }
  return Qfalse;
}

static VALUE rhrdt_hash(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  return rb_funcall(rb_float_new(rhrdt__to_double_offset(d)), rhrd_id_hash, 0);
}

static VALUE rhrdt_hour(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_HMS(dt)
  return INT2NUM(dt->hour);
}

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

static VALUE rhrdt_jd(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return INT2NUM(d->jd);
}

static VALUE rhrdt_ld(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return INT2NUM(d->jd - RHR_JD_LD);
}

static VALUE rhrdt_leap_q(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_CIVIL(d)
  return rhrd__leap_year(d->year) ? Qtrue : Qfalse;
}

static VALUE rhrdt_min(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_HMS(dt)
  return INT2NUM(dt->minute);
}

static VALUE rhrdt_mjd(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return INT2NUM(d->jd - RHR_JD_MJD);
}

static VALUE rhrdt_month(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  return INT2NUM(dt->month);
}

static VALUE rhrdt_new_offset(int argc, VALUE *argv, VALUE self) {
  rhrdt_t *dt;
  double offset;

  switch(argc) {
    case 0:
      offset = 0;
      break;
    case 1:
      offset = NUM2DBL(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  if(offset < -0.6 || offset > 0.6) {
    rb_raise(rb_eArgError, "invalid offset (%f)", offset);
  } 
  Data_Get_Struct(self, rhrdt_t, dt);
  return rhrdt__from_double(rhrdt__to_double(dt) - dt->offset/1440.0 + offset, offset * 1440.0);
}

static VALUE rhrdt_offset(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  return rb_float_new(dt->offset/1440.0);
}

static VALUE rhrdt_sec(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_HMS(dt)
  return INT2NUM(dt->second);
}

static VALUE rhrdt_sec_fraction(VALUE self) {
  double f;
  long i;
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_FRACTION(dt)
 
  f = dt->fraction * 24;
  i = floor(f);
  f = (f - i) * 60;
  i = floor(f);
  f = (f - i) * 60;
  i = floor(f);
  f = (f - i)/86400;
  return rb_float_new(f);
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

static VALUE rhrdt_year(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  return INT2NUM(dt->year);
}

/* Operator methods */ 

static VALUE rhrdt_op_right_shift(VALUE self, VALUE other) {
  return rhrdt__add_months(self, NUM2LONG(other));
}

static VALUE rhrdt_op_left_shift(VALUE self, VALUE other) {
  return rhrdt__add_months(self, -NUM2LONG(other));
}

static VALUE rhrdt_op_plus(VALUE self, VALUE other) {
   return rhrdt__add_days(self, NUM2DBL(other));
}

static VALUE rhrdt_op_minus(VALUE self, VALUE other) {
  rhrdt_t *dt;
  rhrdt_t *newdt;
  rhrd_t *newd;
  Data_Get_Struct(self, rhrdt_t, dt);

  if (RTEST(rb_obj_is_kind_of(other, rb_cNumeric))) {
    return rhrdt__add_days(self, -NUM2DBL(other));
  }
  if (RTEST((rb_obj_is_kind_of(other, rhrdt_class)))) {
    Data_Get_Struct(other, rhrdt_t, newdt);
    return rb_float_new(rhrdt__to_double_offset(dt) - rhrdt__to_double_offset(newdt)); 
  }
  if (RTEST((rb_obj_is_kind_of(other, rhrd_class)))) {
    Data_Get_Struct(other, rhrd_t, newd);
    RHR_FILL_JD(newd)
    return rb_float_new(rhrdt__to_double(dt) - newd->jd); 
  }
  rb_raise(rb_eTypeError, "expected numeric or date");
}

static VALUE rhrdt_op_relationship(VALUE self, VALUE other) {
  rhrdt_t *dt, *odt;
  rhrd_t *o;
  long jd;

  if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
    Data_Get_Struct(other, rhrdt_t, odt);
    RHRDT_FILL_JD(odt)
    jd = odt->jd;
  } else if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(other, rhrd_t, o);
    RHR_FILL_JD(o)
    jd = o->jd;
  } else if (RTEST((rb_obj_is_kind_of(other, rb_cNumeric)))) {
    jd = NUM2LONG(other);
  } else {
    return Qfalse;
  }

  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_JD(dt)
  return dt->jd == jd ? Qtrue : Qfalse;
}

static VALUE rhrdt_op_spaceship(VALUE self, VALUE other) {
  rhrdt_t *dt, *odt;
  rhrd_t *od;
  double diff;
  int res;
  Data_Get_Struct(self, rhrdt_t, dt);

  if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
    Data_Get_Struct(other, rhrdt_t, odt);
    return INT2NUM(rhrdt__spaceship(dt, odt));
  }
  if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(other, rhrd_t, od);
    RHRDT_FILL_JD(dt)
    RHR_FILL_JD(od)
    RHR_SPACE_SHIP(res, dt->jd, od->jd)
    if (res == 0) {
      RHRDT_FILL_FRACTION(dt)
      RHR_SPACE_SHIP(res, dt->fraction, 0)
    }
    return INT2NUM(res);
  }
  if (RTEST((rb_obj_is_kind_of(other, rb_cNumeric)))) {
    diff = NUM2DBL(other);
    RHRDT_FILL_JD(dt)
    RHR_SPACE_SHIP(res, dt->jd, (long)diff)
    if (res == 0) {
      RHRDT_FILL_FRACTION(dt)
      RHR_SPACE_SHIP(res, dt->fraction, diff - floor(diff))
    }
    return INT2NUM(res);
  }
  return Qnil;
}


/* Library initialization */

void Init_datetime(void) {
  rhrdt_class = rb_define_class("DateTime", rhrd_class);
  rhrdt_s_class = rb_singleton_class(rhrdt_class);

  rb_undef(rhrdt_s_class, rb_intern("today"));
  rb_define_method(rhrdt_s_class, "_load", rhrdt_s__load, 1);
  rb_define_method(rhrdt_s_class, "civil", rhrdt_s_civil, -1);
  rb_define_method(rhrdt_s_class, "commercial", rhrdt_s_commercial, -1);
  rb_define_method(rhrdt_s_class, "jd", rhrdt_s_jd, -1);
  rb_define_method(rhrdt_s_class, "new!", rhrdt_s_new_b, -1);
  rb_define_method(rhrdt_s_class, "now", rhrdt_s_now, -1);
  rb_define_method(rhrdt_s_class, "ordinal", rhrdt_s_ordinal, -1);

  rb_define_alias(rhrdt_s_class, "new", "civil");

  rb_define_method(rhrdt_class, "_dump", rhrdt__dump, 1);
  rb_define_method(rhrdt_class, "asctime", rhrdt_asctime, 0);
  rb_define_method(rhrdt_class, "cwday", rhrdt_cwday, 0);
  rb_define_method(rhrdt_class, "cweek", rhrdt_cweek, 0);
  rb_define_method(rhrdt_class, "cwyear", rhrdt_cwyear, 0);
  rb_define_method(rhrdt_class, "day", rhrdt_day, 0);
  rb_define_method(rhrdt_class, "day_fraction", rhrdt_day_fraction, 0);
  rb_define_method(rhrdt_class, "eql?", rhrdt_eql_q, 1);
  rb_define_method(rhrdt_class, "hash", rhrdt_hash, 0);
  rb_define_method(rhrdt_class, "hour", rhrdt_hour, 0);
  rb_define_method(rhrdt_class, "inspect", rhrdt_inspect, 0);
  rb_define_method(rhrdt_class, "jd", rhrdt_jd, 0);
  rb_define_method(rhrdt_class, "ld", rhrdt_ld, 0);
  rb_define_method(rhrdt_class, "leap?", rhrdt_leap_q, 0);
  rb_define_method(rhrdt_class, "min", rhrdt_min, 0);
  rb_define_method(rhrdt_class, "mjd", rhrdt_mjd, 0);
  rb_define_method(rhrdt_class, "month", rhrdt_month, 0);
  rb_define_method(rhrdt_class, "new_offset", rhrdt_new_offset, -1);
  rb_define_method(rhrdt_class, "offset", rhrdt_offset, 0);
  rb_define_method(rhrdt_class, "sec", rhrdt_sec, 0);
  rb_define_method(rhrdt_class, "sec_fraction", rhrdt_sec_fraction, 0);
  rb_define_method(rhrdt_class, "to_s", rhrdt_to_s, 0);
  rb_define_method(rhrdt_class, "year", rhrdt_year, 0);
  
  rb_define_method(rhrdt_class, ">>", rhrdt_op_right_shift, 1);
  rb_define_method(rhrdt_class, "<<", rhrdt_op_left_shift, 1);
  rb_define_method(rhrdt_class, "+", rhrdt_op_plus, 1);
  rb_define_method(rhrdt_class, "-", rhrdt_op_minus, 1);
  rb_define_method(rhrdt_class, "===", rhrdt_op_relationship, 1);
  rb_define_method(rhrdt_class, "<=>", rhrdt_op_spaceship, 1);

  rb_define_alias(rhrdt_class, "ajd", "jd");
  rb_define_alias(rhrdt_class, "amjd", "mjd");
  rb_define_alias(rhrdt_class, "ctime", "asctime");
  rb_define_alias(rhrdt_class, "mday", "day");
  rb_define_alias(rhrdt_class, "mon", "month");
  rb_define_alias(rhrdt_class, "newof", "new_offset");
  rb_define_alias(rhrdt_class, "of", "offset");

#ifdef RUBY19
#else
  rb_define_alias(rhrdt_s_class, "new0", "new!");
  rb_define_alias(rhrdt_s_class, "new1", "jd");
  rb_define_alias(rhrdt_s_class, "new2", "ordinal");
  rb_define_alias(rhrdt_s_class, "new3", "civil");
  rb_define_alias(rhrdt_s_class, "neww", "commercial");
#endif
}

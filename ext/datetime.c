
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

  dt->offset = lround(offset * 1440);
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

void rhrdt__nanos_to_hms(rhrdt_t *d) {
  unsigned int seconds;
  seconds = d->nanos/RHR_NANOS_PER_SECOND;
  d->hour = seconds/3600;
  d->minute = (seconds % 3600)/60;
  d->second = seconds % 60;
  d->flags |= RHR_HAVE_HMS;
}

void rhrdt__hms_to_nanos(rhrdt_t *d) {
  d->nanos = (d->hour*3600 + d->minute*60 + d->second)*RHR_NANOS_PER_SECOND;
  d->flags |= RHR_HAVE_NANOS;
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
  VALUE rt; 
  rt = rb_funcall(rb_cTime, rhrd_id_now, 0);
  offset = NUM2LONG(rb_funcall(rt, rhrd_id_utc_offset, 0));
  t = NUM2LONG(rb_funcall(rt, rhrd_id_to_i, 0)) + offset;
  dt->jd = rhrd__unix_to_jd(t);
#ifdef RUBY19
  dt->nanos = rhrd__mod(t, 86400) * RHR_NANOS_PER_SECOND + NUM2LONG(rb_funcall(rt, rhrd_id_nsec, 0));
#else
  dt->nanos = rhrd__mod(t, 86400) * RHR_NANOS_PER_SECOND + NUM2LONG(rb_funcall(rt, rhrd_id_usec, 0)) * 1000;
#endif
  dt->offset = offset/60;
  dt->flags |= RHR_HAVE_JD | RHR_HAVE_NANOS;
  RHR_CHECK_JD(dt);
}

VALUE rhrdt__from_jd_nanos(long jd, long long nanos, short offset) {
  long t;
  rhrdt_t *dt;
  VALUE new;
  new = Data_Make_Struct(rhrdt_class, rhrdt_t, NULL, free, dt);
  
  if (nanos < 0) {
    t = nanos/RHR_NANOS_PER_DAY - 1;
    nanos -= t * RHR_NANOS_PER_DAY;
    jd += t;
  } else if (nanos >= RHR_NANOS_PER_DAY) {
    t = nanos/RHR_NANOS_PER_DAY;
    nanos -= t * RHR_NANOS_PER_DAY;
    jd += t;
  }
  dt->jd = jd;
  dt->nanos = nanos;
  dt->offset = offset;
  dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS;
  return new;
}

long rhrdt__spaceship(rhrdt_t *dt, rhrdt_t *odt) {
  RHRDT_FILL_JD(dt)
  RHRDT_FILL_JD(odt)
  RHRDT_FILL_NANOS(dt)
  RHRDT_FILL_NANOS(odt)
  if (dt->jd == odt->jd) { 
    if (dt->nanos == odt->nanos) {
      return 0;
    } else if (dt->nanos < odt->nanos) {
      return -1;
    } 
    return 1;
  } else if (dt->jd < odt->jd) {
    return -1;
  } 
  return 1;
}

VALUE rhrdt__add_days(VALUE self, double n) {
  long l;
  long long nanos;
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_JD(dt)
  RHRDT_FILL_NANOS(dt)
  l = floor(n);
  nanos = llround((n - l) * RHR_NANOS_PER_DAY);
  return rhrdt__from_jd_nanos(rhrd__safe_add_long(dt->jd, l), dt->nanos + nanos, dt->offset);
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

void rhrdt__fill_from_hash(rhrdt_t *dt, VALUE hash) {
  rhrd_t d;
  long hour = 0;
  long minute = 0;
  long second = 0;
  long offset = 0;
  long long nanos = 0;
  long time_set = 0;
  int r;
  VALUE rhour, rmin, rsec, runix, roffset, rsecf;

  if (!RTEST(hash)) {
    rb_raise(rb_eArgError, "invalid date");
  }

  roffset = rb_hash_aref(hash, rhrd_sym_offset);
  if (RTEST(roffset)) {
    offset = NUM2LONG(roffset);
  }

  rsecf = rb_hash_aref(hash, rhrd_sym_sec_fraction);
  if (RTEST(rsecf)) {
    nanos = llround(NUM2DBL(rsecf) * RHR_NANOS_PER_SECOND);
  }

  runix = rb_hash_aref(hash, rhrd_sym_seconds);
  if (RTEST(runix)) {
    time_set = NUM2LONG(runix);
    dt->jd = rhrd__unix_to_jd(time_set);
    time_set = rhrd__mod(time_set, 86400);
    dt->nanos = time_set*RHR_NANOS_PER_SECOND + nanos;
    dt->hour = time_set/3600;
    dt->minute = (time_set - dt->hour * 3600)/60;
    dt->second = rhrd__mod(time_set, 60);
    offset /= 60;
    if (offset > 864 || offset < -864) {
      rb_raise(rb_eArgError, "invalid offset: %ld minutes", offset);
    }
    RHR_CHECK_JD(dt);
    dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS | RHR_HAVE_HMS;
    return;
  } else {
    rhour = rb_hash_aref(hash, rhrd_sym_hour);
    rmin = rb_hash_aref(hash, rhrd_sym_min);
    rsec = rb_hash_aref(hash, rhrd_sym_sec);
    
    if (RTEST(rhour)) {
      time_set = 1;
      hour = NUM2LONG(rhour);
    }
    if (RTEST(rmin)) {
      time_set = 1;
      minute = NUM2LONG(rmin);
    }
    if (RTEST(rsec)) {
      time_set = 1;
      second = NUM2LONG(rsec);
    }
  }

  memset(&d, 0, sizeof(rhrd_t));
  r = rhrd__fill_from_hash(&d, hash);
  if(r > 0) {
    /* bad date info */
    rb_raise(rb_eArgError, "invalid date");
  } else if (r < 0) {
    if (time_set) {
      /* time info but no date info, assume current date */
      rhrd__today(&d);
    } else {
      /* no time or date info */
      rb_raise(rb_eArgError, "invalid date");
    }
  } 

  if(RHR_HAS_JD(&d)) {
    dt->jd = d.jd;
    dt->flags |= RHR_HAVE_JD;
  }
  if(RHR_HAS_CIVIL(&d)) {
    dt->year = d.year;
    dt->month = d.month;
    dt->day = d.day;
    dt->flags |= RHR_HAVE_CIVIL;
  }
  if(time_set) {
    rhrdt__valid_time(dt, hour, minute, second, offset/86400.0);
    if(nanos) {
      RHRDT_FILL_NANOS(dt)
      dt->nanos += nanos;
    }
  } else if (offset) {
    if(!rhrdt__valid_offset(dt, offset/86400.0)){
      rb_raise(rb_eArgError, "invalid date");
    } 
  }
}

VALUE rhrdt__new_offset(VALUE self, double offset) {
  rhrdt_t *dt;
  long offset_min;

  if(offset < -0.6 || offset > 0.6) {
    rb_raise(rb_eArgError, "invalid offset (%f)", offset);
  } 
  offset_min = lround(offset * 1440.0);
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_JD(dt)
  RHRDT_FILL_NANOS(dt)
  return rhrdt__from_jd_nanos(dt->jd, dt->nanos - (dt->offset - offset_min)*RHR_NANOS_PER_MINUTE, offset_min);
}

/* Class methods */

/* call-seq:
 *   _load(string) -> DateTime
 *
 * Unmarshal a dumped +DateTime+ object. Not generally called directly,
 * usually called by <tt>Marshal.load</tt>.
 *
 * Note that this does not handle the marshalling format used by
 * the stdlib's +DateTime+, it only handles marshalled versions of
 * this library's +DateTime+ objects.
 */
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

  d->nanos = NUM2LL(rb_ary_entry(ary, 1));
  if (d->nanos < 0 || d->nanos >= RHR_NANOS_PER_DAY) {
    rb_raise(rb_eArgError, "invalid nanos: %lld", d->nanos);
  }

  x = NUM2LONG(rb_ary_entry(ary, 2));
  if (x > 864 || x < -864) {
    rb_raise(rb_eArgError, "invalid offset: %ld minutes", x);
  }
  d->offset = x;
  
  d->flags = RHR_HAVE_JD | RHR_HAVE_NANOS;
  return rd;
}

/* call-seq:
 *   _strptime(string, format='%FT%T%z') -> Hash
 *
 * Attemps to parse the string using the given format, returning
 * a hash if there is a match (or +nil+ if no match).
 *
 * +_strptime+ supports the same formats that <tt>DateTime#strftime</tt> does.
 */
static VALUE rhrdt_s__strptime(int argc, VALUE *argv, VALUE klass) {
  char * fmt_str = "%FT%T%z";
  long fmt_len = 7;
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

static VALUE rhrdt_s_civil(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *dt;
  long year;
  long month = 1;
  long day = 1;
  long hour = 0;
  long minute = 0;
  long second = 0;
  double offset = 0.0;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 0:
      dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS | RHR_HAVE_HMS;
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
#ifdef RUBY19
      break;
    case 0:
      dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS | RHR_HAVE_HMS;
      return rdt;
#else
    case 0:
      break;
#endif
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
      dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS | RHR_HAVE_HMS;
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
      dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS | RHR_HAVE_HMS;
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
      dt->nanos = (offset - dt->jd)*RHR_NANOS_PER_DAY;
      dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS;
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
  long year;
  long day = 1;
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
      dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS | RHR_HAVE_HMS;
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

static VALUE rhrdt_s_parse(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *dt;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 0:
      dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS | RHR_HAVE_HMS;
      return rdt;
    case 1:
      rhrdt__fill_from_hash(dt, rb_funcall(klass, rhrd_id__parse, 1, argv[0]));
      break;
    case 2:
    case 3:
      rhrdt__fill_from_hash(dt, rb_funcall(klass, rhrd_id__parse, 2, argv[0], argv[1]));
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 3)", argc);
      break;
  }

  return rdt;
}

static VALUE rhrdt_s_strptime(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *dt;
  VALUE rdt = Data_Make_Struct(klass, rhrdt_t, NULL, free, dt);

  switch(argc) {
    case 0:
      dt->flags = RHR_HAVE_JD | RHR_HAVE_NANOS | RHR_HAVE_HMS;
      return rdt;
    case 3:
      argc = 2;
    case 1:
    case 2:
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 3)", argc);
      break;
  }

  rhrdt__fill_from_hash(dt, rhrdt_s__strptime(argc, argv, klass));
  return rdt;
}

/* Instance methods */

static VALUE rhrdt__dump(VALUE self, VALUE limit) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  RHRDT_FILL_NANOS(d)
  return rb_marshal_dump(rb_ary_new3(3, LONG2NUM(d->jd), LL2NUM(d->nanos), LONG2NUM(d->offset)), LONG2NUM(NUM2LONG(limit) - 1));
}

static VALUE rhrdt_ajd(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return rb_float_new(d->jd + d->nanos/RHR_NANOS_PER_DAYD - d->offset/1440.0 + 0.5);
}

static VALUE rhrdt_amjd(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return rb_float_new(d->jd + d->nanos/RHR_NANOS_PER_DAYD - d->offset/1440.0 - RHR_JD_MJD);
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
    rb_raise(rb_eNoMemError, "in DateTime#asctime (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrdt_cwday(VALUE self) {
  rhrdt_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return LONG2NUM(n.day);
}

static VALUE rhrdt_cweek(VALUE self) {
  rhrdt_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return LONG2NUM(n.month);
}

static VALUE rhrdt_cwyear(VALUE self) {
  rhrdt_t *d;
  rhrd_t n;
  memset(&n, 0, sizeof(rhrd_t));
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  n.jd = d->jd;
  rhrd__fill_commercial(&n);
  return LONG2NUM(n.year);
}

static VALUE rhrdt_day(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  return LONG2NUM(dt->day);
}

static VALUE rhrdt_day_fraction(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_NANOS(dt)
  return rb_float_new(dt->nanos/RHR_NANOS_PER_DAYD);
}

static VALUE rhrdt_downto(VALUE self, VALUE other) {
  VALUE argv[2];
  argv[0] = other;
  argv[1] = INT2FIX(-1);
  return rhrdt_step(2, argv, self);
}

static VALUE rhrdt_eql_q(VALUE self, VALUE other) {
  rhrdt_t *dt, *odt;
  rhrd_t *o;
  long diff;

  if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
    self = rhrdt__new_offset(self, 0);
    other = rhrdt__new_offset(other, 0);
    Data_Get_Struct(self, rhrdt_t, dt);
    Data_Get_Struct(other, rhrdt_t, odt);
    return rhrdt__spaceship(dt, odt) == 0 ? Qtrue : Qfalse;
  } else if (RTEST(rb_obj_is_kind_of(other, rhrd_class))) {
    Data_Get_Struct(self, rhrdt_t, dt);
    Data_Get_Struct(other, rhrd_t, o);
    RHRDT_FILL_JD(dt)
    RHR_FILL_JD(o)
    RHR_SPACE_SHIP(diff, dt->jd, o->jd)
    if (diff == 0) {
      RHRDT_FILL_NANOS(dt)
      RHR_SPACE_SHIP(diff, dt->nanos, 0)
    }
    return diff == 0 ? Qtrue : Qfalse;
  }
  return Qfalse;
}

static VALUE rhrdt_hash(VALUE self) {
  rhrdt_t *d;

  self = rhrdt__new_offset(self, 0);
  Data_Get_Struct(self, rhrdt_t, d);
  return rb_funcall(rb_ary_new3(2, LONG2NUM(d->jd), LL2NUM(d->nanos)), rhrd_id_hash, 0);
}

static VALUE rhrdt_hour(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_HMS(dt)
  return LONG2NUM(dt->hour);
}

static VALUE rhrdt_inspect(VALUE self) {
  VALUE s;
  rhrdt_t *dt;
  int len;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  RHRDT_FILL_HMS(dt)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "#<DateTime %04li-%02hhi-%02hhiT%02hhi:%02hhi:%02hhi%+03i:%02i>",
        dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second, dt->offset/60, abs(dt->offset % 60));
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in DateTime#inspect (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrdt_jd(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return LONG2NUM(d->jd);
}

static VALUE rhrdt_ld(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return LONG2NUM(d->jd - RHR_JD_LD);
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
  return LONG2NUM(dt->minute);
}

static VALUE rhrdt_mjd(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return LONG2NUM(d->jd - RHR_JD_MJD);
}

static VALUE rhrdt_month(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  return LONG2NUM(dt->month);
}

static VALUE rhrdt_new_offset(int argc, VALUE *argv, VALUE self) {
  double offset;

  switch(argc) {
    case 0:
      offset = 0;
      break;
    case 1:
      if (RTEST(rb_obj_is_kind_of(argv[0], rb_cString))) {
        offset = NUM2LONG(rhrd_s_zone_to_diff(self, argv[0]))/86400.0;
      } else {
        offset = NUM2DBL(argv[0]);
      }
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }
  return rhrdt__new_offset(self, offset);
}

static VALUE rhrdt_next(VALUE self) {
   return rhrdt__add_days(self, 1);
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
  return LONG2NUM(dt->second);
}

static VALUE rhrdt_sec_fraction(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_NANOS(dt)
  return rb_float_new((dt->nanos % RHR_NANOS_PER_SECOND)/RHR_NANOS_PER_DAYD);
} 

static VALUE rhrdt_step(int argc, VALUE *argv, VALUE self) {
  rhrdt_t *d, *ndt, *d0;
  rhrd_t *nd;
  double step, limit;
  long long step_nanos, limit_nanos, current_nanos;
  long step_jd, limit_jd, current_jd;
  VALUE rlimit, new;
  Data_Get_Struct(self, rhrdt_t, d);
  Data_Get_Struct(rhrdt__new_offset(self, 0), rhrdt_t, d0);

  rb_need_block();
  switch(argc) {
    case 1:
      step_nanos = 0;
      step_jd = 1;
      break;
    case 2:
      step = NUM2DBL(argv[1]);
      step_jd = floor(step);
      step_nanos = llround((step - step_jd)*RHR_NANOS_PER_DAY);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 2", argc);
      break;
  }

  rlimit = argv[0];
  if (RTEST(rb_obj_is_kind_of(rlimit, rb_cNumeric))) {
    limit = NUM2DBL(rlimit);
    limit_jd = floor(limit);
    limit_nanos = llround((limit - limit_jd)*RHR_NANOS_PER_DAY);
  } else if (RTEST((rb_obj_is_kind_of(rlimit, rhrdt_class)))) {
    rlimit = rhrdt__new_offset(rlimit, 0);
    Data_Get_Struct(rlimit, rhrdt_t, ndt);
    RHRDT_FILL_JD(ndt)
    RHRDT_FILL_NANOS(ndt)
    limit_jd = ndt->jd; 
    limit_nanos = ndt->nanos;
  } else if (RTEST((rb_obj_is_kind_of(rlimit, rhrd_class)))) {
    Data_Get_Struct(rlimit, rhrd_t, nd);
    RHR_FILL_JD(nd)
    limit_jd = nd->jd; 
    limit_nanos = d->offset*RHR_NANOS_PER_MINUTE;
    if (limit_nanos < 0) {
      limit_jd--;
      limit_nanos += RHR_NANOS_PER_DAY;
    }
  } else {
    rb_raise(rb_eTypeError, "expected numeric or date");
  }

  current_jd = d0->jd;
  current_nanos = d0->nanos;
  new = rhrdt__from_jd_nanos(current_jd, current_nanos, d->offset);
  if (limit_jd > current_jd || (limit_jd == current_jd && limit_nanos > current_nanos)) {
    if (step_jd > 0 || (step_jd == 0 && step_nanos > 0)) {
      while (limit_jd >= current_jd || (limit_jd == current_jd && limit_nanos >= current_nanos)) {
        rb_yield(new);
        new = rhrdt__from_jd_nanos(current_jd + step_jd, current_nanos + step_nanos, d->offset);
        Data_Get_Struct(new, rhrdt_t, ndt);
        current_jd = ndt->jd;
        current_nanos = ndt->nanos;
      }
    }
  } else if (limit_jd < current_jd || (limit_jd == current_jd && limit_nanos < current_nanos)) {
    if (step_jd < 0 || (step_jd == 0 && step_nanos < 0)) {
      while (limit_jd <= current_jd || (limit_jd == current_jd && limit_nanos <= current_nanos)) {
        rb_yield(new);
        new = rhrdt__from_jd_nanos(current_jd + step_jd, current_nanos + step_nanos, d->offset);
        Data_Get_Struct(new, rhrdt_t, ndt);
        current_jd = ndt->jd;
        current_nanos = ndt->nanos;
      }
    }
  } else {
    rb_yield(self);
  }

  return self;
}

static VALUE rhrdt_strftime(int argc, VALUE *argv, VALUE self) {
  rhrdt_t* dt;
  VALUE r;

  switch(argc) {
    case 0:
      return rhrdt_to_s(self);
    case 1:
      r = rb_str_to_str(argv[0]);
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  RHRDT_FILL_JD(dt)
  RHRDT_FILL_HMS(dt)
  RHRDT_FILL_NANOS(dt)
  return rhrd__strftime(dt, RSTRING_PTR(r), RSTRING_LEN(r));
}

static VALUE rhrdt_to_s(VALUE self) {
  VALUE s;
  rhrdt_t *dt;
  int len;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  RHRDT_FILL_HMS(dt)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%04li-%02hhi-%02hhiT%02hhi:%02hhi:%02hhi%+03i:%02i",
        dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second, dt->offset/60, abs(dt->offset % 60));
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in DateTime#to_s (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrdt_upto(VALUE self, VALUE other) {
  VALUE argv[1];
  argv[0] = other;
  return rhrdt_step(1, argv, self);
}

static VALUE rhrdt_wday(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return LONG2NUM(rhrd__jd_to_wday(d->jd));
}

static VALUE rhrdt_yday(VALUE self) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_CIVIL(d)
  return LONG2NUM(rhrd__ordinal_day(d->year, d->month, d->day));
}

static VALUE rhrdt_year(VALUE self) {
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  return LONG2NUM(dt->year);
}

static VALUE rhrdt_zone(VALUE self) {
  VALUE s;
  rhrdt_t *dt;
  int len;
  Data_Get_Struct(self, rhrdt_t, dt);

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%+03i:%02i", dt->offset/60, abs(dt->offset % 60));
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in DateTime#zone (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
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

  if (RTEST(rb_obj_is_kind_of(other, rb_cNumeric))) {
    Data_Get_Struct(self, rhrdt_t, dt);
    return rhrdt__add_days(self, -NUM2DBL(other));
  }
  if (RTEST((rb_obj_is_kind_of(other, rhrdt_class)))) {
    self = rhrdt__new_offset(self, 0);
    other = rhrdt__new_offset(other, 0);
    Data_Get_Struct(self, rhrdt_t, dt);
    Data_Get_Struct(other, rhrdt_t, newdt);
    RHRDT_FILL_JD(dt)
    RHRDT_FILL_NANOS(dt)
    RHRDT_FILL_JD(newdt)
    RHRDT_FILL_NANOS(newdt)
    if (dt->nanos == newdt->nanos) {
      return rb_float_new(dt->jd - newdt->jd);
    } else if (dt->jd == newdt->jd) 
      return rb_float_new((dt->nanos - newdt->nanos)/RHR_NANOS_PER_DAYD);
    else {
      return rb_float_new((dt->jd - newdt->jd) + (dt->nanos - newdt->nanos)/RHR_NANOS_PER_DAYD);
    }
  }
  if (RTEST((rb_obj_is_kind_of(other, rhrd_class)))) {
    Data_Get_Struct(self, rhrdt_t, dt);
    Data_Get_Struct(other, rhrd_t, newd);
    RHRDT_FILL_JD(dt)
    RHRDT_FILL_NANOS(dt)
    RHR_FILL_JD(newd)
    return rb_float_new((dt->jd - newd->jd) + dt->nanos/RHR_NANOS_PER_DAYD); 
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

  if (RTEST(rb_obj_is_kind_of(other, rhrdt_class))) {
    self = rhrdt__new_offset(self, 0);
    other = rhrdt__new_offset(other, 0);
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
  return Qnil;
}

#ifdef RUBY19

/* 1.9 helper methods */

VALUE rhrdt__add_years(VALUE self, long n) {
  rhrdt_t *d;
  rhrdt_t *newd;
  VALUE new;
  Data_Get_Struct(self, rhrdt_t, d);

  new = Data_Make_Struct(rhrdt_class, rhrdt_t, NULL, free, newd);
  RHRDT_FILL_CIVIL(d)
  memcpy(newd, d, sizeof(rhrdt_t));

  newd->year = rhrd__safe_add_long(n, d->year);
  if(d->month == 2 && d->day == 29 && !rhrd__leap_year(newd->year)) {
    newd->day = 28;
  } 

  RHR_CHECK_CIVIL(newd)
  newd->flags &= ~RHR_HAVE_JD;
  return new;
}

VALUE rhrdt__day_q(VALUE self, long day) {
  rhrdt_t *d;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  return rhrd__jd_to_wday(d->jd) == day ? Qtrue : Qfalse;
}

long rhrdt__add_iso_time_format(rhrdt_t *dt, char *str, long len, long i) {
  int l;

  RHRDT_FILL_HMS(dt)

  if (i < 1) {
    i = 0;
  } else if (i > 9) {
    i = 9;
  }

  l = snprintf(str + len, 128 - len, "T%02hhi:%02hhi:%02hhi", dt->hour, dt->minute, dt->second);
  if (l == -1 || l > 127) {
    rb_raise(rb_eNoMemError, "in DateTime formatting method (in snprintf)");
  }
  len += l;

  if (i) {
    RHRDT_FILL_NANOS(dt)
    l = snprintf(str + len, 128 - len, ".%09lli", dt->nanos % RHR_NANOS_PER_SECOND);
    if (l == -1 || l > 127) {
      rb_raise(rb_eNoMemError, "in DateTime formatting method (in snprintf)");
    }
    len += i + 1;
  }

  l = snprintf(str + len, 128 - len, "%+03i:%02i", dt->offset/60, abs(dt->offset % 60));
  if (l == -1 || l > 127) {
    rb_raise(rb_eNoMemError, "in DateTime formatting method (in snprintf)");
  }

  return len + l;
}

/* 1.9 class methods */

static VALUE rhrdt_s_httpdate(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *d;
  VALUE rd;
  rd = Data_Make_Struct(klass, rhrdt_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->flags = RHR_HAVE_JD | RHR_HAVE_HMS | RHR_HAVE_NANOS;
      return rd;
    case 1:
    case 2:
      rhrdt__fill_from_hash(d, rb_funcall(klass, rhrd_id__httpdate, 1, argv[0]));
      return rd;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrdt_s_iso8601(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *d;
  VALUE rd;
  rd = Data_Make_Struct(klass, rhrdt_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->flags = RHR_HAVE_JD | RHR_HAVE_HMS | RHR_HAVE_NANOS;
      return rd;
    case 1:
    case 2:
      rhrdt__fill_from_hash(d, rb_funcall(klass, rhrd_id__iso8601, 1, argv[0]));
      return rd;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrdt_s_jisx0301(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *d;
  VALUE rd;
  rd = Data_Make_Struct(klass, rhrdt_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->flags = RHR_HAVE_JD | RHR_HAVE_HMS | RHR_HAVE_NANOS;
      return rd;
    case 1:
    case 2:
      rhrdt__fill_from_hash(d, rb_funcall(klass, rhrd_id__jisx0301, 1, argv[0]));
      return rd;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrdt_s_rfc2822(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *d;
  VALUE rd;
  rd = Data_Make_Struct(klass, rhrdt_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->flags = RHR_HAVE_JD | RHR_HAVE_HMS | RHR_HAVE_NANOS;
      return rd;
    case 1:
    case 2:
      rhrdt__fill_from_hash(d, rb_funcall(klass, rhrd_id__rfc2822, 1, argv[0]));
      return rd;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrdt_s_rfc3339(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *d;
  VALUE rd;
  rd = Data_Make_Struct(klass, rhrdt_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->flags = RHR_HAVE_JD | RHR_HAVE_HMS | RHR_HAVE_NANOS;
      return rd;
    case 1:
    case 2:
      rhrdt__fill_from_hash(d, rb_funcall(klass, rhrd_id__rfc3339, 1, argv[0]));
      return rd;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

static VALUE rhrdt_s_xmlschema(int argc, VALUE *argv, VALUE klass) {
  rhrdt_t *d;
  VALUE rd;
  rd = Data_Make_Struct(klass, rhrdt_t, NULL, free, d);

  switch(argc) {
    case 0:
      d->flags = RHR_HAVE_JD | RHR_HAVE_HMS | RHR_HAVE_NANOS;
      return rd;
    case 1:
    case 2:
      rhrdt__fill_from_hash(d, rb_funcall(klass, rhrd_id__xmlschema, 1, argv[0]));
      return rd;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments (%i for 2)", argc);
      break;
  }
}

/* 1.9 instance methods */

static VALUE rhrdt_httpdate(VALUE self) {
  VALUE s;
  rhrdt_t *d;
  int len;
  s = rhrdt__new_offset(self, 0);
  Data_Get_Struct(s, rhrdt_t, d);
  RHRDT_FILL_JD(d)
  RHRDT_FILL_CIVIL(d)
  RHRDT_FILL_HMS(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s, %02hhi %s %04li %02hhi:%02hhi:%02hhi GMT", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        d->day,
        rhrd__abbr_month_names[d->month],
        d->year, d->hour, d->minute, d->second);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in DateTime#httpdate (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrdt_iso8601(int argc, VALUE *argv, VALUE self) {
  long i;
  VALUE s;
  rhrdt_t *dt;
  char * str;
  int len;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)

  switch(argc) {
    case 1:
      i = NUM2LONG(argv[0]);
      break;
    case 0:
      i = 0;
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  s = rb_str_buf_new(128);
  str = RSTRING_PTR(s);

  len = snprintf(str, 128, "%04li-%02hhi-%02hhi", dt->year, dt->month, dt->day);
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in DateTime#to_s (in snprintf)");
  }

  len = rhrdt__add_iso_time_format(dt, str, len, i);
  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrdt_jisx0301(int argc, VALUE *argv, VALUE self) {
  VALUE s;
  rhrdt_t *d;
  int len;
  int i;
  char c;
  char * str;
  long year;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_CIVIL(d)
  RHRDT_FILL_JD(d)

  switch(argc) {
    case 1:
      i = NUM2LONG(argv[0]);
      break;
    case 0:
      i = 0;
      break;
    default:
      rb_raise(rb_eArgError, "wrong number of arguments: %i for 1", argc);
      break;
  }

  s = rb_str_buf_new(128);
  str = RSTRING_PTR(s); 

  if (d->jd < 2405160) {
    len = snprintf(str, 128, "%04li-%02hhi-%02hhi", d->year, d->month, d->day);
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
    rb_raise(rb_eNoMemError, "in DateTime#jisx0301 (in snprintf)");
  }

  len = rhrdt__add_iso_time_format(d, str, len, i);
  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrdt_next_day(int argc, VALUE *argv, VALUE self) {
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

   return rhrdt__add_days(self, i);
}

static VALUE rhrdt_next_month(int argc, VALUE *argv, VALUE self) {
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

  return rhrdt__add_months(self, i);
}

static VALUE rhrdt_next_year(int argc, VALUE *argv, VALUE self) {
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

  return rhrdt__add_years(self, i);
}

static VALUE rhrdt_prev_day(int argc, VALUE *argv, VALUE self) {
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

   return rhrdt__add_days(self, i);
}

static VALUE rhrdt_prev_month(int argc, VALUE *argv, VALUE self) {
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

  return rhrdt__add_months(self, i);
}

static VALUE rhrdt_prev_year(int argc, VALUE *argv, VALUE self) {
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

  return rhrdt__add_years(self, i);
}

static VALUE rhrdt_rfc2822(VALUE self) {
  VALUE s;
  rhrdt_t *d;
  int len;
  Data_Get_Struct(self, rhrdt_t, d);
  RHRDT_FILL_CIVIL(d)
  RHRDT_FILL_JD(d)
  RHRDT_FILL_HMS(d)

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "%s, %hhi %s %04li %02hhi:%02hhi:%02hhi %+03i%02i", 
        rhrd__abbr_day_names[rhrd__jd_to_wday(d->jd)],
        d->day,
        rhrd__abbr_month_names[d->month],
        d->year, d->hour, d->minute, d->second, d->offset/60, abs(d->offset % 60));
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in DateTime#rfc2822 (in snprintf)");
  }

  RHR_RETURN_RESIZED_STR(s, len)
}

static VALUE rhrdt_to_date(VALUE self) {
  rhrd_t *d;
  rhrdt_t *dt;
  VALUE rd = Data_Make_Struct(rhrd_class, rhrd_t, NULL, free, d);
  Data_Get_Struct(self, rhrdt_t, dt);

  if (RHR_HAS_CIVIL(dt)) {
    d->year = dt->year;
    d->month = dt->month;
    d->day = dt->day;
    d->flags |= RHR_HAVE_CIVIL;
  }
  if (RHR_HAS_JD(dt)) {
    d->jd = dt->jd;
    d->flags |= RHR_HAVE_JD;
  }

  return rd;
}

static VALUE rhrdt_to_time(VALUE self) {
  long h, m, s;
  rhrdt_t *dt;
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_JD(dt)
  RHRDT_FILL_NANOS(dt)
  self = rhrdt__from_jd_nanos(dt->jd, dt->nanos - dt->offset * RHR_NANOS_PER_MINUTE, 0);
  Data_Get_Struct(self, rhrdt_t, dt);
  RHRDT_FILL_CIVIL(dt)
  RHRDT_FILL_HMS(dt)

  s = dt->nanos/RHR_NANOS_PER_SECOND;
  h = s/3600;
  m = (s % 3600) / 60;
  return rb_funcall(rb_funcall(rb_cTime, rhrd_id_utc, 6, LONG2NUM(dt->year), LONG2NUM(dt->month), LONG2NUM(dt->day), LONG2NUM(h), LONG2NUM(m), rb_float_new(s % 60 + (dt->nanos % RHR_NANOS_PER_SECOND)/RHR_NANOS_PER_SECONDD)), rhrd_id_localtime, 0);
}

static VALUE rhrdt_time_to_datetime(VALUE self) {
  rhrdt_t *dt;
  VALUE rd;
  long t, offset;
  rd = Data_Make_Struct(rhrdt_class, rhrdt_t, NULL, free, dt);

  offset = NUM2LONG(rb_funcall(self, rhrd_id_utc_offset, 0));
  t = NUM2LONG(rb_funcall(self, rhrd_id_to_i, 0)) + offset;
  dt->jd = rhrd__unix_to_jd(t);
#ifdef RUBY19
  dt->nanos = rhrd__mod(t, 86400) * RHR_NANOS_PER_SECOND + NUM2LONG(rb_funcall(self, rhrd_id_nsec, 0));
#else
  dt->nanos = rhrd__mod(t, 86400) * RHR_NANOS_PER_SECOND + NUM2LONG(rb_funcall(self, rhrd_id_usec, 0)) * 1000;
#endif
  dt->offset = offset/60;
  dt->flags |= RHR_HAVE_JD | RHR_HAVE_NANOS;
  RHR_CHECK_JD(dt);
  return rd;
}

/* 1.9 day? instance methods */

static VALUE rhrdt_sunday_q(VALUE self) {
  return rhrdt__day_q(self, 0);
}

static VALUE rhrdt_monday_q(VALUE self) {
  return rhrdt__day_q(self, 1);
}

static VALUE rhrdt_tuesday_q(VALUE self) {
  return rhrdt__day_q(self, 2);
}

static VALUE rhrdt_wednesday_q(VALUE self) {
  return rhrdt__day_q(self, 3);
}

static VALUE rhrdt_thursday_q(VALUE self) {
  return rhrdt__day_q(self, 4);
}

static VALUE rhrdt_friday_q(VALUE self) {
  return rhrdt__day_q(self, 5);
}

static VALUE rhrdt_saturday_q(VALUE self) {
  return rhrdt__day_q(self, 6);
}

#endif

/* Library initialization */

void Init_datetime(void) {
  rhrdt_class = rb_define_class("DateTime", rhrd_class);
  rb_undef_alloc_func(rhrdt_class);
  rhrdt_s_class = rb_singleton_class(rhrdt_class);

  rb_undef(rhrdt_s_class, rb_intern("today"));
  rb_define_method(rhrdt_s_class, "_load", rhrdt_s__load, 1);
  rb_define_method(rhrdt_s_class, "_strptime", rhrdt_s__strptime, -1);
  rb_define_method(rhrdt_s_class, "civil", rhrdt_s_civil, -1);
  rb_define_method(rhrdt_s_class, "commercial", rhrdt_s_commercial, -1);
  rb_define_method(rhrdt_s_class, "jd", rhrdt_s_jd, -1);
  rb_define_method(rhrdt_s_class, "new!", rhrdt_s_new_b, -1);
  rb_define_method(rhrdt_s_class, "now", rhrdt_s_now, -1);
  rb_define_method(rhrdt_s_class, "ordinal", rhrdt_s_ordinal, -1);
  rb_define_method(rhrdt_s_class, "parse", rhrdt_s_parse, -1);
  rb_define_method(rhrdt_s_class, "strptime", rhrdt_s_strptime, -1);

  rb_define_alias(rhrdt_s_class, "new", "civil");

  rb_define_method(rhrdt_class, "_dump", rhrdt__dump, 1);
  rb_define_method(rhrdt_class, "ajd", rhrdt_ajd, 0);
  rb_define_method(rhrdt_class, "amjd", rhrdt_amjd, 0);
  rb_define_method(rhrdt_class, "asctime", rhrdt_asctime, 0);
  rb_define_method(rhrdt_class, "cwday", rhrdt_cwday, 0);
  rb_define_method(rhrdt_class, "cweek", rhrdt_cweek, 0);
  rb_define_method(rhrdt_class, "cwyear", rhrdt_cwyear, 0);
  rb_define_method(rhrdt_class, "day", rhrdt_day, 0);
  rb_define_method(rhrdt_class, "day_fraction", rhrdt_day_fraction, 0);
  rb_define_method(rhrdt_class, "downto", rhrdt_downto, 1);
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
  rb_define_method(rhrdt_class, "next", rhrdt_next, 0);
  rb_define_method(rhrdt_class, "offset", rhrdt_offset, 0);
  rb_define_method(rhrdt_class, "sec", rhrdt_sec, 0);
  rb_define_method(rhrdt_class, "sec_fraction", rhrdt_sec_fraction, 0);
  rb_define_method(rhrdt_class, "step", rhrdt_step, -1);
  rb_define_method(rhrdt_class, "strftime", rhrdt_strftime, -1);
  rb_define_method(rhrdt_class, "to_s", rhrdt_to_s, 0);
  rb_define_method(rhrdt_class, "upto", rhrdt_upto, 1);
  rb_define_method(rhrdt_class, "wday", rhrdt_wday, 0);
  rb_define_method(rhrdt_class, "yday", rhrdt_yday, 0);
  rb_define_method(rhrdt_class, "year", rhrdt_year, 0);
  rb_define_method(rhrdt_class, "zone", rhrdt_zone, 0);
  
  rb_define_method(rhrdt_class, ">>", rhrdt_op_right_shift, 1);
  rb_define_method(rhrdt_class, "<<", rhrdt_op_left_shift, 1);
  rb_define_method(rhrdt_class, "+", rhrdt_op_plus, 1);
  rb_define_method(rhrdt_class, "-", rhrdt_op_minus, 1);
  rb_define_method(rhrdt_class, "===", rhrdt_op_relationship, 1);
  rb_define_method(rhrdt_class, "<=>", rhrdt_op_spaceship, 1);

  rb_define_alias(rhrdt_class, "ctime", "asctime");
  rb_define_alias(rhrdt_class, "mday", "day");
  rb_define_alias(rhrdt_class, "mon", "month");
  rb_define_alias(rhrdt_class, "succ", "next");

#ifdef RUBY19
  rb_define_method(rhrdt_s_class, "httpdate", rhrdt_s_httpdate, -1);
  rb_define_method(rhrdt_s_class, "iso8601", rhrdt_s_iso8601, -1);
  rb_define_method(rhrdt_s_class, "jisx0301", rhrdt_s_jisx0301, -1);
  rb_define_method(rhrdt_s_class, "rfc2822", rhrdt_s_rfc2822, -1);
  rb_define_method(rhrdt_s_class, "rfc3339", rhrdt_s_rfc3339, -1);
  rb_define_method(rhrdt_s_class, "xmlschema", rhrdt_s_xmlschema, -1);

  rb_define_alias(rhrdt_s_class, "rfc822", "rfc2822");

  rb_define_method(rhrdt_class, "httpdate", rhrdt_httpdate, 0);
  rb_define_method(rhrdt_class, "iso8601", rhrdt_iso8601, -1);
  rb_define_method(rhrdt_class, "jisx0301", rhrdt_jisx0301, -1);
  rb_define_method(rhrdt_class, "next_day", rhrdt_next_day, -1);
  rb_define_method(rhrdt_class, "next_month", rhrdt_next_month, -1);
  rb_define_method(rhrdt_class, "next_year", rhrdt_next_year, -1);
  rb_define_method(rhrdt_class, "prev_day", rhrdt_prev_day, -1);
  rb_define_method(rhrdt_class, "prev_month", rhrdt_prev_month, -1);
  rb_define_method(rhrdt_class, "prev_year", rhrdt_prev_year, -1);
  rb_define_method(rhrdt_class, "rfc2822", rhrdt_rfc2822, 0);
  rb_define_method(rhrdt_class, "to_date", rhrdt_to_date, 0);
  rb_define_method(rhrdt_class, "to_time", rhrdt_to_time, 0);

  rb_define_alias(rhrdt_class, "minute", "min");
  rb_define_alias(rhrdt_class, "rfc3339", "iso8601");
  rb_define_alias(rhrdt_class, "rfc822", "rfc2822");
  rb_define_alias(rhrdt_class, "second", "sec");
  rb_define_alias(rhrdt_class, "second_fraction", "sec_fraction");
  rb_define_alias(rhrdt_class, "to_datetime", "gregorian");
  rb_define_alias(rhrdt_class, "xmlschema", "iso8601");

  rb_define_method(rhrdt_class, "sunday?", rhrdt_sunday_q, 0);
  rb_define_method(rhrdt_class, "monday?", rhrdt_monday_q, 0);
  rb_define_method(rhrdt_class, "tuesday?", rhrdt_tuesday_q, 0);
  rb_define_method(rhrdt_class, "wednesday?", rhrdt_wednesday_q, 0);
  rb_define_method(rhrdt_class, "thursday?", rhrdt_thursday_q, 0);
  rb_define_method(rhrdt_class, "friday?", rhrdt_friday_q, 0);
  rb_define_method(rhrdt_class, "saturday?", rhrdt_saturday_q, 0);

  rb_define_method(rb_cTime, "to_datetime", rhrdt_time_to_datetime, 0);
#else
  rb_define_alias(rhrdt_s_class, "new0", "new!");
  rb_define_alias(rhrdt_s_class, "new1", "jd");
  rb_define_alias(rhrdt_s_class, "new2", "ordinal");
  rb_define_alias(rhrdt_s_class, "new3", "civil");
  rb_define_alias(rhrdt_s_class, "neww", "commercial");

  rb_define_alias(rhrdt_class, "newof", "new_offset");
  rb_define_alias(rhrdt_class, "of", "offset");
#endif
}

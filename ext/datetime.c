
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

#define RHRDT_FILL_JD(d) if (((d)->flags & RHR_HAVE_JD) == 0) { rhrdt__civil_to_jd(d); }
#define RHRDT_FILL_CIVIL(d) if (((d)->flags & RHR_HAVE_CIVIL) == 0) { rhrdt__jd_to_civil(d); }
#define RHRDT_FILL_HMS(d) if (((d)->flags & RHR_HAVE_CIVIL) == 0) { rhrdt__fraction_to_hms(d); }

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
  if (offset < -0.6 || offset > 0.6) {
    return 0;
  }

  dt->hour = h;
  dt->minute = m;
  dt->second = s;
  dt->offset = round(offset * 1440);
  return 1;
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
      break;
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

/* Instance methods */

static VALUE rhrdt_inspect(VALUE self) {
  VALUE s;
  rhrdt_t *dt;
  int len;
  Data_Get_Struct(self, rhrdt_t, dt);
  /*
  RHR_FILL_CIVIL(dt)
  RHR_FILL_HMS(dt)
  */

  s = rb_str_buf_new(128);
  len = snprintf(RSTRING_PTR(s), 128, "#<DateTime %04li-%02hhi-%02hhiT%02hhi:%02hhi:%02hhi%+03i:%02li>",
        dt->year, dt->month, dt->day, dt->hour, dt->minute, dt->second, dt->offset/60, rhrd__mod(dt->offset, 60));
  if (len == -1 || len > 127) {
    rb_raise(rb_eNoMemError, "in Date#inspect (in snprintf)");
  }

  return rb_str_resize(s, len);
}


/* Library initialization */

void Init_datetime(void) {
  rhrdt_class = rb_define_class("DateTime", rb_cObject);
  rhrdt_s_class = rb_singleton_class(rhrdt_class);

  rb_define_method(rhrdt_s_class, "civil", rhrdt_s_civil, -1);

  rb_define_method(rhrdt_class, "inspect", rhrdt_inspect, 0);
}

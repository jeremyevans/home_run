
#ifndef DATE_EXT_H
#define DATE_EXT_H 1

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ruby.h>

#if defined(HAVE_RUBY_ENCODING_H) && HAVE_RUBY_ENCODING_H
#define RHR_ENCODING 1
#include <ruby/encoding.h>
#endif

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
#define RHR_SECONDS_PER_HOUR 3600
#define RHR_MINUTES_PER_DAYD 1440.0
#define RHR_SECONDS_PER_DAY 86400
#define RHR_SECONDS_PER_DAYD 86400.0
#define RHR_MAX_OFFSET 840
#define RHR_MIN_OFFSET -840

/*
In both the 32-bit and 64-bit cases, the limits are chosen so that you cannot
store a civil date where converting it to a jd would cause an overflow.
*/
#if __LP64__
/* 
On 64-bit systems, the limits depend on the number of significant digits in
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

#define RHR_NO_RAISE 0
#define RHR_OVERLIMIT_RAISE 1

#define RHR_NANOS_PER_MILLISECOND 1000000LL
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
#define RHRR_UNIXM_SET 0x80000

#define RHR_HAS_JD(d) (((d)->flags & RHR_HAVE_JD) == RHR_HAVE_JD)
#define RHR_HAS_CIVIL(d) (((d)->flags & RHR_HAVE_CIVIL) == RHR_HAVE_CIVIL)

#define RHR_FILL_JD(d) if (((d)->flags & RHR_HAVE_JD) == 0) { rhrd__civil_to_jd(d); }
#define RHR_FILL_CIVIL(d) if (((d)->flags & RHR_HAVE_CIVIL) == 0) { rhrd__jd_to_civil(d); }

#define RHRDT_FILL_JD(d) if (!((d)->flags & RHR_HAVE_JD)) { rhrdt__civil_to_jd(d); }
#define RHRDT_FILL_CIVIL(d) if (!((d)->flags & RHR_HAVE_CIVIL)) { rhrdt__jd_to_civil(d); }
#define RHRDT_FILL_HMS(d) if (!((d)->flags & RHR_HAVE_HMS)) { rhrdt__nanos_to_hms(d); }
#define RHRDT_FILL_NANOS(d) if (!((d)->flags & RHR_HAVE_NANOS)) { rhrdt__hms_to_nanos(d); }

#ifdef RHR_ENCODING
#define RHR_ASCII_ENCODING(s) s = rb_enc_associate_index(s, rhrd_encoding_index); \
  if(rb_default_internal_encoding()) {s = rb_str_export_to_enc(s, rb_default_internal_encoding());}
#else
#define RHR_ASCII_ENCODING(s) /* do nothing */
#endif

#ifdef RUBY186
#define RHR_RETURN_RESIZED_STR(s, len) return rb_str_resize(s, len);
#else
#define RHR_RETURN_RESIZED_STR(s, len) rb_str_set_len(s, len); RHR_ASCII_ENCODING(s); return s;
#endif

#define RHR_SPACE_SHIP(x, l, r) if (l < r) { x = -1; } else if (l == r) { x = 0; } else { x = 1; } 

#define RHR_CHECK_JD(d) if ((d->jd > RHR_JD_MAX) || (d->jd < RHR_JD_MIN)) { rb_raise(rb_eRangeError, "date out of range: jd = %li", d->jd);}
#define RHR_CHECK_CIVIL(d) if (!rhrd__valid_civil_limits(d->year, d->month, d->day)) { rb_raise(rb_eRangeError, "date out of range: year = %li, month = %i, day = %i", d->year, (int)d->month, (int)d->day);}

#define RHR_CACHED_IV(self, iv) VALUE v = rb_ivar_get(self, iv); if (RTEST(v)) {return v;} 

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

void rhrdt__civil_to_jd(rhrdt_t *d);
void rhrdt__hms_to_nanos(rhrdt_t *d);
int rhrd__leap_year(long year);
int rhrd__valid_civil_limits(long year, long month, long day);
long rhrd__ymd_to_jd(long year, long month, long day);
long rhrd__commercial_to_jd(long cwyear, long cweek, long cwday);
void rhrd__fill_commercial(rhrd_t *d);
long rhrd__unix_to_jd(long long t);
long rhrd__mod(long a, long b);
long rhrd__modll(long long a, long b);
long rhrd__safe_add_long(long a, long b);
unsigned char rhrd__days_in_month(long year, unsigned char month);
int rhrd__fill_from_hash(rhrd_t *d, VALUE hash);
void rhrd__today(rhrd_t * d);
VALUE rhrd__strptime(VALUE rstr, const char *fmt_str, long fmt_len);
long rhrd__jd_to_wday(long jd);
void rhrd__set_cw_ivs(VALUE self, rhrd_t *d);
void rhrd__civil_to_jd(rhrd_t *d);
VALUE rhrd_s_zone_to_diff(VALUE self, VALUE zone);
VALUE rhrd__strftime(rhrdt_t *d, const char * fmt, int fmt_len);
long rhrd__ordinal_day(long year, unsigned char month, unsigned char day);
VALUE rhrd__ragel_parse(char * p, long len);
VALUE rhrdt__new_offset(VALUE self, double offset);
long rhrdt__spaceship(rhrdt_t *dt, rhrdt_t *odt);
void Init_datetime(void);

#endif /* DATE_EXT_H */

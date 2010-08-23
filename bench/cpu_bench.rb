load(File.join(File.dirname(File.expand_path(__FILE__)), 'cpu_bench_util.rb'))
n = N
compare("._parse"){|dc| n.times{dc._parse('2010-12-13')}}
compare("._strptime"){|dc| n.times{dc._strptime('fri jan 5 00:00:00 2007', '%c')}}
compare(".civil"){|dc| n.times{dc.civil(2010, 1, 1)}}
compare(".commercial"){|dc| n.times{dc.commercial(2010, 1, 1)}}
compare(".gregorian_leap?"){|dc| n.times{dc.gregorian_leap?(2000)}}
compare(".jd"){|dc| n.times{dc.jd(2010)}}
compare(".julian_leap?"){|dc| n.times{dc.julian_leap?(2000)}}
compare(".new!"){|dc| n.times{dc.new!(2012)}}
compare(".ordinal"){|dc| n.times{dc.ordinal(2012, 100)}}
compare(".parse"){|dc| n.times{dc.parse('2010-12-13')}}
compare(".strptime"){|dc| n.times{dc.strptime('fri jan 5 00:00:00 2007', '%c')}}
compare(".today"){|dc| n.times{dc.today}}
compare(".valid_civil?"){|dc| n.times{dc.valid_civil?(2010, 1, 1)}}
compare(".valid_commercial?"){|dc| n.times{dc.valid_commercial?(2010, 1, 1)}}
compare(".valid_jd?"){|dc| n.times{dc.valid_jd?(2010)}}
compare(".valid_ordinal?"){|dc| n.times{dc.valid_jd?(2010, 100)}}

dt_compare("._parse"){|dc| n.times{dc._parse('2010-12-13 10:20:30-07:00')}}
dt_compare("._strptime"){|dc| n.times{dc._strptime('fri jan 5 13:43:37 2007', '%c')}}
dt_compare(".civil"){|dc| n.times{dc.civil(2010, 1, 1, 13, 43, 57)}}
dt_compare(".commercial"){|dc| n.times{dc.commercial(2010, 1, 1, 13, 43, 57)}}
dt_compare(".jd"){|dc| n.times{dc.jd(2010, 13, 43, 57)}}
dt_compare(".new!"){|dc| n.times{dc.new!(201013.3, -8/24.0)}}
dt_compare(".now"){|dc| n.times{dc.now}}
dt_compare(".ordinal"){|dc| n.times{dc.ordinal(2010, 1, 13, 43, 57)}}
dt_compare(".parse"){|dc| n.times{dc.parse('2010-12-13T13:43:57+08:00')}}
dt_compare(".strptime"){|dc| n.times{dc.strptime('fri jan 5 13:43:37 2007', '%c')}}

compare("#+"){|dc| d = dc.civil(2010, 1, 1); n.times{d + 100}}
compare("#- Integer"){|dc| d = dc.civil(2010, 1, 1); n.times{d - 100}}
compare("#- Date"){|dc| d = dc.jd(2010); d2 = dc.jd(2011); n.times{d - d2}}
compare("#- DateTime"){|dc| d = dc.jd(2010); d2 = DateTime.jd(2011); n.times{d - d2}}
compare("#<<"){|dc| d = dc.civil(2011, 7, 31); n.times{d << 17}}
compare("#>>"){|dc| d = dc.civil(2010, 1, 31); n.times{d >> 13}}
compare("#<=> Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 2); n.times{d <=> d2}}
compare("#<=> DateTime"){|dc| d = dc.civil(2010, 1, 1); d2 = DateTime.civil(2010, 1, 2, 13, 14, 15); n.times{d <=> d2}}
compare("#<=> Integer"){|dc| d = dc.civil(2010, 1, 1); d2 = 2010; n.times{d <=> d2}}
compare("#=== Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 2); n.times{d === d2}}
compare("#=== DateTime"){|dc| d = dc.civil(2010, 1, 1); d2 = DateTime.civil(2010, 1, 2, 13, 14, 15); n.times{d === d2}}
compare("#=== Integer"){|dc| d = dc.civil(2010, 1, 1); d2 = 2010; n.times{d === d2}}
compare("#ajd"){|dc| d = dc.jd(2010); n.times{d.ajd}}
compare("#amjd"){|dc| d = dc.jd(2010); n.times{d.amjd}}
compare("#asctime"){|dc| d = dc.civil(2010, 1, 1); n.times{d.asctime}}
compare("#cwday"){|dc| d = dc.civil(2010, 1, 1); n.times{d.cwday}}
compare("#cweek"){|dc| d = dc.civil(2010, 1, 1); n.times{d.cweek}}
compare("#cwyear"){|dc| d = dc.civil(2010, 1, 1); n.times{d.cwyear}}
compare("#day"){|dc| d = dc.civil(2010, 1, 1); n.times{d.day}}
compare("#day_fraction"){|dc| d = dc.civil(2010, 1, 1); n.times{d.day_fraction}}
compare("#downto"){|dc| d = dc.civil(2010, 1, 10); d2 = dc.civil(2010, 1, 1); n.times{d.downto(d2){}}}
compare("#eql?"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 1); n.times{d.eql? d2}}
compare("#gregorian"){|dc| d = dc.civil(2010, 1, 1); n.times{d.gregorian}}
compare("#gregorian?"){|dc| d = dc.civil(2010, 1, 1); n.times{d.gregorian?}}
compare("#hash"){|dc| d = dc.civil(2010, 1, 1); n.times{d.hash}}
compare("#inspect"){|dc| d = dc.civil(2010, 1, 1); n.times{d.inspect}}
compare("#jd"){|dc| d = dc.jd(2010); n.times{d.jd}}
compare("#julian?"){|dc| d = dc.civil(2010, 1, 1); n.times{d.julian?}}
compare("#ld"){|dc| d = dc.civil(2010, 1, 1); n.times{d.ld}}
compare("#leap?"){|dc| d = dc.civil(2010, 1, 1); n.times{d.leap?}}
compare("#mjd"){|dc| d = dc.civil(2010, 1, 1); n.times{d.mjd}}
compare("#month"){|dc| d = dc.civil(2010, 1, 1); n.times{d.month}}
compare("#new_start"){|dc| d = dc.civil(2010, 1, 1); n.times{d.new_start 0}}
compare("#next"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next}}
compare("#start"){|dc| d = dc.civil(2010, 1, 1); n.times{d.start}}
compare("#step"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 10); n.times{d.step(d2){}}}
compare("#strftime"){|dc| d = dc.civil(2010, 1, 10); n.times{d.strftime('%+')}}
compare("#to_s"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_s}}
compare("#upto"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 10); n.times{d.upto(d2){}}}
compare("#wday"){|dc| d = dc.civil(2010, 1, 1); n.times{d.wday}}
compare("#yday"){|dc| d = dc.civil(2010, 1, 1); n.times{d.yday}}
compare("#year"){|dc| d = dc.civil(2010, 1, 1); n.times{d.year}}

dt_compare("#+"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d + 100}}
dt_compare("#- Integer"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d - 100}}
dt_compare("#- Date"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = Date.civil(2010, 1, 2); n.times{d - d2}}
dt_compare("#- DateTime"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 6, 21, 31, 45); n.times{d - d2}}
dt_compare("#>>"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d >> 13}}
dt_compare("#<<"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d << 17}}
dt_compare("#<=> DateTime"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 2, 3, 4, 5); n.times{d <=> d2}}
dt_compare("#<=> Date"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = Date.civil(2010, 1, 2); n.times{d <=> d2}}
dt_compare("#<=> Integer"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 2).jd; n.times{d <=> d2}}
dt_compare("#=== Date"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = Date.civil(2010, 1, 2); n.times{d === d2}}
dt_compare("#=== DateTime"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 2, 13, 14, 15); n.times{d === d2}}
dt_compare("#=== Integer"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = 2010; n.times{d === d2}}
dt_compare("#ajd"){|dc| d = dc.jd(2010, 13, 43, 57); n.times{d.ajd}}
dt_compare("#amjd"){|dc| d = dc.jd(2010, 13, 43, 57); n.times{d.amjd}}
dt_compare("#asctime"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.asctime}}
dt_compare("#cwday"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.cwday}}
dt_compare("#cweek"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.cweek}}
dt_compare("#cwyear"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.cwyear}}
dt_compare("#day"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.day}}
dt_compare("#day_fraction"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.day_fraction}}
dt_compare("#downto"){|dc| d = dc.civil(2010, 1, 10, 13, 43, 57); d2 = dc.civil(2010, 1, 1, 13, 43, 56); n.times{d.downto(d2){}}}
dt_compare("#eql?"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.eql? d2}}
dt_compare("#gregorian"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.gregorian}}
dt_compare("#gregorian?"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.gregorian?}}
dt_compare("#hash"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.hash}}
dt_compare("#hour"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.hour}}
dt_compare("#inspect"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.inspect}}
dt_compare("#jd"){|dc| d = dc.jd(2010, 13, 43, 57); n.times{d.jd}}
dt_compare("#julian?"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.julian?}}
dt_compare("#ld"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.ld}}
dt_compare("#leap?"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.leap?}}
dt_compare("#min"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.min}}
dt_compare("#mjd"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.mjd}}
dt_compare("#month"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.month}}
dt_compare("#new_offset"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.new_offset 0}}
dt_compare("#new_start"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.new_start 0}}
dt_compare("#next"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.next}}
dt_compare("#offset"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.offset}}
dt_compare("#sec"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.sec}}
dt_compare("#sec_fraction"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.sec_fraction}}
dt_compare("#start"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.start}}
dt_compare("#step"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 10, 13, 43, 58); n.times{d.step(d2){}}}
dt_compare("#strftime"){|dc| d = dc.civil(2010, 1, 10, 13, 43, 57); n.times{d.strftime('%+')}}
dt_compare("#to_s"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.to_s}}
dt_compare("#upto"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 10); n.times{d.upto(d2){}}}
dt_compare("#wday"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.wday}}
dt_compare("#yday"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.yday}}
dt_compare("#year"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.year}}
dt_compare("#zone"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.zone}}

compare(".jd.to_s"){|dc|n.times{dc.jd(2010).to_s}}
compare(".jd.(year|month|day)"){|dc| n.times{d = dc.jd(2010); d.year; d.month; d.day}}
compare(".civil.jd"){|dc| n.times{dc.civil(2010, 1, 1).jd}}

dt_compare(".now.to_s"){|dc|n.times{dc.now.to_s}}
dt_compare(".now.(year|month|day|hour|min|sec)"){|dc| n.times{d = dc.now; d.year; d.month; d.day; d.hour; d.min; d.sec}}
dt_compare(".civil.jd"){|dc| n.times{dc.civil(2010, 1, 1, 13, 43, 57).jd}}

compare(" Marshal.dump"){|dc| d = dc.civil(2010, 1, 1); n.times{Marshal.dump(d)}}
compare(" Marshal.load"){|dc| d = Marshal.dump(dc.civil(2010, 1, 1)); n.times{Marshal.load(d)}}

dt_compare(" Marshal.dump"){|dc| d = dc.now; n.times{Marshal.dump(d)}}
dt_compare(" Marshal.load"){|dc| d = Marshal.dump(dc.now); n.times{Marshal.load(d)}}

if RUBY_VERSION >= '1.9.0.'
  compare("._httpdate"){|dc| n.times{dc._httpdate("Thu, 12 Aug 2010 00:00:00 GMT")}}
  compare("._iso8601"){|dc| n.times{dc._iso8601("2010-08-12")}}
  compare("._jisx0301"){|dc| n.times{dc._jisx0301("H22.08.12")}}
  compare("._rfc2822"){|dc| n.times{dc._rfc2822("Thu, 12 Aug 2010 00:00:00 +0000")}}
  compare("._rfc3339"){|dc| n.times{dc._rfc3339("2010-08-12T08:27:18-07:00")}}
  compare("._xmlschema"){|dc| n.times{dc._xmlschema("2010-08-12")}}
  compare(".httpdate"){|dc| n.times{dc.httpdate("Thu, 12 Aug 2010 00:00:00 GMT")}}
  compare(".iso8601"){|dc| n.times{dc.iso8601("2010-08-12")}}
  compare(".jisx0301"){|dc| n.times{dc.jisx0301("H22.08.12")}}
  compare(".rfc2822"){|dc| n.times{dc.rfc2822("Thu, 12 Aug 2010 00:00:00 +0000")}}
  compare(".rfc3339"){|dc| n.times{dc.rfc3339("2010-08-12T08:27:18-07:00")}}
  compare(".xmlschema"){|dc| n.times{dc.xmlschema("2010-08-12")}}

  dt_compare("._httpdate"){|dc| n.times{dc._httpdate("Thu, 12 Aug 2010 15:27:12 GMT")}}
  dt_compare("._iso8601"){|dc| n.times{dc._iso8601("2010-08-12T08:27:18-07:00")}}
  dt_compare("._jisx0301"){|dc| n.times{dc._jisx0301("H22.08.12T08:27:23-07:00")}}
  dt_compare("._rfc2822"){|dc| n.times{dc._rfc2822("Thu, 12 Aug 2010 08:27:31 -0700")}}
  dt_compare("._rfc3339"){|dc| n.times{dc._rfc3339("2010-08-12T08:27:18-07:00")}}
  dt_compare("._xmlschema"){|dc| n.times{dc._xmlschema("2010-08-12T08:27:18-07:00")}}
  dt_compare(".httpdate"){|dc| n.times{dc.httpdate("Thu, 12 Aug 2010 15:27:12 GMT")}}
  dt_compare(".iso8601"){|dc| n.times{dc.iso8601("2010-08-12T08:27:18-07:00")}}
  dt_compare(".jisx0301"){|dc| n.times{dc.jisx0301("H22.08.12T08:27:23-07:00")}}
  dt_compare(".rfc2822"){|dc| n.times{dc.rfc2822("Thu, 12 Aug 2010 08:27:31 -0700")}}
  dt_compare(".rfc3339"){|dc| n.times{dc.rfc3339("2010-08-12T08:27:18-07:00")}}
  dt_compare(".xmlschema"){|dc| n.times{dc.xmlschema("2010-08-12T08:27:18-07:00")}}

  compare("#httpdate"){|dc| d = dc.civil(2010, 1, 1); n.times{d.httpdate}}
  compare("#iso8601"){|dc| d = dc.civil(2010, 1, 1); n.times{d.iso8601}}
  compare("#jisx0301"){|dc| d = dc.civil(2010, 1, 1); n.times{d.jisx0301}}
  compare("#next_day"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next_day}}
  compare("#next_month"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next_month}}
  compare("#next_year"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next_year}}
  compare("#prev_day"){|dc| d = dc.civil(2010, 1, 1); n.times{d.prev_day}}
  compare("#prev_month"){|dc| d = dc.civil(2010, 1, 1); n.times{d.prev_month}}
  compare("#prev_year"){|dc| d = dc.civil(2010, 1, 1); n.times{d.prev_year}}
  compare("#rfc2822"){|dc| d = dc.civil(2010, 1, 1); n.times{d.rfc2822}}
  compare("#rfc3339"){|dc| d = dc.civil(2010, 1, 1); n.times{d.rfc3339}}
  compare("#sunday?"){|dc| d = dc.civil(2010, 1, 1); n.times{d.sunday?}}
  compare("#to_date"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_date}}
  compare("#to_datetime"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_datetime}}
  compare("#to_time"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_time}}
  compare("#xmlschema"){|dc| d = dc.civil(2010, 1, 1); n.times{d.xmlschema}}

  dt_compare("#httpdate"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.httpdate}}
  dt_compare("#iso8601"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.iso8601}}
  dt_compare("#jisx0301"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.jisx0301}}
  dt_compare("#next_day"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.next_day}}
  dt_compare("#next_month"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.next_month}}
  dt_compare("#next_year"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.next_year}}
  dt_compare("#prev_day"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.prev_day}}
  dt_compare("#prev_month"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.prev_month}}
  dt_compare("#prev_year"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.prev_year}}
  dt_compare("#rfc2822"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.rfc2822}}
  dt_compare("#rfc3339"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.rfc3339}}
  dt_compare("#sunday?"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.sunday?}}
  dt_compare("#to_date"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.to_date}}
  dt_compare("#to_datetime"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.to_datetime}}
  dt_compare("#to_time"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.to_time}}
  dt_compare("#xmlschema"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.xmlschema}}

  compare(" Time#to_time"){|dc| t = Time.now; n.times{t.to_time}}
  compare(" Time#to_date"){|dc| t = Time.now; n.times{t.to_date}}
  dt_compare(" Time#to_datetime"){|dc| t = Time.now; n.times{t.to_datetime}}
else
  compare(".ajd_to_amjd"){|dc| n.times{dc.ajd_to_amjd(1)}}
  compare(".ajd_to_jd"){|dc| n.times{dc.ajd_to_jd(1)}}
  compare(".amjd_to_ajd"){|dc| n.times{dc.amjd_to_ajd(1)}}
  compare(".civil_to_jd"){|dc| n.times{dc.civil_to_jd(1, 1, 1)}}
  compare(".commercial_to_jd"){|dc| n.times{dc.commercial_to_jd(2010, 1, 1)}}
  compare(".day_fraction_to_time"){|dc| n.times{dc.day_fraction_to_time(0.5)}}
  compare(".gregorian? Date"){|dc| d = Date.jd(1); n.times{dc.gregorian?(d, 1)}}
  compare(".gregorian? Integer"){|dc| n.times{dc.gregorian?(1, 1)}}
  compare(".jd_to_ajd"){|dc| n.times{dc.jd_to_ajd(2000, 0)}}
  compare(".jd_to_civil"){|dc| n.times{dc.jd_to_civil(2010)}}
  compare(".jd_to_commercial"){|dc| n.times{dc.jd_to_commercial(2010)}}
  compare(".jd_to_ld"){|dc| n.times{dc.jd_to_ld(2010)}}
  compare(".jd_to_mjd"){|dc| n.times{dc.jd_to_mjd(2010)}}
  compare(".jd_to_ordinal"){|dc| n.times{dc.jd_to_ordinal(2010)}}
  compare(".jd_to_wday"){|dc| n.times{dc.jd_to_wday(2010)}}
  compare(".julian? Date"){|dc| d = Date.jd(1); n.times{dc.julian?(d, 1)}}
  compare(".julian? Integer"){|dc| n.times{dc.julian?(1, 1)}}
  compare(".ld_to_jd"){|dc| n.times{dc.ld_to_jd(2010)}}
  compare(".mjd_to_jd"){|dc| n.times{dc.mjd_to_jd(2010)}}
  compare(".ordinal_to_jd"){|dc| n.times{dc.ordinal_to_jd(2012, 100)}}
  compare(".time_to_day_fraction"){|dc| n.times{dc.time_to_day_fraction(7, 13, 17)}}
  compare(".valid_time?"){|dc| n.times{dc.valid_time?(13, 35, 52)}}
  compare(".zone_to_diff"){|dc| s = "+0800"; n.times{dc.zone_to_diff(s)}}
end

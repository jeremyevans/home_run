require 'date'
require 'benchmark'
SD = Date
SDT = DateTime
$:.unshift(File.join(File.dirname(File.dirname(File.expand_path(__FILE__))), 'ext'))
Object.send(:remove_const, :Date)
Object.send(:remove_const, :DateTime)
require 'ext/date.so'
HRD = Date
HRDT = DateTime

def compare(label, &block)
  Object.send(:remove_const, :Date)
  Object.send(:const_set, :Date, SD)
  t = Time.now
  yield SD
  stdlib = Time.now - t

  Object.send(:remove_const, :Date)
  Object.send(:const_set, :Date, HRD)
  t = Time.now
  yield HRD
  home_run = Time.now - t
  
  puts sprintf('Date%s,%0.5f,%0.5f,%0.2f', label, stdlib, home_run, stdlib/home_run)
end

def dt_compare(label, &block)
  Object.send(:remove_const, :Date)
  Object.send(:remove_const, :DateTime)
  Object.send(:const_set, :Date, SD)
  Object.send(:const_set, :DateTime, SDT)
  t = Time.now
  yield SDT
  stdlib = Time.now - t

  Object.send(:remove_const, :Date)
  Object.send(:remove_const, :DateTime)
  Object.send(:const_set, :Date, HRD)
  Object.send(:const_set, :DateTime, HRDT)
  t = Time.now
  yield HRDT
  home_run = Time.now - t
  
  puts sprintf('DateTime%s,%0.5f,%0.5f,%0.2f', label, stdlib, home_run, stdlib/home_run)
end

n = (ARGV.first || 100000).to_i
puts "label,stdlib,home_run,times faster"
dt_compare(".civil"){|dc| n.times{dc.civil(2010, 1, 1, 13, 43, 57)}}
dt_compare(".commercial"){|dc| n.times{dc.commercial(2010, 1, 1, 13, 43, 57)}}
dt_compare(".jd"){|dc| n.times{dc.jd(2010, 13, 43, 57)}}
dt_compare(".new!"){|dc| n.times{dc.new!(201013.3, -8/24.0)}}
dt_compare(".now"){|dc| n.times{dc.now}}
dt_compare(".ordinal"){|dc| n.times{dc.ordinal(2010, 1, 13, 43, 57)}}
dt_compare("#inspect"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.inspect}}
dt_compare("#to_s"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d.to_s}}
dt_compare("#<=> DateTime"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 2, 3, 4, 5); n.times{d <=> d2}}
dt_compare("#<=> Date"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = Date.civil(2010, 1, 2); n.times{d <=> d2}}
dt_compare("#<=> Integer"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 2).jd; n.times{d <=> d2}}
dt_compare("#=== Date"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = Date.civil(2010, 1, 2); n.times{d === d2}}
dt_compare("#=== DateTime"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 2, 13, 14, 15); n.times{d === d2}}
dt_compare("#=== Integer"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = 2010; n.times{d === d2}}
dt_compare("#- Integer"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d - 100}}
dt_compare("#- Date"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = Date.civil(2010, 1, 2); n.times{d - d2}}
dt_compare("#- DateTime"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); d2 = dc.civil(2010, 1, 6, 21, 31, 45); n.times{d - d2}}
dt_compare("#>>"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d >> 13}}
dt_compare("#<<"){|dc| d = dc.civil(2010, 1, 1, 13, 43, 57); n.times{d << 17}}

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
compare(".strptime'"){|dc| n.times{dc.strptime('fri jan 5 00:00:00 2007', '%c')}}
compare(".today"){|dc| n.times{dc.today}}
compare(".valid_date?"){|dc| n.times{dc.valid_date?(2010, 1, 1)}}

compare("#inspect"){|dc| d = dc.civil(2010, 1, 1); n.times{d.inspect}}
compare("#to_s"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_s}}
compare("#asctime"){|dc| d = dc.civil(2010, 1, 1); n.times{d.asctime}}
compare("#jd"){|dc| d = dc.jd(2010); n.times{d.jd}}
compare("#eql?"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 1); n.times{d.eql? d2}}
compare("#<=> Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 2); n.times{d <=> d2}}
compare("#<=> DateTime"){|dc| d = dc.civil(2010, 1, 1); d2 = DateTime.civil(2010, 1, 2, 13, 14, 15); n.times{d <=> d2}}
compare("#<=> Integer"){|dc| d = dc.civil(2010, 1, 1); d2 = 2010; n.times{d <=> d2}}
compare("#=== Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 2); n.times{d === d2}}
compare("#=== DateTime"){|dc| d = dc.civil(2010, 1, 1); d2 = DateTime.civil(2010, 1, 2, 13, 14, 15); n.times{d === d2}}
compare("#=== Integer"){|dc| d = dc.civil(2010, 1, 1); d2 = 2010; n.times{d === d2}}
compare("#+"){|dc| d = dc.civil(2010, 1, 1); n.times{d + 100}}
compare("#- Integer"){|dc| d = dc.civil(2010, 1, 1); n.times{d - 100}}
compare("#- Date"){|dc| d = dc.jd(2010); d2 = dc.jd(2011); n.times{d - d2}}
compare("#- DateTime"){|dc| d = dc.jd(2010); d2 = DateTime.jd(2011); n.times{d - d2}}
compare("#>>"){|dc| d = dc.civil(2010, 1, 31); n.times{d >> 13}}
compare("#<<"){|dc| d = dc.civil(2011, 7, 31); n.times{d << 17}}
compare("#next"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next}}
compare("#yday"){|dc| d = dc.civil(2010, 1, 1); n.times{d.yday}}
compare("#wday"){|dc| d = dc.civil(2010, 1, 1); n.times{d.wday}}
compare("#leap?"){|dc| d = dc.civil(2010, 1, 1); n.times{d.leap?}}
compare("#day_fraction"){|dc| d = dc.civil(2010, 1, 1); n.times{d.day_fraction}}
compare("#hash"){|dc| d = dc.civil(2010, 1, 1); n.times{d.hash}}
compare("#mjd"){|dc| d = dc.civil(2010, 1, 1); n.times{d.mjd}}
compare("#ld"){|dc| d = dc.civil(2010, 1, 1); n.times{d.ld}}
compare("#cwyear"){|dc| d = dc.civil(2010, 1, 1); n.times{d.cwyear}}
compare("#cweek"){|dc| d = dc.civil(2010, 1, 1); n.times{d.cweek}}
compare("#cwday"){|dc| d = dc.civil(2010, 1, 1); n.times{d.cwday}}
compare("#step"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 10); n.times{d.step(d2){}}}
compare("#downto"){|dc| d = dc.civil(2010, 1, 10); d2 = dc.civil(2010, 1, 1); n.times{d.downto(d2){}}}
compare("#upto"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 10); n.times{d.upto(d2){}}}
compare("#strftime"){|dc| d = dc.civil(2010, 1, 10); n.times{d.strftime('%+')}}

compare(".jd.inspect"){|dc|n.times{dc.jd(2010).inspect}}
compare(".jd.(year|month|day)"){|dc| n.times{d = dc.jd(2010); d.year; d.month; d.day}}
compare(".civil.jd"){|dc| n.times{dc.civil(2010, 1, 1).jd}}

compare("Marshal.dump Date"){|dc| d = dc.civil(2010, 1, 1); n.times{Marshal.dump(d)}}
compare("Marshal.load Date"){|dc| d = Marshal.dump(dc.civil(2010, 1, 1)); n.times{Marshal.load(d)}}

if RUBY_VERSION >= '1.9.0.'
  compare("#sunday?"){|dc| d = dc.civil(2010, 1, 1); n.times{d.sunday?}}
  compare("#to_date"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_date}}
  compare("#to_time"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_time}}
  compare("#next_day"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next_day}}
  compare("#prev_day"){|dc| d = dc.civil(2010, 1, 1); n.times{d.prev_day}}
  compare("#next_month"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next_month}}
  compare("#prev_month"){|dc| d = dc.civil(2010, 1, 1); n.times{d.prev_month}}
  compare("#next_year"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next_year}}
  compare("#prev_year"){|dc| d = dc.civil(2010, 1, 1); n.times{d.prev_year}}
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
  compare(".valid_commercial?"){|dc| n.times{dc.valid_commercial?(2010, 1, 1)}}
  compare(".valid_jd?"){|dc| n.times{dc.valid_jd?(2010)}}
  compare(".valid_ordinal?"){|dc| n.times{dc.valid_jd?(2010, 100)}}
  compare(".valid_time?"){|dc| n.times{dc.valid_time?(13, 35, 52)}}
end



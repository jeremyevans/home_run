require 'date'
require 'benchmark'
SD = Date
$:.unshift(File.join(File.dirname(File.dirname(File.expand_path(__FILE__))), 'ext'))
load 'lib/date.rb'
HRD = Date

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
  
  puts sprintf('%s,%0.3f,%0.3f,%0.3f', label, stdlib, home_run, stdlib/home_run)
end

n = 100000
puts "label,stdlib,home_run,times faster"
compare(".civil"){|dc| n.times{dc.civil(2010, 1, 1)}}
compare(".jd"){|dc| n.times{dc.jd(2010)}}
compare(".today"){|dc| n.times{dc.today}}
compare(".ajd_to_amjd"){|dc| n.times{dc.ajd_to_amjd(1)}}
compare(".ajd_to_jd"){|dc| n.times{dc.ajd_to_jd(1)}}
compare(".amjd_to_ajd"){|dc| n.times{dc.amjd_to_ajd(1)}}

compare("#inspect"){|dc| d = dc.civil(2010, 1, 1); n.times{d.inspect}}
compare("#to_s"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_s}}
compare("#asctime"){|dc| d = dc.civil(2010, 1, 1); n.times{d.asctime}}
compare("#jd"){|dc| d = dc.jd(2010); n.times{d.jd}}
compare("#eql?"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 1); n.times{d.eql? d2}}
compare("#<=> Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 2); n.times{d <=> d2}}
compare("#<=> Integer"){|dc| d = dc.civil(2010, 1, 1); d2 = 2010; n.times{d <=> d2}}
compare("#=== Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 2); n.times{d === d2}}
compare("#=== Integer"){|dc| d = dc.civil(2010, 1, 1); d2 = 2010; n.times{d === d2}}
compare("#+"){|dc| d = dc.civil(2010, 1, 1); n.times{d + 100}}
compare("#- Integer"){|dc| d = dc.civil(2010, 1, 1); n.times{d - 100}}
compare("#- Date"){|dc| d = dc.jd(2010); d2 = dc.jd(2011); n.times{d - d2}}
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

compare(".jd.inspect"){|dc|n.times{dc.jd(2010).inspect}}
compare(".jd.(year|month|day)"){|dc| n.times{d = dc.jd(2010); d.year; d.month; d.day}}
compare(".civil.jd"){|dc| n.times{dc.civil(2010, 1, 1).jd}}
compare("Marshal.dump"){|dc| d = dc.civil(2010, 1, 1); n.times{Marshal.dump(d)}}
compare("Marshal.load"){|dc| d = Marshal.dump(dc.civil(2010, 1, 1)); n.times{Marshal.load(d)}}



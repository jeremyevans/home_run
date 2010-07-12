require 'date'
require 'benchmark'
D = Date
$:.unshift(File.join(File.dirname(File.dirname(File.expand_path(__FILE__))), 'ext'))
load 'lib/date.rb'

def compare(label, &block)
  t = Time.now
  yield D
  stdlib = Time.now - t

  t = Time.now
  yield Date
  home_run = Time.now - t
  
  puts sprintf('%s,%0.3f,%0.3f,%0.3f', label, stdlib, home_run, stdlib/home_run)
end

n = 100000
puts "label,stdlib,home_run,times faster"
compare(".civil"){|dc| n.times{dc.civil(2010, 1, 1)}}
compare(".jd"){|dc| n.times{dc.jd(2010)}}
compare(".today"){|dc| n.times{dc.today}}

compare("#inspect"){|dc| d = dc.civil(2010, 1, 1); n.times{d.inspect}}
compare("#to_s"){|dc| d = dc.civil(2010, 1, 1); n.times{d.to_s}}
compare("#jd"){|dc| d = dc.jd(2010); n.times{d.jd}}
compare("#eql? Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 1); n.times{d.eql? d2}}
compare("#<=> Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 2); n.times{d <=> d2}}
compare("#<=> Integer"){|dc| d = dc.civil(2010, 1, 1); d2 = 2010; n.times{d <=> d2}}
compare("#=== Date"){|dc| d = dc.civil(2010, 1, 1); d2 = dc.civil(2010, 1, 2); n.times{d === d2}}
compare("#=== Integer"){|dc| d = dc.civil(2010, 1, 1); d2 = 2010; n.times{d === d2}}
compare("#+ 100"){|dc| d = dc.civil(2010, 1, 1); n.times{d + 100}}
compare("#- 100"){|dc| d = dc.civil(2010, 1, 1); n.times{d - 100}}
#compare("#- dc"){|dc| d = dc.jd(2010); d2 = dc.jd(2011); n.times{d - d2}}
compare("#>> 13"){|dc| d = dc.civil(2010, 1, 31); n.times{d >> 13}}
compare("#<< 17"){|dc| d = dc.civil(2011, 7, 31); n.times{d << 17}}
compare("#next"){|dc| d = dc.civil(2010, 1, 1); n.times{d.next}}
compare("#yday"){|dc| d = dc.civil(2010, 1, 1); n.times{d.yday}}
compare("#wday"){|dc| d = dc.civil(2010, 1, 1); n.times{d.wday}}
compare("#leap?"){|dc| d = dc.civil(2010, 1, 1); n.times{d.leap?}}
compare("#day_fraction"){|dc| d = dc.civil(2010, 1, 1); n.times{d.day_fraction}}
compare("#hash"){|dc| d = dc.civil(2010, 1, 1); n.times{d.hash}}
compare("#mjd"){|dc| d = dc.civil(2010, 1, 1); n.times{d.mjd}}

compare(".jd.inspect"){|dc|n.times{dc.jd(2010).inspect}}
compare(".jd.(year|month|day)"){|dc| n.times{d = dc.jd(2010); d.year; d.month; d.day}}
compare(".civil.jd"){|dc| n.times{dc.civil(2010, 1, 1).jd}}


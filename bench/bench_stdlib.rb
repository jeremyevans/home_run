require 'date'
require 'benchmark'

n = 100000
Benchmark.bm(25) do |x|
  x.report(".civil(y,m,d)"){n.times{Date.civil(2010, 1, 1)}}
  x.report(".jd(jd)"){n.times{Date.jd(2010)}}
  x.report(".today"){n.times{Date.today}}

  x.report("#inspect"){d = Date.civil(2010, 1, 1); n.times{d.inspect}}
  x.report("#jd"){d = Date.jd(2010); n.times{d.jd}}
  x.report("#<=> (civil,civil)"){d = Date.civil(2010, 1, 1); d2 = Date.civil(2010, 1, 2); n.times{d <=> d2}}
  x.report("#<=> (jd,civil)"){d = Date.civil(2010, 1, 1); d2 = Date.jd(2010); n.times{d <=> d2}}
  x.report("#<=> (jd,jd)"){d = Date.jd(2010); d2 = Date.jd(2011); n.times{d <=> d2}}
  x.report("#<=> (civil,int)"){d = Date.civil(2010, 1, 1); d2 = 2010; n.times{d <=> d2}}
  x.report("#<=> (jd,int)"){d = Date.jd(2010); d2 = 2011; n.times{d <=> d2}}
  x.report("#+ 100"){d = Date.civil(2010, 1, 1); n.times{d + 100}}
  x.report("#- 100"){d = Date.civil(2010, 1, 1); n.times{d - 100}}
  x.report("#- Date"){d = Date.jd(2010); d2 = Date.jd(2011); n.times{d - d2}}
  x.report("#>> 13"){d = Date.civil(2010, 1, 31); n.times{d >> 13}}

  x.report(".jd(jd).inspect"){n.times{Date.jd(2010).inspect}}
  x.report(".jd(jd).{year,month,day}"){n.times{d = Date.jd(2010); d.year; d.month; d.day}}
  x.report(".civil(y,m,d).jd"){n.times{Date.civil(2010, 1, 1).jd}}
end


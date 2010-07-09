require 'date'
require 'benchmark'

n = 100000
Benchmark.bm(25) do |x|
  x.report(".civil(y,m,d)"){n.times{Date.civil(2010, 1, 1)}}
  x.report(".jd(jd)"){n.times{Date.jd(2010)}}
  x.report(".today"){n.times{Date.today}}

  x.report("#inspect"){d = Date.civil(2010, 1, 1); n.times{d.inspect}}
  x.report("#jd"){d = Date.jd(2010); n.times{d.jd}}

  x.report(".jd(jd).inspect"){n.times{Date.jd(2010).inspect}}
  x.report(".jd(jd).{year,month,day}"){n.times{d = Date.jd(2010); d.year; d.month; d.day}}
  x.report(".civil(y,m,d).jd"){n.times{Date.civil(2010, 1, 1).jd}}
end


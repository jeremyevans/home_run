require 'date'
require 'benchmark'

n = 100000
Benchmark.bm(20) do |x|
  x.report(".civil(y,m,d)"){n.times{Date.civil(2010, 1, 1)}}
  x.report(".jd(jd)"){n.times{Date.jd(2010)}}
  x.report("#inspect"){d = Date.civil(2010, 1, 1); n.times{d.inspect}}
  x.report(".jd(jd).inspect"){n.times{Date.jd(2010).inspect}}
end


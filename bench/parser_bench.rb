load(File.join(File.dirname(File.expand_path(__FILE__)), 'cpu_bench_util.rb'))
n = N
formats = <<END
2010-12-13
2010-12-13 10:20:30
2010-12-13 10:20:30-07:00
2009/09/23
2009/09/23 06:38:06
23/Aug/2010
23/Aug/2010:14:31:32
23/Aug/2010:14:31:32 -0700
Mon, 23 Aug 2010
Mon, 23 Aug 2010 11:13:31
Mon, 23 Aug 2010 11:13:31 -0700
Mon, 23 Aug 2010 21:43:53 GMT
END
formats = formats.split("\n").map{|l| l.chomp}
formats.each do |f|
  compare("._parse #{f.inspect}"){|dc| n.times{dc._parse(f)}}
end

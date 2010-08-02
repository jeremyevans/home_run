require "rake"
require "rake/clean"

CLEAN.include %w'Makefile ext/date.*o **/*.rbc *.core' 
RUBY=ENV['RUBY'] || 'ruby'
IRB=ENV['IRB'] || 'irb'
MSPEC=ENV['MSPEC'] || 'mspec'

desc "Run the rubyspecs with mspec"
task :default => :spec
task :spec do
  sh %{#{MSPEC} -I ext rubyspec/library/date/*_spec.rb}
end

desc "Build the ragel parser"
task :parser do
  require 'erb'
  File.open('ext/date_parser.rl', 'wb'){|f| f.write(ERB.new(File.read('ext/date_parser.rl.erb')).result(binding))}
  sh %{cd ext && ragel date_parser.rl}
end

desc "Build the extension"
task :build=>[:clean] do
  sh %{cd ext && #{RUBY} extconf.rb && make}
end

desc "Build debug version of extension"
task :build_debug=>[:clean] do
  ENV['DEBUG'] = '1'
  sh %{cd ext && #{RUBY} extconf.rb && make}
end

desc "Start an IRB shell using the extension"
task :irb do
  sh %{#{IRB} -I ext -r date}
end

desc "Run comparative benchmarks"
task :bench do
  sh %{#{RUBY} bench/cpu_bench.rb}
end

desc "Run quick comparative benchmarks"
task :bench_quick do
  sh %{#{RUBY} bench/cpu_bench.rb 1000}
end

desc "Run memory benchmarks"
task :mem_bench do
  stdlib = `#{RUBY} bench/mem_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/mem_bench.rb`.to_i
  puts "stdlib Date: #{stdlib}KB"
  puts "home_run Date: #{home_run}KB"
  puts "home_run Date uses #{sprintf('%0.1f', stdlib/home_run.to_f)} times less memory"

  stdlib = `#{RUBY} bench/dt_mem_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/dt_mem_bench.rb`.to_i
  puts "stdlib DateTime: #{stdlib}KB"
  puts "home_run DateTime: #{home_run}KB"
  puts "home_run DateTime uses #{sprintf('%0.1f', stdlib/home_run.to_f)} times less memory"
end

desc "Run garbage creation benchmarks"
task :garbage_bench do
  stdlib = `#{RUBY} bench/garbage_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/garbage_bench.rb`.to_i
  puts "stdlib Date: #{stdlib}KB"
  puts "home_run Date: #{home_run}KB"
  puts "home_run Date creates #{sprintf('%0.1f', stdlib/home_run.to_f)} times less garbage"

  stdlib = `#{RUBY} bench/dt_garbage_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/dt_garbage_bench.rb`.to_i
  puts "stdlib DateTime: #{stdlib}KB"
  puts "home_run DateTime: #{home_run}KB"
  puts "home_run DateTime creates #{sprintf('%0.1f', stdlib/home_run.to_f)} times less garbage"
end

def date_methods
  [`#{RUBY} -r date -e 'puts class << Date; instance_methods(false); end'`.split,
  `#{RUBY} -I ext -r date -e 'puts class << Date; instance_methods(false); end'`.split,
  `#{RUBY} -r date -e 'puts Date.instance_methods(false)'`.split,
  `#{RUBY} -I ext -r date -e 'puts Date.instance_methods(false)'`.split]
end

def datetime_methods
  [`#{RUBY} -r date -e 'puts DateTime.methods - Class.methods'`.split,
  `#{RUBY} -I ext -r date -e 'puts class << DateTime; instance_methods(false); end - Class.methods'`.split,
  `#{RUBY} -r date -e 'puts DateTime.instance_methods - Object.instance_methods'`.split,
  `#{RUBY} -I ext -r date -e 'puts DateTime.instance_methods(true) - Object.instance_methods'`.split]
end

desc "Print all methods that still need to be implemented"
task :todo do
  scm, hrcm, sim, hrim = date_methods
  puts "Date Class Methods: #{(scm-hrcm).sort.join(', ')}"
  puts "Date Instance Methods: #{(sim-hrim).sort.join(', ')}"
  puts ""
  scm, hrcm, sim, hrim = datetime_methods
  puts "DateTime Class Methods: #{(scm-hrcm).sort.join(', ')}"
  puts "DateTime Instance Methods: #{(sim-hrim).sort.join(', ')}"
end

desc "Print methods that are implemented but shouldn't be"
task :toofar do
  scm, hrcm, sim, hrim = date_methods
  puts "Date Class Methods: #{(hrcm-scm).sort.join(', ')}"
  puts "Date Instance Methods: #{(hrim-sim).sort.join(', ')}"
  puts ""
  scm, hrcm, sim, hrim = datetime_methods
  puts "DateTime Class Methods: #{(hrcm-scm).sort.join(', ')}"
  puts "DateTime Instance Methods: #{(hrim-sim).sort.join(', ')}"
end


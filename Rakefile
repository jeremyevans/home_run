require "rake"
require "rake/clean"

CLEAN.include %w'Makefile ext/home_run_date.o ext/home_run_date.so **/*.rbc' 
RUBY=ENV['RUBY'] || 'ruby'
IRB=ENV['IRB'] || 'irb'

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
  sh %{#{IRB} -I ext -I lib -r date}
end

desc "Run comparative benchmarks"
task :bench do
  sh %{#{RUBY} bench/cpu_bench.rb}
end

desc "Print all methods that still need to be implemented"
task :todo do
  scm = `#{RUBY} -r date -e 'puts class << Date; instance_methods(true); end'`.split
  hrcm = `#{RUBY} -I ext -I lib -r date -e 'puts class << Date; instance_methods(true); end'`.split
  sim = `#{RUBY} -r date -e 'puts Date.instance_methods(true)'`.split
  hrim = `#{RUBY} -I ext -I lib -r date -e 'puts Date.instance_methods(true)'`.split
  puts "Class Methods: #{(scm-hrcm).sort.join(', ')}"
  puts ""
  puts "Instance Methods: #{(sim-hrim).sort.join(', ')}"
end


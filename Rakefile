require "rake"
require "rake/clean"

CLEAN.include %w'Makefile ext/home_run_date.o ext/home_run_date.so' 
RUBY=ENV['RUBY'] || 'ruby'
IRB=ENV['IRB'] || 'irb'

desc "Build the extension"
task :build=>[:clean] do
  sh %{cd ext && #{RUBY} extconf.rb && make}
end

desc "Start an IRB shell using the extension"
task :irb do
  sh %{#{IRB} -I ext -I lib -r date}
end

desc "Run comparative benchmarks"
task :bench=>[:bench_hr] do
  sh %{#{RUBY} bench/bench_stdlib.rb}
end

desc "Benchmark this implementation"
task :bench_hr do
  sh %{#{RUBY} bench/bench_home_run.rb}
end


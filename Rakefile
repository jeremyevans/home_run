require "rake"
require "rake/clean"

CLEAN.include %w'Makefile ext/home_run_date.o ext/home_run_date.so' 
RUBY=ENV['RUBY'] || 'ruby'

desc "Build the extension"
task :build=>[:clean] do
  sh %{cd ext && #{RUBY} extconf.rb && make}
end

desc "Run comparative benchmarks"
task :bench do
  sh %{#{RUBY} bench/bench_stdlib.rb}
  sh %{#{RUBY} bench/bench_home_run.rb}
end


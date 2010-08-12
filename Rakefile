require "rake"
require "rake/clean"
begin
  require "hanna/rdoctask"
rescue LoadError
  gem 'rdoc'
  require 'rdoc/rdoc'
  require "rake/rdoctask"
end

CLEAN.include %w'Makefile ext/date.*o **/*.rbc *.core' 
RUBY=ENV['RUBY'] || 'ruby'
IRBPROG=ENV['IRB'] || 'irb'
MSPEC=ENV['MSPEC'] || 'mspec'

Rake::RDocTask.new do |rdoc|
  rdoc.rdoc_dir = "rdoc"
  rdoc.options += ["--quiet", "--line-numbers", "--inline-source", '--title',
    'home_run: Fast Date/DateTime classes for ruby', '--main', 'README.rdoc']
  rdoc.rdoc_files.add %w"README.rdoc CHANGELOG LICENSE ext/**/*.rb ext/*.c"
end

desc "Run the specs with mspec"
task :default => :spec
task :spec do
  sh %{#{MSPEC} -I ext -rdate spec/*/*_spec.rb}
end

desc "Build the gem"
task :gem => [:clean, :parser] do
  sh %{gem build home_run.gemspec}
end

desc "Build the ragel parser"
task :parser do
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
  sh %{#{IRBPROG} -I ext -r date}
end

desc "Run comparative benchmarks"
task :bench do
  sh %{#{RUBY} bench/cpu_bench.rb}
end

desc "Run quick comparative benchmarks"
task :bench_quick do
  sh %{#{RUBY} bench/cpu_bench.rb 1000}
end

desc "Run long comparative benchmarks"
task :bench_long do
  sh %{#{RUBY} bench/cpu_bench.rb 100000}
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

desc "Print all methods that still need to be implemented"
task :todo do
  scm = `#{RUBY} -r date -e 'puts Date.singleton_methods(false)'`.split
  hrcm = `#{RUBY} -I ext -r date -e 'puts Date.singleton_methods(false)'`.split
  sim = `#{RUBY} -r date -e 'puts Date.instance_methods(false)'`.split
  hrim = `#{RUBY} -I ext -r date -e 'puts Date.instance_methods(false)'`.split
  puts "Date Class Methods: #{(scm-hrcm).sort.join(', ')}"
  puts "Date Instance Methods: #{(sim-hrim).sort.join(', ')}"
  puts ""
  scm = `#{RUBY} -r date -e 'puts (DateTime.singleton_methods(false) + Date.singleton_methods(false)).uniq'`.split
  hrcm = `#{RUBY} -I ext -r date -e 'puts DateTime.singleton_methods(false)'`.split
  sim = `#{RUBY} -r date -e 'puts (DateTime.instance_methods(false) + Date.instance_methods(false)).uniq'`.split
  hrim = `#{RUBY} -I ext -r date -e 'puts DateTime.instance_methods(false)'`.split
  puts "DateTime Class Methods: #{(scm-hrcm).sort.reject{|m| m.to_s =~ /_(parse|httpdate|iso8601|jisx0301|rfc(2?822|3339)|xmlschema)|zone_to_diff|to_jd|jd_to|day_fraction|valid_|exist|leap|julian|gregorian|today|\A[no]s\?\z/}.join(', ')}"
  puts "DateTime Instance Methods: #{(sim-hrim).sort.reject{|m| m.to_s =~ /gregorian|italy|julian|england|start|sg|\A[on]s\?/}.join(', ')}"
end

desc "Print methods that are implemented but shouldn't be"
task :toofar do
  scm = `#{RUBY} -r date -e 'puts Date.methods'`.split
  hrcm = `#{RUBY} -I ext -r date -e 'puts Date.methods'`.split
  sim = `#{RUBY} -r date -e 'puts Date.instance_methods'`.split
  hrim = `#{RUBY} -I ext -r date -e 'puts Date.instance_methods'`.split
  puts "Date Class Methods: #{(hrcm-scm).sort.join(', ')}"
  puts "Date Instance Methods: #{(hrim-sim).sort.join(', ')}"
  puts ""
  scm = `#{RUBY} -r date -e 'puts DateTime.methods'`.split
  hrcm = `#{RUBY} -I ext -r date -e 'puts DateTime.methods'`.split
  sim = `#{RUBY} -r date -e 'puts DateTime.instance_methods'`.split
  hrim = `#{RUBY} -I ext -r date -e 'puts DateTime.instance_methods'`.split
  puts "DateTime Class Methods: #{(hrcm-scm).sort.join(', ')}"
  puts "DateTime Instance Methods: #{(hrim-sim).sort.join(', ')}"
end


require "rake"
require "rake/clean"

CLEAN.include %w'Makefile ext/date.*o **/*.rbc *.core' 
RUBY=ENV['RUBY'] || 'ruby'
IRBPROG=ENV['IRB'] || 'irb'
MSPEC=ENV['MSPEC'] || 'mspec'

begin
  gem 'rdoc'
  require 'rdoc/rdoc'
  require "rake/rdoctask"
  Rake::RDocTask.new do |rdoc|
    rdoc.rdoc_dir = "rdoc"
    rdoc.options += ["--quiet", "--line-numbers", "--inline-source", '--title',
      'home_run: Fast Date/DateTime classes for ruby', '--main', 'README.rdoc']
    rdoc.rdoc_files.add %w"README.rdoc CHANGELOG LICENSE ext/**/*.rb ext/*.c"
  end
rescue LoadError
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

begin
  require "rake/extensiontask"
  desc "Build the windows binary gem"
  ENV['RUBY_CC_VERSION'] = '1.8.6:1.9.1'
  load('home_run.gemspec')
  Rake::ExtensionTask.new('home_run', HOME_RUN_GEMSPEC) do |ext|
    ext.name = 'date'
    ext.ext_dir = 'ext' 
    ext.lib_dir = 'ext' 
    ext.cross_compile = true
    ext.cross_platform = 'i386-mswin32'
    ext.source_pattern = '*.c'
  end
rescue LoadError
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

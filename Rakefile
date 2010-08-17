require "rake"
require "rake/clean"

CLEAN.include %w'Makefile ext/date.*o **/*.rbc *.core' 
RUBY=ENV['RUBY'] || 'ruby'
IRBPROG=ENV['IRB'] || 'irb'

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
  sh %{mspec -I ext -rdate spec/*/*_spec.rb}
end

desc "Build the gem"
task :gem => [:clean, :parser] do
  sh %{gem build home_run.gemspec}
end

if RUBY_PLATFORM !~ /win|w32/ and File.directory?(File.join(File.expand_path(ENV['HOME']), '.rake-compiler'))
begin
  require "rake/extensiontask"
  desc "Internal--cross compile the windows binary gem"
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

  # FIXME: Incredibly hacky, should figure out how to get
  # rake compiler to do this.
  desc "Build the cross compiled windows binary gem"
  task :windows_gem do
    sh %{rm -rf tmp}
    sh %{rm -rf home_run-*.gem}
    sh %{rake cross native gem}
    sh %{rm -rf home_run-*.gem}
    sh %{mv pkg/home_run-*.gem .}
    sh %{rmdir pkg}
  end
rescue LoadError
end
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

desc "Run all benchmarks"
task :bench_all => [:bench, :mem_bench, :garbage_bench]

desc "Run comparative memory benchmarks"
task :mem_bench do
  if RUBY_PLATFORM =~ /win|w32/
    puts "Memory benchmarks not supported on Windows"
    next
  end
  stdlib = `#{RUBY} bench/mem_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/mem_bench.rb`.to_i
  puts "Date memory use,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"

  stdlib = `#{RUBY} bench/dt_mem_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/dt_mem_bench.rb`.to_i
  puts "DateTime memory use,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"
end

desc "Run comparative garbage creation benchmarks"
task :garbage_bench do
  if RUBY_PLATFORM =~ /win|w32/
    puts "Garbage creation benchmarks not supported on Windows"
    next
  end
  stdlib = `#{RUBY} bench/garbage_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/garbage_bench.rb`.to_i
  puts "Date garbage created,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"

  stdlib = `#{RUBY} bench/dt_garbage_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/dt_garbage_bench.rb`.to_i
  puts "DateTime garbage created,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"
end

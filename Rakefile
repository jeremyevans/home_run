require "rake"
require "rake/clean"
require 'rbconfig'

CLEAN.include %w'ext/date_ext/Makefile ext/date_ext/date_ext.*o **/*.rbc *.core rdoc coverage' 
RUBY=ENV['RUBY'] || File.join(RbConfig::CONFIG['bindir'], RbConfig::CONFIG['ruby_install_name'])

desc "Build the RDoc documentation"
task :rdoc do
  require 'fileutils'
  FileUtils.rm_rf('rdoc')
  sh 'rdoc --quiet --line-numbers --inline-source --output rdoc --title ' \
     '"home_run: Fast Date/DateTime classes for ruby" --main README.rdoc ' \
     'README.rdoc CHANGELOG LICENSE lib/date.rb lib/date/format.rb ext/date_ext/*.c'
end

desc "Run the specs with mspec"
task :default => :spec
task :spec do
  ENV['RUBY'] ||= RUBY
  sh %{mspec}
end

desc "Run a coverage report for the pure ruby file"
task :coverage do
  sh %{cd spec && ../bin/home_run mspec -o ../coverage -t rcov -- ../ext/date/format.rb date*/*}
end

desc "Build the gem"
task :gem => [:clean, :parser] do
  sh %{gem build home_run.gemspec}
end

desc "Try to clean up everything"
task :distclean  do
  CLEAN.concat(%w'pkg home_run-*.gem lib/1.* tmp rdoc ext/date_ext/date_parser.c')
  Rake::Task[:clean].invoke
end

if RUBY_PLATFORM !~ /mswin|mingw/ and File.directory?(File.join(File.expand_path(ENV['HOME']), '.rake-compiler'))
  begin
    ENV['RUBY_CC_VERSION'] = '1.8.6:1.9.1'
    require 'rake/extensiontask'
    load('home_run.gemspec')
    desc "Internal--cross compile the windows binary gem"
    Rake::ExtensionTask.new('date_ext', HOME_RUN_GEMSPEC) do |ext|
      ext.cross_compile = true
      ext.cross_platform = ['x86-mingw32', 'x86-mswin32-60']
    end
  rescue LoadError
  end
end

desc "Build the ragel parser"
task :parser do
  sh %{cd ext/date_ext && ragel date_parser.rl}
end

desc "Build the extension"
task :build=>[:clean] do
  sh %{cd ext/date_ext && #{RUBY} extconf.rb && make}
end

desc "Build debug version of extension"
task :build_debug=>[:clean] do
  ENV['DEBUG'] = '1'
  Rake::Task[:build].invoke
end

desc "Start an IRB shell using the extension"
task :irb do
  irb = ENV['IRB'] || File.join(RbConfig::CONFIG['bindir'], File.basename(RUBY).sub('ruby', 'irb'))
  sh %{#{irb} -I ext -r date}
end

desc "Run comparative benchmarks"
task :bench do
  sh %{#{RUBY} bench/cpu_bench.rb}
end

desc "Run all benchmarks"
task :bench_all => [:bench, :mem_bench, :garbage_bench]

desc "Run comparative parser benchmarks"
task :parser_bench do
  sh %{#{RUBY} bench/parser_bench.rb}
end

desc "Run comparative memory benchmarks"
task :mem_bench do
  if RUBY_PLATFORM =~ /mswin|mingw/
    puts "Memory benchmarks not supported on Windows"
    next
  end

  stdlib = `#{RUBY} -I #{RbConfig::CONFIG['rubylibdir']} bench/mem_bench.rb`.to_i
  home_run = `#{RUBY} -I lib -I ext/date_ext bench/mem_bench.rb`.to_i
  puts "Date memory use,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"

  stdlib = `#{RUBY} -I #{RbConfig::CONFIG['rubylibdir']} bench/dt_mem_bench.rb`.to_i
  home_run = `#{RUBY} -I lib -I ext/date_ext bench/dt_mem_bench.rb`.to_i
  puts "DateTime memory use,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"
end

desc "Run comparative garbage creation benchmarks"
task :garbage_bench do
  if RUBY_PLATFORM =~ /mswin|mingw/
    puts "Garbage creation benchmarks not supported on Windows"
    next
  end

  stdlib = `#{RUBY} -I #{RbConfig::CONFIG['rubylibdir']} bench/garbage_bench.rb`.to_i
  home_run = `#{RUBY} -I lib ext/date_ext bench/garbage_bench.rb`.to_i
  puts "Date garbage created,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"

  stdlib = `#{RUBY} -I #{RbConfig::CONFIG['rubylibdir']} bench/dt_garbage_bench.rb`.to_i
  home_run = `#{RUBY} -I lib ext/date_ext bench/dt_garbage_bench.rb`.to_i
  puts "DateTime garbage created,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"
end

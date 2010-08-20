require "rake"
require "rake/clean"
require 'rbconfig'

CLEAN.include %w'ext/Makefile ext/date_ext.*o **/*.rbc *.core rdoc' 
RUBY=ENV['RUBY'] || File.join(RbConfig::CONFIG['bindir'], RbConfig::CONFIG['ruby_install_name'])

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
  ENV['RUBY'] ||= RUBY
  sh %{mspec}
end

desc "Build the gem"
task :gem => [:clean, :parser] do
  sh %{gem build home_run.gemspec}
end

desc "Try to clean up everything"
task :distclean  do
  CLEAN.concat(%w'pkg home_run-*.gem ext/1.* tmp rdoc ext/date_parser.c')
  Rake::Task[:clean].invoke
end

if RUBY_PLATFORM !~ /win|w32/ and File.directory?(File.join(File.expand_path(ENV['HOME']), '.rake-compiler'))
begin
  require "rake/extensiontask"
  ENV['RUBY_CC_VERSION'] = '1.8.6:1.9.1'
  load('home_run.gemspec')
  desc "Internal--cross compile the windows binary gem"
  cross_platform = ENV['CROSS_PLATFORM'] || 'i386-mingw32'
  Rake::ExtensionTask.new('date_ext', HOME_RUN_GEMSPEC) do |ext|
    ext.name = 'date_ext'
    ext.ext_dir = 'ext' 
    ext.lib_dir = 'ext' 
    ext.cross_compile = true
    ext.cross_platform = cross_platform
    ext.source_pattern = '*.c'
  end

  # FIXME: Incredibly hacky, should figure out how to get
  # rake compiler to do this correctly
  desc "Build the cross compiled windows binary gem"
  task :windows_gem => [:clean, :parser] do
    sh %{rm -rf tmp pkg home_run-*.gem ext/1.*}
    system %{rake cross native gem}
    unless File.directory?('pkg')
      sh "cp ext/*.c tmp/#{cross_platform}/date_ext/1.8.6"
      system %{rake cross native gem}
      sh "cp ext/*.c tmp/#{cross_platform}/date_ext/1.9.1"
      system %{rake cross native gem}
      sh %{rake cross native gem}
    end
    sh %{rm -rf home_run-*.gem tmp ext/1.*}
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
  irb = ENV['IRB'] || File.join(RbConfig::CONFIG['bindir'], File.basename(RUBY).sub('ruby', 'irb'))
  sh %{#{irb} -I ext -r date}
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

  stdlib = `#{RUBY} -I #{RbConfig::CONFIG['rubylibdir']} bench/mem_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/mem_bench.rb`.to_i
  puts "Date memory use,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"

  stdlib = `#{RUBY} -I #{RbConfig::CONFIG['rubylibdir']} bench/dt_mem_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/dt_mem_bench.rb`.to_i
  puts "DateTime memory use,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"
end

desc "Run comparative garbage creation benchmarks"
task :garbage_bench do
  if RUBY_PLATFORM =~ /win|w32/
    puts "Garbage creation benchmarks not supported on Windows"
    next
  end

  stdlib = `#{RUBY} -I #{RbConfig::CONFIG['rubylibdir']} bench/garbage_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/garbage_bench.rb`.to_i
  puts "Date garbage created,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"

  stdlib = `#{RUBY} -I #{RbConfig::CONFIG['rubylibdir']} bench/dt_garbage_bench.rb`.to_i
  home_run = `#{RUBY} -I ext bench/dt_garbage_bench.rb`.to_i
  puts "DateTime garbage created,#{stdlib}KB,#{home_run}KB,#{sprintf('%0.1f', stdlib/home_run.to_f)}"
end

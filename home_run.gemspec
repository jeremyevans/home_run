HOME_RUN_GEMSPEC = Gem::Specification.new do |s|
  s.name = 'home_run'
  s.version = '0.9.0'
  s.platform = Gem::Platform::RUBY
  s.has_rdoc = true
  s.extra_rdoc_files = ["README.rdoc", "CHANGELOG", "LICENSE"]
  s.rdoc_options += ["--quiet", "--line-numbers", "--inline-source", '--title', 'home_run: Fast Date/DateTime classes for ruby', '--main', 'README.rdoc']
  s.summary = "Fast Date/DateTime classes for ruby"
  s.author = "Jeremy Evans"
  s.email = "code@jeremyevans.net"
  s.homepage = "http://github.com/jeremyevans/home_run"
  s.required_ruby_version = ">= 1.8.6"
  s.files = %w(LICENSE CHANGELOG README.rdoc Rakefile default.mspec) + Dir["{bin,ext,spec,bench}/**/*"]
  s.require_paths << "ext/date_ext"
  s.extensions << 'ext/date_ext/extconf.rb'
  s.bindir = 'bin'
  s.executables << 'home_run'
  s.description = <<END
home_run is an implementation of ruby's Date/DateTime classes in C,
with much better performance (20-200x) than the version in the
standard library, while being almost completely compatible.
END
end

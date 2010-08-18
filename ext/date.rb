begin
  require "date_ext"
rescue LoadError
  raise unless RUBY_PLATFORM =~ /win|w32/
  require "#{RUBY_VERSION[0...3]}/date_ext"
end
require "date/format" unless defined?(Date::Format)

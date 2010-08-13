begin
  require "date.so"
rescue LoadError
  raise unless RUBY_PLATFORM =~ /win|w32/
  require "#{RUBY_VERSION[0...3]}/date"
end
require "date/format"

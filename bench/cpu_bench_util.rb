require 'rbconfig'
$:.unshift RbConfig::CONFIG['rubylibdir']
require 'date'
require 'benchmark'

SD = Date
SDT = DateTime
Object.send(:remove_const, :Date)
Object.send(:remove_const, :DateTime)
$:.unshift(File.join(File.dirname(File.dirname(File.expand_path(__FILE__))), 'lib'))
load 'lib/date.rb'
load 'lib/date/format.rb'
HRD = Date
HRDT = DateTime
NANOS_PER_SEC = 1000000000
N = ENV['N'] ? ENV['N'].to_i : 10000
S = ENV['S'] ? ENV['S'].to_f : 2
FILTER = ARGV.empty? ? nil : Regexp.new(ARGV[0])

def compare(label, datetime=false, &block)
  return if FILTER && label !~ FILTER
  Object.send(:remove_const, :Date)
  Object.send(:remove_const, :DateTime)
  Object.send(:const_set, :Date, SD)
  Object.send(:const_set, :DateTime, SDT)
  stdlib = 0.0
  stdlib_times = 0
  while stdlib < S
    t = Time.now
    yield(datetime ? SDT : SD)
    stdlib += Time.now - t
    stdlib_times += 1
  end

  Object.send(:remove_const, :Date)
  Object.send(:remove_const, :DateTime)
  Object.send(:const_set, :Date, HRD)
  Object.send(:const_set, :DateTime, HRDT)
  home_run = 0.0
  home_run_times = 0
  while home_run < S
    t = Time.now
    yield(datetime ? HRDT : HRD)
    home_run += Time.now - t
    home_run_times += 1
  end
  
  puts sprintf('%s%s,%i,%i,%0.2f', datetime ? 'DateTime' : 'Date', label, (stdlib * NANOS_PER_SEC)/(N * stdlib_times),  (home_run * NANOS_PER_SEC)/(N * home_run_times), (stdlib/stdlib_times)/(home_run/home_run_times))
end

def dt_compare(label, &block)
  compare(label, true, &block)
end

puts "label,stdlib,home_run,times faster"

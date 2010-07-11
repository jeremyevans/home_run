require 'date'

def mem_usage
  `ps -o vsz -p #{$$}`.split.last.to_i
end
n = 100000
GC.start
GC.disable
start_mem = mem_usage
n.times{Date.civil(2010, 11, 21)}
puts(mem_usage - start_mem)

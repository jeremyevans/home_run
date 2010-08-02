require 'date'

def mem_usage
  `ps -o vsz -p #{$$}`.split.last.to_i
end
n = 100000
GC.start
start_mem = mem_usage
m = (0...n).map{nil}
GC.start
array_usage = mem_usage - start_mem
m = (0...n).map{DateTime.civil}
GC.start
puts(mem_usage - start_mem - array_usage)

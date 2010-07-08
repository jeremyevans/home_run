$:.unshift(File.join(File.dirname(File.dirname(File.expand_path(__FILE__))), 'ext'))
$:.unshift(File.join(File.dirname(File.dirname(File.expand_path(__FILE__))), 'lib'))
load(File.join(File.dirname(File.expand_path(__FILE__)), 'bench_stdlib.rb'))

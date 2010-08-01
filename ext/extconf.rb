require 'mkmf'
$CFLAGS << " -Wall#{' -DRUBY19' if  RUBY_VERSION >= '1.9.0'}"
$CFLAGS << ' -g -ggdb -rdynamic -O0 -DDEBUG' if ENV['DEBUG']
$objs = ['date.o']
create_makefile("date")

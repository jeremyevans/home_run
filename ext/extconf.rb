require 'mkmf'
$CFLAGS << " -Wall#{' -DRUBY19' if  RUBY_VERSION >= '1.9.0'}"
$CFLAGS << ' -g -ggdb -rdynamic -O0 -DDEBUG' if ENV['DEBUG']
$objs = ['home_run_date.o']
create_makefile("home_run_date")

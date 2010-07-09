require 'mkmf'
$CFLAGS << ' -Wall'
$CFLAGS << ' -g -ggdb -rdynamic -O0 -DDEBUG' if ENV['DEBUG']
create_makefile("home_run_date")

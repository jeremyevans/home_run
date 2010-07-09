require 'mkmf'
$CFLAGS << '-Wall'
$CFLAGS << ' -g -ggdb -rdynamic -O0' if ENV['DEBUG']
create_makefile("home_run_date")

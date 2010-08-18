require 'mkmf'
$CFLAGS << " -O2 -Wall#{' -DRUBY19' if  RUBY_VERSION >= '1.9.0'}#{' -DRUBY186' if  RUBY_VERSION < '1.8.7'}"
$CFLAGS << ' -Wno-extra' if $CFLAGS =~ /-Wextra/
$CFLAGS << ' -g -ggdb -rdynamic -O0 -DDEBUG' if ENV['DEBUG']
$objs = ['date_ext.o']
create_makefile("date_ext")

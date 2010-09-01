require 'mkmf'
$CFLAGS << " -DRUBY19" if  RUBY_VERSION >= '1.9.0'
$CFLAGS << " -DRUBY186" if  RUBY_VERSION < '1.8.7'
$CFLAGS << " -Wall " unless RUBY_PLATFORM =~ /solaris/
$CFLAGS << ' -g -ggdb -rdynamic -O0 -DDEBUG' if ENV['DEBUG']
$CFLAGS << " -Wconversion -Wsign-compare -Wno-unused-parameter -Wwrite-strings -Wpointer-arith -fno-common -pedantic -Wno-long-long" if ENV['STRICT']
$CFLAGS << (ENV['CFLAGS'] || '')
create_makefile("date_ext")

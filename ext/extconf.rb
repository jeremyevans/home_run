require 'mkmf'
$CFLAGS << " -O2 -Wall#{' -DRUBY19' if  RUBY_VERSION >= '1.9.0'}#{' -DRUBY186' if  RUBY_VERSION < '1.8.7'}"
$CFLAGS << ' -Wno-extra' if $CFLAGS =~ /-Wextra/
$CFLAGS << ' -g -ggdb -rdynamic -O0 -DDEBUG' if ENV['DEBUG']
$objs = ['date.o']
create_makefile("date")
File.open('Makefile', 'ab') do |f|
  f.puts <<END

install-home-run: install-so
	$(INSTALL_PROG) date.rb $(RUBYLIBDIR)
	$(INSTALL_PROG) -d $(RUBYLIBDIR)/date
	$(INSTALL_PROG) date/format.rb $(RUBYLIBDIR)/date

uninstall-home-run:
	rm -f $(RUBYARCHDIR)/date.so $(RUBYLIBDIR)/date.rb \
		$(RUBYLIBDIR)/date/format.rb
	rmdir $(RUBYLIBDIR)/date
END
end

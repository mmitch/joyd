SHELL=/bin/sh

#optimize:
#CFLAGS=-O3 -Wall

#debug:
#CFLAGS=-g -Wall

# name and version of package
PROG=joyd
VERSION=0.0.5

OLDVERSION=0.0.4
NEXTVERSION=0.0.6

#
# How to create patches (outside both directories):
# LC_ALL=C TZ=UTC0 diff -Naru old-path new-path
#
# How to publish:
# - change ALL version numbers
# - www.h.shuttle.de
# - metalab.unc.edu
# - www.freshmeat.net
# - server51.freshmeat.net
# - new version in Makefile
#
# How to apply patches (in "old" directory):
# patch -Np1
#

# name of tar.gz-archive
TARGZ=$(PROG)-$(VERSION)

# Standard GNU
prefix=/usr/local
exec_prefix=$(prefix)
bindir=$(exec_prefix)/bin
libdir=$(exec_prefix)/lib
datadir=$(prefix)/lib
statedir=$(prefix)/lib
includedir=$(prefix)/include
manext=1
mandir=$(prefix)/man
infodir=$(prefix)/info

srcdir=.

INSTALL=cp
INSTALL_PROGRAM=$(INSTALL)
INSTALL_DATA=$(INSTALL)

#
# -----
#

all:	joyd	joyreadbutton	joyreadaxis

install:
	$(INSTALL_PROGRAM) -f $(srcdir)/joyd $(bindir)
	$(INSTALL_PROGRAM) -f $(srcdir)/joyreadbutton $(bindir)
	$(INSTALL_PROGRAM) -f $(srcdir)/joyreadaxis $(bindir)
	chmod 755 $(bindir)/joyd
	chmod 755 $(bindir)/joyreadbutton
	chmod 755 $(bindir)/joyreadaxis
	$(INSTALL_DATA) -f $(srcdir)/joyd.1 $(mandir)/man1
	$(INSTALL_DATA) -f $(srcdir)/joydrc.5 $(mandir)/man5

uninstall:
	rm $(bindir)/joyd
	rm $(bindir)/joyreadbutton
	rm $(bindir)/joyreadaxis
	rm $(mandir)/man1/joyd.1
	rm $(mandir)/man5/joydrc.5

clean:
	-rm $(srcdir)/joyd
	-rm $(srcdir)/joyreadbutton
	-rm $(srcdir)/joyreadaxis
	-rm $(srcdir)/*.o
	-rm $(srcdir)/*~

distclean:	clean
mostlyclean:	clean
realclean:	clean

diff:	clean
	(\
	cd ..; \
	export LC_ALL=C; \
	export TZ=UTC0; \
	diff -Naru $(PROG)-$(OLDVERSION) $(PROG)-$(VERSION) | \
	gzip -9 > $(PROG)-patch-$(OLDVERSION)-to-$(VERSION).gz \
	)

patch:
	gunzip < $(PROG)-patch-$(VERSION)-to-$(NEXTVERSION).gz | patch -Np1

dist:
	-rm -r $(srcdir)/$(TARGZ)
	mkdir $(srcdir)/$(TARGZ)
	cp $(srcdir)/COPYING $(srcdir)/$(TARGZ)
	cp $(srcdir)/CONFIG* $(srcdir)/$(TARGZ)
	-rm $(srcdir)/$(TARGZ)/CONFIG*~
	cp $(srcdir)/HISTORY $(srcdir)/$(TARGZ)
	cp $(srcdir)/README $(srcdir)/$(TARGZ)
	cp $(srcdir)/Makefile $(srcdir)/$(TARGZ)
	cp $(srcdir)/joyd.lsm $(srcdir)/$(TARGZ)
	cp $(srcdir)/joyd.1 $(srcdir)/$(TARGZ)
	cp $(srcdir)/joydrc.5 $(srcdir)/$(TARGZ)
	cp $(srcdir)/*.c $(srcdir)/$(TARGZ)
	cp $(srcdir)/*.h $(srcdir)/$(TARGZ)
	cp $(srcdir)/joydrc* $(srcdir)/$(TARGZ)
	-rm $(srcdir)/$(TARGZ)/joydrc*~
	chmod 777 $(srcdir)/$(TARGZ)
	chmod 666 $(srcdir)/$(TARGZ)/*
	tar -c $(srcdir)/$(TARGZ) -zvf $(srcdir)/$(TARGZ).tar.gz
	rm $(srcdir)/$(TARGZ)/*
	rmdir $(srcdir)/$(TARGZ)

#
# -----
#

joyd:	daemon.o joystick.o log.o options.o signal.o string.o joyd.o
	$(CC) $(CFLAGS) 		\
		daemon.o 		\
		joystick.o 		\
		log.o 			\
		options.o 		\
		signal.o 		\
		string.o 		\
		joyd.o 			\
		-o $(srcdir)/joyd

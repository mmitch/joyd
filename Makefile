SHELL=/bin/sh

#CFLAGS=-g -Wall

# Programmname (für kurze eine-Quelldatei-Projekte)
PROG=joyd
VERSION=0.0.2

#
# How to create patches (outside both directories):
# LC_ALL=C TZ=UTC0 diff -Naur old-path new-path
#
# How to apply patches (in "old" directory):
# patch -Np1

# Name fuer tar-archiv
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
# Ab hier geht's los:
#

all:	daemon.o joystick.o log.o options.o signal.o string.o joyd.o
	$(CC) $(CFLAGS) 		\
		daemon.o 		\
		joystick.o 		\
		log.o 			\
		options.o 		\
		signal.o 		\
		string.o 		\
		joyd.o 			\
		-o $(srcdir)/$(PROG)

install:
	$(INSTALL_PROGRAM) -f $(srcdir)/$(PROG) $(bindir)
	chmod 755 $(bindir)/$(PROG)
	$(INSTALL_DATA) -f $(srcdir)/joyd.1 $(mandir)/man1
	$(INSTALL_DATA) -f $(srcdir)/joydrc.5 $(mandir)/man5

uninstall:
	rm $(bindir)/$(PROG)
	rm $(mandir)/man1/joyd.1
	rm $(mandir)/man5/joydrc.5

clean:
	-rm $(srcdir)/$(PROG)
	-rm $(srcdir)/*.o
	-rm $(srcdir)/*~

diff:
	cd ..

distclean:	clean
mostlyclean:	clean
realclean:	clean

dist:
	-rm -r $(srcdir)/$(TARGZ)
	mkdir $(srcdir)/$(TARGZ)
	cp $(srcdir)/COPYING $(srcdir)/$(TARGZ)
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

# TAGS:
# info:
# dvi:
# check:
# installcheck:
# installdirs:

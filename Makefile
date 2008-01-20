SHELL=/bin/sh

#CFLAGS=-g -Wall

# Programmname (für kurze eine-Quelldatei-Projekte)
PROG=joyd
VERSION=0.0.1

# Name fuer tar-archiv
TARGZ=$(PROG)-$(VERSION).src

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

all:	joyd.o config.o daemon.o joystick.o log.o signal.o string.o
	$(CC) $(CFLAGS) 		\
		joyd.o 			\
		config.o 		\
		daemon.o 		\
		joystick.o 		\
		log.o 			\
		signal.o 		\
		string.o 		\
		-o $(srcdir)/$(PROG)

install:
	$(INSTALL_PROGRAM) -f $(srcdir)/$(PROG) $(bindir)
	chmod 755 $(bindir)/$(PROG)

uninstall:
	rm $(bindir)/$(PROG)

clean:
	-rm $(srcdir)/$(PROG)
	-rm $(srcdir)/*.o
	-rm $(srcdir)/*~

distclean:	clean
mostlyclean:	clean
realclean:	clean

dist:
	-rm -r $(srcdir)/$(TARGZ)
	mkdir $(srcdir)/$(TARGZ)
	cp $(srcdir)/COPYING $(srcdir)/$(TARGZ)
	cp $(srcdir)/README $(srcdir)/$(TARGZ)
	cp $(srcdir)/Makefile $(srcdir)/$(TARGZ)
	cp $(srcdir)/joyd.lsm $(srcdir)/$(TARGZ)
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

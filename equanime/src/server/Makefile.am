SUBDIRS = lib modules

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/server/include \
-I$(top_srcdir)/src/include \
@EQUANIME_SERVER_CFLAGS@

bin_PROGRAMS = equ_server

equ_server_DEPENDENCIES = \
$(top_builddir)/src/server/lib/libequ_server.la

equ_server_SOURCES = \
equ_server.c

equ_server_LDADD = \
$(top_builddir)/src/server/lib/libequ_server.la \
@EQUANIME_SERVER_LIBS@

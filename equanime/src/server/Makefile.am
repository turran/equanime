SUBDIRS = drivers modules hal

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/server/include \
-I$(top_srcdir)/src/include \
@EQUANIME_SERVER_CFLAGS@

noinst_LTLIBRARIES = libequ_server.la

bin_PROGRAMS = equ_server

libequ_server_la_SOURCES =

libequ_server_la_LIBADD = \
$(top_builddir)/src/common/libequ_common.la \
$(top_builddir)/src/server/hal/libequ_hal.la \
$(top_builddir)/src/server/drivers/libequ_drivers.la

libequ_server_la_DEPENDENCIES = \
$(top_builddir)/src/common/libequ_common.la \
$(top_builddir)/src/server/hal/libequ_hal.la \
$(top_builddir)/src/server/drivers/libequ_drivers.la

equ_server_DEPENDENCIES = \
$(top_builddir)/src/server/libequ_server.la

equ_server_SOURCES = \
equ_server.c

equ_server_LDADD = \
$(top_builddir)/src/server/libequ_server.la \
@EQUANIME_SERVER_LIBS@

MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/server/include \
@EQUANIME_SERVER_CFLAGS@

dummydir = $(libdir)/equanime/
dummy_LTLIBRARIES = dummy.la

dummy_la_SOURCES = \
module.c

dummy_la_LIBADD       = $(top_builddir)/src/lib/libequanime.la
dummy_la_LDFLAGS      = @create_shared_lib@ -module -avoid-version
dummy_la_DEPENDENCIES = $(top_builddir)/config.h

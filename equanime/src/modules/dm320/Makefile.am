MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EQUANIME_CFLAGS@


controllerdir = $(libdir)/equanime/controller
controller_LTLIBRARIES = dm320.la

dm320_la_SOURCES = \
controller.c

dm320_la_LIBADD       = $(top_builddir)/src/lib/libequanime.la
dm320_la_LDFLAGS      = @create_shared_lib@ -module -avoid-version
dm320_la_DEPENDENCIES = $(top_builddir)/config.h
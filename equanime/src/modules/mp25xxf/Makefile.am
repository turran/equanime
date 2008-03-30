MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EINA_CFLAGS@


controllerdir = $(libdir)/equanime/controller
controller_LTLIBRARIES = mp2520f.la

mp2520f_la_SOURCES = \
controller.c

mp2520f_la_LIBADD       = $(top_builddir)/src/lib/libequanime.la
mp2520f_la_LDFLAGS      = @create_shared_lib@ -module -avoid-version
mp2520f_la_DEPENDENCIES = $(top_builddir)/config.h
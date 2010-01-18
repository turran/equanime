MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/server/include \
-I$(top_srcdir)/src/server/lib/drivers/dm6446/ \
@EQUANIME_SERVER_CFLAGS@


neuros_osd2dir = $(libdir)/equanime/
neuros_osd2_LTLIBRARIES = neuros_osd2.la

neuros_osd2_la_SOURCES = \
neuros_osd2.c

neuros_osd2_la_LIBADD       = $(top_builddir)/src/server/lib/libequ_server.la @EQUANIME_SERVER_LIBS@
neuros_osd2_la_LDFLAGS      = @create_shared_lib@ -module -avoid-version
neuros_osd2_la_DEPENDENCIES = \
$(top_builddir)/config.h \
$(top_builddir)/src/server/lib/libequ_server.la

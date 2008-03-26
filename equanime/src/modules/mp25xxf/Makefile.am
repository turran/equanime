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

layerdir = $(libdir)/equanime/layer
layer_LTLIBRARIES = \
mp2520f_rgb.la \
mp2520f_osd.la \
mp2520f_cursor.la

mp2520f_cursor_la_SOURCES      = cursor.c
mp2520f_cursor_la_LIBADD       = $(top_builddir)/src/lib/libequanime.la
mp2520f_cursor_la_LDFLAGS      = @create_shared_lib@ -module -avoid-version
mp2520f_cursor_la_DEPENDENCIES = $(top_builddir)/config.h

mp2520f_rgb_la_SOURCES      = rgb.c
mp2520f_rgb_la_LIBADD       = $(top_builddir)/src/lib/libequanime.la
mp2520f_rgb_la_LDFLAGS      = @create_shared_lib@ -module -avoid-version
mp2520f_rgb_la_DEPENDENCIES = $(top_builddir)/config.h

mp2520f_osd_la_SOURCES       = osd
mp2520f_osd_la_LIBADD        = $(top_builddir)/src/lib/libequanime.la
mp2520f_osd_la_LDFLAGS       = @create_shared_lib@ -module -avoid-version
mp2520f_osdr_la_DEPENDENCIES = $(top_builddir)/config.h


MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = hal

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/modules/mp25xxf

lib_LTLIBRARIES = libequanime.la

include_HEADERS = \
Equanime.h

libequanime_la_SOURCES = \
equanime_controller.c \
equanime_layer.c \
equanime_region.c

libequanime_la_LIBADD = \
$(top_srcdir)/src/modules/libequanime_modules.la \
$(top_srcdir)/src/lib/hal/libequanime_hal.la

libequanime_la_DEPENDENCIES = \
$(top_srcdir)/src/modules/libequanime_modules.la \
$(top_srcdir)/src/lib/hal/libequanime_hal.la 

libequanime_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@

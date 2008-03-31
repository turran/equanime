MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = hal

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EINA_CFLAGS@

lib_LTLIBRARIES = libequanime.la

libequanime_la_SOURCES = \
equanime_controller.c \
equanime_layer.c \
equanime_module.c \
equanime_region.c

libequanime_la_LIBADD = \
$(top_srcdir)/src/lib/hal/libequanime_hal.la \
@EINA_LIBS@ \
-ldl

libequanime_la_DEPENDENCIES = \
$(top_srcdir)/src/lib/hal/libequanime_hal.la 

libequanime_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@

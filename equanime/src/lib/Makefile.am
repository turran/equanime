MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = drivers hal

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EQUANIME_CFLAGS@

lib_LTLIBRARIES = libequanime.la

libequanime_la_SOURCES = \
equ_component.c \
equ_controller.c \
equ_core.c \
equ_csc.c \
equ_host.c \
equ_input.c \
equ_layer.c \
equ_output.c \
equ_region.c \
equ_scaler.c \
equ_surface.c

libequanime_la_LIBADD = \
$(top_builddir)/src/lib/hal/libequanime_hal.la \
$(top_builddir)/src/lib/drivers/libequanime_drivers.la \
@EQUANIME_LIBS@

libequanime_la_DEPENDENCIES = \
$(top_builddir)/src/lib/hal/libequanime_hal.la \
$(top_builddir)/src/lib/drivers/libequanime_drivers.la

libequanime_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@

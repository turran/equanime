SUBDIRS = hal drivers

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/server/include \
-I$(top_srcdir)/src/include \
@EQUANIME_SERVER_CFLAGS@

noinst_LTLIBRARIES = libequ_server.la

libequ_server_la_SOURCES = \
equ_component.c \
equ_controller.c \
equ_core.c \
equ_host.c \
equ_layer.c \
equ_output.c \
equ_rotator.c \
equ_csc.c \
equ_input.c \
equ_mode.c \
equ_pool.c \
equ_region.c \
equ_scaler.c \
equ_surface.c

libequ_server_la_LIBADD = \
$(top_builddir)/src/common/libequ_common.la \
$(top_builddir)/src/server/lib/hal/libequ_hal.la \
$(top_builddir)/src/server/lib/drivers/libequ_drivers.la \
@EQUANIME_SERVER_LIBS@

libequ_server_la_DEPENDENCIES = \
$(top_builddir)/src/common/libequ_common.la \
$(top_builddir)/src/server/lib/hal/libequ_hal.la \
$(top_builddir)/src/server/lib/drivers/libequ_drivers.la


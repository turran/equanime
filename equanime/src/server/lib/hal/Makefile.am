MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/server/include \
@EQUANIME_SERVER_CFLAGS@

noinst_LTLIBRARIES = libequ_hal.la

libequ_hal_la_SOURCES = \
hal_uio.c \
hal_i2c.c \
hal_dmem.c

libequ_hal_la_LIBADD =

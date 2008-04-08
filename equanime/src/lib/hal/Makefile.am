MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EINA_CFLAGS@

noinst_LTLIBRARIES = libequanime_hal.la

libequanime_hal_la_SOURCES = \
hal_uio.c \
hal_dmem.c
 
libequanime_hal_la_LIBADD =
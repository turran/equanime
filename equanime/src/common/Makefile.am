## Process this file with automake to produce Makefile.in
MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EQUANIME_COMMON_CFLAGS@

noinst_LTLIBRARIES = libequ_common.la

libequ_common_la_SOURCES = \
equ_message.c

libequ_common_la_LIBADD = \
@EQUANIME_COMMON_LIBS@

libequ_common_la_DEPENDENCIES =

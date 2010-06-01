## Process this file with automake to produce Makefile.in
MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@ESHM_CFLAGS@

noinst_LTLIBRARIES = libeshm_common.la

libeshm_common_la_SOURCES = \
eshm_message.c

libeshm_common_la_LIBADD = \
@ESHM_LIBS@

libeshm_common_la_DEPENDENCIES =

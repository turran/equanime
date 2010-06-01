## Process this file with automake to produce Makefile.in
MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@ESHM_CFLAGS@

lib_LTLIBRARIES = libeshm.la

libeshm_la_SOURCES = \
eshm_main.c \
eshm_segment.c

libeshm_la_LIBADD = \
${top_builddir}/src/common/libeshm_common.la \
@ESHM_LIBS@

libeshm_la_DEPENDENCIES = \
${top_builddir}/src/common/libeshm_common.la

libeshm_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@ESHM_CFLAGS@

if BUILD_EXAMPLE

bin_PROGRAMS = eshm_client

eshm_client_DEPENDENCIES = $(top_builddir)/src/lib/libeshm.la

eshm_client_SOURCES = \
eshm_client.c

eshm_client_LDADD = \
$(top_builddir)/src/lib/libeshm.la \
@ESHM_LIBS@

endif

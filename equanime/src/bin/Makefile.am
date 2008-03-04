MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib

bin_PROGRAMS = equanime_info

equanime_info_DEPENDENCIES = $(top_builddir)/src/lib/libequanime.la

equanime_info_SOURCES = \
equanime_info.c

equanime_info_LDADD = \
$(top_builddir)/src/lib/libequanime.la
MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EQUANIME_LIB_CFLAGS@

bin_PROGRAMS = equanime_mode_set

equanime_mode_set_DEPENDENCIES = $(top_builddir)/src/lib/libequanime.la

equanime_mode_set_SOURCES = \
equanime_mode_set.c

equanime_mode_set_LDADD = \
$(top_builddir)/src/lib/libequanime.la \
@EQUANIME_LIB_LIBS@


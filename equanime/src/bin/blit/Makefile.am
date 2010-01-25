MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EQUANIME_LIB_CFLAGS@

bin_PROGRAMS = equanime_blit

equanime_blit_DEPENDENCIES = $(top_builddir)/src/lib/libequanime.la

equanime_blit_SOURCES = \
equanime_blit.c

equanime_blit_LDADD = \
$(top_builddir)/src/lib/libequanime.la \
@EQUANIME_LIB_LIBS@

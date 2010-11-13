MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EIX_LIB_CFLAGS@

bin_PROGRAMS = eix_parser

eix_parser_DEPENDENCIES = $(top_builddir)/src/lib/libeix.la

eix_parser_SOURCES = \
eix_parser.c

#scan.o: scan.l parse.c
#parse.o: parse.y

eix_parser_LDADD = \
$(top_builddir)/src/lib/libequanime.la \
@EIX_LIB_LIBS@

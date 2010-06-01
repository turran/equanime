MAINTAINERCLEANFILES = \
Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/include \
@EIX_CFLAGS@

lib_LTLIBRARIES = libeix.la

libeix_la_SOURCES = \
eix_message.c \
eix_client.c \
eix_server.c

libeix_la_LIBADD = \
@EIX_LIBS@

libeix_la_DEPENDENCIES =

libeix_la_LDFLAGS = -no-undefined -version-info @version_info@

MAINTAINERCLEANFILES = \
Makefile.in

if BUILD_MODULE_SDL

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/server/include \
-I$(top_srcdir)/src/include \
@EQUANIME_SERVER_CFLAGS@

sdldir = $(libdir)/equanime/
sdl_LTLIBRARIES = sdl.la

sdl_la_SOURCES = \
module.c

sdl_la_LIBADD       = $(top_builddir)/src/server/lib/libequ_server.la @EQUANIME_SERVER_LIBS@
sdl_la_LDFLAGS      = @create_shared_lib@ -module -avoid-version
sdl_la_DEPENDENCIES = \
$(top_builddir)/config.h \
$(top_builddir)/src/server/lib/libequ_server.la

endif

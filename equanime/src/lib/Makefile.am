MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EQUANIME_LIB_CFLAGS@

lib_LTLIBRARIES = libequanime.la

libequanime_la_SOURCES = \
equ_component.c \
equ_controller.c \
equ_core.c \
equ_csc.c \
equ_host.c \
equ_input.c \
equ_layer.c \
equ_mode.c \
equ_output.c \
equ_region.c \
equ_scaler.c \
equ_surface.c

libequanime_la_LIBADD = \
@EQUANIME_LIB_LIBS@

libequanime_la_DEPENDENCIES = \
$(top_builddir)/src/common/libequ_common.la

libequanime_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@

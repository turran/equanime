# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT(equanime, 0.0.4, enlightenment-devel@lists.sourceforge.net)
AC_PREREQ(2.52)
AC_CONFIG_SRCDIR(configure.ac)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ISC_POSIX

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(config.h)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST
AC_C_BIGENDIAN

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

VMAJ=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $1);}'`
VMIN=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $2);}'`
VMIC=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $3);}'`
SNAP=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $4);}'`
version_info=`expr $VMAJ + $VMIN`":$VMIC:$VMIN"
AC_SUBST(version_info)

create_shared_lib=""
case "$host_os" in
    mingw|mingw32)
	create_shared_lib="-no-undefined"
    ;;
esac
AC_SUBST(create_shared_lib)

# Common
PKG_CHECK_MODULES(EINA, [eina >= 1.0.0])

# Server modules
PKG_CHECK_MODULES(ESHM, [eshm])
PKG_CHECK_MODULES(EIX, [eix])
PKG_CHECK_MODULES(SDL, [sdl], [have_sdl=yes], [have_sdl=no])
if test "x$have_sdl" = "xyes"; then
	build_module_sdl=yes
	AC_DEFINE(BUILD_MODULE_SDL, [1], [Build support for SDL engine])
else
	build_module_sdl=no
fi
AM_CONDITIONAL(BUILD_MODULE_SDL, test "x$build_module_sdl" = "xyes")

EQUANIME_COMMON_CFLAGS="$EINA_CFLAGS $EIX_CFLAGS $ESHM_CFLAGS"
EQUANIME_COMMON_LIBS="$EINA_LIBS $EIX_LIBS $ESHM_LIBS"
AC_SUBST(EQUANIME_COMMON_CFLAGS)
AC_SUBST(EQUANIME_COMMON_LIBS)

EQUANIME_LIB_CFLAGS="$EQUANIME_COMMON_CFLAGS"
EQUANIME_LIB_LIBS="$EQUANIME_COMMON_LIBS"
AC_SUBST(EQUANIME_LIB_CFLAGS)
AC_SUBST(EQUANIME_LIB_LIBS)

EQUANIME_SERVER_CFLAGS="$EQUANIME_COMMON_CFLAGS"
EQUANIME_SERVER_LIBS="$EQUANIME_COMMON_LIBS"
AC_SUBST(EQUANIME_SERVER_CFLAGS)
AC_SUBST(EQUANIME_SERVER_LIBS)

## Make the debug preprocessor configurable

AC_OUTPUT([
Makefile
equanime.pc
src/Makefile
src/common/Makefile
src/lib/Makefile
src/server/Makefile
src/server/modules/Makefile
src/server/modules/neuros_osd2/Makefile
src/server/modules/dummy/Makefile
src/server/modules/sdl/Makefile
src/server/lib/drivers/Makefile
src/server/lib/drivers/mp25xxf/Makefile
src/server/lib/drivers/mp2520f/Makefile
src/server/lib/drivers/mp2530f/Makefile
src/server/lib/drivers/dm320/Makefile
src/server/lib/drivers/dm6446/Makefile
src/server/lib/drivers/ths8200/Makefile
src/server/lib/drivers/ths7313/Makefile
src/server/lib/hal/Makefile
src/server/lib/Makefile
src/bin/Makefile
src/bin/mode/Makefile
src/bin/info/Makefile
src/bin/blit/Makefile
src/include/Makefile
],[
])


#####################################################################
## Info

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo
echo
echo "Configuration Options Summary:"
echo
echo "Drivers:"
echo
echo
echo "Installation Path.........: $prefix"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE,"
echo "and then afterwards as root (or the user who will install this), type"
echo "'make install'. Change users with 'su' or 'sudo' appropriately."
echo

# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT([eix], [0.0.1], [enlightenment-devel@lists.sourceforge.net])
AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ISC_POSIX

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(src/include/config.h)

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


### Additional options to configure


### Checks for programs
AC_PROG_CC


### Checks for libraries
PKG_CHECK_MODULES(EINA, [eina-0 >= 0.0.1])
PKG_CHECK_MODULES(EET, [eet >= 1.2.2])
PKG_CHECK_MODULES(ECORE, [ecore])
PKG_CHECK_MODULES(ECORE_CON, [ecore-con])

### Checks for header files


### Checks for types


### Checks for structures


### Checks for compiler characteristics

AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST
AC_C_BIGENDIAN


### Checks for linker characteristics


### Checks for library functions

### Substitutions

EIX_CFLAGS="$EINA_CFLAGS $ECORE_CFLAGS $ECORE_CON_CFLAGS"
EIX_LIBS="$EINA_LIBS $ECORE_LIBS $ECORE_CON_LIBS"
AC_SUBST(EIX_CFLAGS)
AC_SUBST(EIX_LIBS)

## Make the debug preprocessor configurable

AC_OUTPUT([
Makefile
eix.pc
src/Makefile
src/include/Makefile
src/lib/Makefile
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
echo "Installation Path...........................: ${prefix}"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE,"
echo "and then afterwards as root (or the user who will install this), type"
echo "'make install'. Change users with 'su' or 'sudo' appropriately."
echo

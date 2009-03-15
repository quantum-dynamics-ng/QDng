dnl Define macro for libxml2 checking
dnl Fails if test routines fail
dnl Sets : LIBXML2_LIBS, LIBXML2_CFLAGS
AC_DEFUN([ACX_CHECK_LIBXML2], [
  succeeded=no

  if test -z "$LIBXML2_CONFIG"; then
    AC_PATH_PROG(LIBXML2_CONFIG, xml2-config, no)
  fi

  if test "$LIBXML2_CONFIG" = "no" ; then
    dnl Let's try a standard path if the xml2-config script is not available
    LIBXML2_CFLAGS="-I/usr/include/libxml2"
    LIBXML2_LIBS="-lxml2 -lz -lm"
  else
    dnl Location for headers etc.
    AC_MSG_CHECKING(LIBXML2_CFLAGS)
    LIBXML2_CFLAGS=`$LIBXML2_CONFIG --cflags`
    AC_MSG_RESULT($LIBXML2_CFLAGS)
    dnl Libraries
    AC_MSG_CHECKING(LIBXML2_LIBS)
    LIBXML2_LIBS=`$LIBXML2_CONFIG --libs`
    AC_MSG_RESULT($LIBXML2_LIBS)
  fi
  AC_CHECK_LIB(xml2, xmlDocGetRootElement,,AC_MSG_ERROR(libxml is missing))
  AC_SUBST(LIBXML2_LIBS)
  AC_SUBST(LIBXML2_CFLAGS)

])

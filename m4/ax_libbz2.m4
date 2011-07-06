#
# Copy & paste from ax_check_zlib
#

AU_ALIAS([CHECK_LIBBZ2], [AX_CHECK_LIBBZ2])
AC_DEFUN([AX_CHECK_LIBBZ2],
#
# Handle user hints
#
[AC_MSG_CHECKING(if libbz2 is wanted)
AC_ARG_WITH(libbz2,
[  --with-libbz2=DIR root directory path of libbz2 installation [defaults to
                    /usr/local or /usr if not found in /usr/local]
  --without-libbz2 to disable libbz2 usage completely],
[if test "$withval" != no ; then
  AC_MSG_RESULT(yes)
  if test -d "$withval"
  then
    ZLIB_HOME="$withval"
  else
    AC_MSG_WARN([Sorry, $withval does not exist, checking usual places])
  fi
else
  AC_MSG_RESULT(no)
fi],
[AC_MSG_RESULT(yes)])

LIBBZ2_HOME=/usr/local
if test ! -f "${LIBBZ2_HOME}/include/bzlib.h"
then
        LIBBZ2_HOME=/usr
fi

#
# Locate zlib, if wanted
#
if test "$withval" != no ; then
   if test -n "${LIBBZ2_HOME}"
   then
         LIBBZ2_OLD_LDFLAGS=$LDFLAGS
         LIBBZ2_OLD_CPPFLAGS=$LDFLAGS
         LDFLAGS="$LDFLAGS -L${LIBBZ2_HOME}/lib"
         CPPFLAGS="$CPPFLAGS -I${LIBBZ2_HOME}/include"
         AC_LANG_SAVE
         AC_LANG_C
         AC_CHECK_LIB(bz2, BZ2_bzCompressInit, [libbz2_cv_libbz2=yes], [libbz2_cv_libbz2=no])
         AC_CHECK_HEADER(bzlib.h, [libbz2_cv_libbz2_h=yes], [libbz2_cv_libbz2_h=no])
         AC_LANG_RESTORE
         if test "$libbz2_cv_libbz2" = "yes" -a "$libbz2_cv_libbz2_h" = "yes"
         then
                  #
                  # If both library and header were found, use them
                  #
                  AC_CHECK_LIB(bz2, BZ2_bzCompressInit)
                  AC_MSG_CHECKING(libbz2 in ${ZLIB_HOME})
                  AC_MSG_RESULT(ok)
         else
                  #
                  # If either header or library was not found, revert and bomb
                  #
                  AC_MSG_CHECKING(BZ2_bzCompressInit in ${LIBBZ2_HOME})
                  LDFLAGS="$LIBBZ2_OLD_LDFLAGS"
                  CPPFLAGS="$LIBBZ2_OLD_CPPFLAGS"
                  AC_MSG_RESULT(failed)
                  AC_MSG_ERROR(either specify a valid libbz2 installation with --with-libbz2=DIR or disable libbz2 usage with --without-libbz2)
         fi
   fi
fi
])

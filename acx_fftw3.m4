AC_DEFUN([ACX_FFTW3],[
  AC_ARG_WITH([fftw3],[AS_HELP_STRING([--with-fftw3=PATH],[Prefix fftw installation])])
  if test "x$with_fftw3" = x; then
    AC_CHECK_HEADER(fftw3.h,,AC_MSG_ERROR(fftw3 headers are missing))
    AC_CHECK_LIB(fftw3, fftw_plan_dft,[FFTW_LIBS="-lfftw3"],AC_MSG_ERROR(fftw3 libs are missing))
  else
    save_LIBS="$LIBS"; $LIBS="$LIBS -L$with_fftw3/lib"
    save_CFLAGS="$CFLAGS"; CFLAGS="$CFLAGS -I$with_fftw3w/include"
    AC_CHECK_HEADER(fftw3.h,[FFTW_CFLAGS="-I$with_fftw3w/include"],AC_MSG_ERROR(fftw3 headers are missing))
    AC_CHECK_LIB(fftw3, fftw_plan_dft,[FFTW_LIBS="-lfftw3 -L$with_fftw3/lib"],AC_MSG_ERROR(fftw3 libs are missing))
    AC_MSG_NOTICE(["Using fftw3 installation from $with_fftw3"])
    LIBS="$save_LIBS"
    CFLAGS="$save_CFLAGS"
  fi
  AC_SUBST(FFTW_LIBS)
  AC_SUBST(FFTW_CFLAGS)
])

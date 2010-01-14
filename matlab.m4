dnl  ====================================================
dnl             MATLAB library macros
dnl  ====================================================

dnl
dnl  CHECK_MATLAB - find Matlab libraries
dnl  ----------------------------------------------------
AC_DEFUN([CHECK_MATLAB],
[
AC_ARG_WITH(matlab,
[  --with-matlab=DIR	the directory where Matlab is installed ],
MATLAB_DIR=${withval},
MATLAB_DIR=)

if test -n "${MATLAB_DIR}"
then
    AC_MSG_CHECKING(for Matlab software)

    case $build_os in
    *linux*)
        MATLAB_FLAGS="-I${MATLAB_DIR}/extern/include -I${MATLAB_DIR}/simulink/include -DMATLAB_MEX_FILE -fPIC -ansi -fno-omit-frame-pointer -fexceptions -D_GNU_SOURCE -pthread -O -DNDEBUG";
        MATLAB_LINK="-shared -pthread -W1,--version-script,${MATLAB_DIR}/extern/lib/glnxa64/mexFunction.map";
        MATLAB_LIB="-W1,--rpath-link,${MATLAB_DIR}/extern/lib/glnxa64,--rpath-link,${MATLAB_DIR}/bin/glnxa64 -L${MATLAB_DIR}/bin/glnxa64 -lmx -lmex -lmat -lm";
        MEXEXT=mexa64 -lstdc++;;
    *cygwin*)
        MATLAB_FLAGS="-I${MATLAB_DIR}/extern/include -I${MATLAB_DIR}/simulink/include -fno-exceptions -mno-cygwin -DMATLAB_MEX_FILE -DNDEBUG";
        MATLAB_LINK="-shared -mno-cygwin -W1,--version-script,${MATLAB_DIR}/extern/lib/win32/mexFunction.def";
        MATLAB_LIB="-W1,--rpath-link,${MATLAB_DIR}/extern/lib/win32,--rpath-link,${MATLAB_DIR}/bin/win32 ${MATLAB_DIR}/bin/win32/libmx.a ${MATLAB_DIR}/bin/win32/libmex.a ${MATLAB_DIR}/bin/win32/libmat.a -lm";
        MATLAB_LINK="-shared -mno-cygwin -L${MATLAB_DIR}/bin/win32 -W1,--version-script,${MATLAB_DIR}/extern/lib/win32/mexFunction.def";
        MATLAB_LIB="-lmx -lmex -lmat -lm";
        MEXEXT=dll;
        if test ! -e "${MATLAB_DIR}/bin/win32/libmx.a"
        then
            cd ${MATLAB_DIR}/bin/win32
            libmx=`dlltool -llibmx.a -d${MATLAB_DIR}/extern/include/libmx.def -Dlibmx.dll`
            cd -
        fi
        if test ! -e "${MATLAB_DIR}/bin/win32/libmex.a"
        then
            cd ${MATLAB_DIR}/bin/win32
            libmex=`dlltool -llibmex.a -d${MATLAB_DIR}/extern/include/libmex.def -Dlibmex.dll`
            cd -
        fi
        if test ! -e "${MATLAB_DIR}/bin/win32/libmex.a"
        then
            cd ${MATLAB_DIR}/bin/win32
            libmat=`dlltool -llibmat.a -d${MATLAB_DIR}/extern/include/libmat.def -Dlibmat.dll`
            cd -
        fi;;
    *mingw*)
        MATLAB_FLAGS="-I${MATLAB_DIR}/extern/include -I${MATLAB_DIR}/simulink/include -fno-exceptions -DMATLAB_MEX_FILE -DNDEBUG";
        MATLAB_LINK="-shared -W1,--version-script,${MATLAB_DIR}/extern/lib/win32/mexFunction.def";
        MATLAB_LIB="-W1,--rpath-link,${MATLAB_DIR}/extern/lib/win32,--rpath-link,${MATLAB_DIR}/bin/win32 ${MATLAB_DIR}/bin/win32/libmx.a ${MATLAB_DIR}/bin/win32/libmex.a ${MATLAB_DIR}/bin/win32/libmat.a -lm";
        MATLAB_LINK="-shared -L${MATLAB_DIR}/bin/win32 -W1,--version-script,${MATLAB_DIR}/extern/lib/win32/mexFunction.def";
        MATLAB_LIB="-lmx -lmex -lmat -lm";
        MEXEXT=dll;
        if test ! -e "${MATLAB_DIR}/bin/win32/libmx.a"
        then
            cd ${MATLAB_DIR}/bin/win32
            libmx=`dlltool -llibmx.a -d${MATLAB_DIR}/extern/include/libmx.def -Dlibmx.dll`
            cd -
        fi
        if test ! -e "${MATLAB_DIR}/bin/win32/libmex.a"
        then
            cd ${MATLAB_DIR}/bin/win32
            libmex=`dlltool -llibmex.a -d${MATLAB_DIR}/extern/include/libmex.def -Dlibmex.dll`
            cd -
        fi
        if test ! -e "${MATLAB_DIR}/bin/win32/libmex.a"
        then
            cd ${MATLAB_DIR}/bin/win32
            libmat=`dlltool -llibmat.a -d${MATLAB_DIR}/extern/include/libmat.def -Dlibmat.dll`
            cd -
        fi;;
    esac
    AC_MSG_RESULT($MATLAB_LINK $MATLAB_LIB)
    AC_SUBST(MATLAB_DIR)
    AC_SUBST(MATLAB_LIB)
    AC_SUBST(MATLAB_LINK)
    AC_SUBST(MATLAB_FLAGS)
    AC_SUBST(MEXEXT)
    AC_SUBST(MEXVERSION)
    AM_CONDITIONAL(HAVE_MATLAB, test "xyes" = "xyes" )
    have_matlab=yes
else
    AM_CONDITIONAL(HAVE_MATLAB, test "xno" = "xyes" )
    have_matlab=no
fi
])

INCLUDES = -I$(top_srcdir)/src \
	-I/home/mko/src/QDng/devel-main/src/cppunit/include -I/usr/include/libxml2 $(all_includes)
METASOURCES = AUTO
AM_CXXFLAGS = -O2 -Werror -pedantic -pedantic-errors

noinst_HEADERS = TestMathFunctions.h TestCheby.h

noinst_PROGRAMS = runtest
runtest_SOURCES = dVecMinMax.cpp main.cpp TestMathFunctions.cpp TestCheby.cpp
runtest_LDFLAGS = ../$(top_builddir)/src/cppunit/lib/libcppunit.a
runtest_LDADD = $(top_builddir)/src/qdlib/libqd.la \
	$(top_builddir)/src/sys/libtools.la $(top_builddir)/src/math/libmath.la \
	$(top_builddir)/src/slatec/libdbesj.la $(top_builddir)/src/slatec/libslatec_common.la \
	$(top_builddir)/src/fft/libfft.la -lfftw3 -lg2c -lblas -llapack -ldl

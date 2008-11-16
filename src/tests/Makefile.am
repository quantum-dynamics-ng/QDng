INCLUDES = -I$(top_srcdir)/src $(all_includes) -I/usr/include/libxml2
METASOURCES = AUTO
AM_CXXFLAGS = -O2 -Werror -pedantic -pedantic-errors

noinst_HEADERS = OGridHarmonic.h Test.h TestCheby.h TestGetopt.h TestLogger.h \
	TestMathFunctions.h TestStridedVector.h TestViews.h WFGridHarmonic.h

noinst_PROGRAMS = Getopt logger runtest
runtest_SOURCES = dVecMinMax.cpp TestViews.cpp main.cpp TestMathFunctions.cpp TestCheby.cpp \
	WFGridHarmonic.cpp OGridHarmonic.cpp 
runtest_LDFLAGS = ../$(top_builddir)/src/cppunit/lib/libcppunit.a
runtest_LDADD = $(top_builddir)/src/fft/libfft.la \
	$(top_builddir)/src/math/libmath.la $(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/sys/libtools.la \
	-lcppunit -lblas -ldl -lfftw3 -lg2c -llapack -lslatec
Getopt_SOURCES = TestGetopt.cpp main.cpp
Getopt_LDADD = $(top_builddir)/src/sys/libtools.la -lcppunit
bin_PROGRAMS = StridedVector
StridedVector_SOURCES = TestStridedVector.cpp main.cpp
StridedVector_LDADD = $(top_builddir)/src/math/libmath.la -lcppunit

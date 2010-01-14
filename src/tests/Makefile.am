INCLUDES = -I$(top_srcdir)/src -I/usr/include/cppunit \
	$(all_includes) -I/usr/include/libxml2
METASOURCES = AUTO
AM_CXXFLAGS = -O2 -Werror -pedantic -pedantic-errors

noinst_HEADERS = ComplexNumbers.h GridSystem.h MultElementsTest.h \
	OGridHarmonic.h OGridNablaSqTest.h OGridNablaTest.h QDClock.h TestCheby.h \
	TestGetopt.h TestLogger.h TestMathFunctions.h TestStridedVector.h TestViews.h \
	Vector.h WFGridCartesian.h WFGridHarmonic.h WFGridSystemTest.h \
	WaveFunctionTest.h defs.h function_gens.h

noinst_PROGRAMS = runtest
runtest_SOURCES = ComplexNumbers.cpp GridSystem.cpp MultElementsTest.cpp \
	OGridNablaSqTest.cpp OGridNablaTest.cpp QDClock.cpp TestViews.cpp TestViews.h Vector.cpp \
	WFGridCartesianTest.cpp WFGridSystemTest.cpp WaveFunctionTest.cpp dVecMinMax.cpp dVecMinMax.h \
	function_gens.cpp main.cpp
runtest_LDADD = $(top_builddir)/src/math/libmath.la \
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/tools/libtools.la $(top_builddir)/src/fft/libfft.la \
	-lfftw3 -lcppunit

INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/fftw/api -I/usr/include/cppunit \
	$(all_includes) -I/usr/include/libxml2
METASOURCES = AUTO
AM_CXXFLAGS = -O2 -Werror -pedantic -pedantic-errors

if HAVE_OPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LDFLAGS = $(OPENMP_CXXFLAGS)
endif


noinst_HEADERS = ComplexNumbers.h TmpFileTest.h MatrixTest.h LapackTest.h GridSystem.h FileWFTest.h MultElementsTest.h \
	OGridHarmonic.h OGridNablaSqTest.h OGridNablaTest.h QDClock.h TestCheby.h \
	TestGetopt.h TestLogger.h TestMathFunctions.h TestStridedVector.h TestViews.h \
	Vector.h WFGridCartesian.h WFGridHarmonic.h WFGridSystemTest.h \
	WaveFunctionTest.h defs.h function_gens.h OHermitianMatrixTest.h WFLevelTest.h OMatDipoleTest.h OGSPOTest.h \
	WFBufferTest.h

noinst_PROGRAMS = runtest
runtest_SOURCES = ComplexNumbers.cpp TmpFileTest.cpp Vector.cpp TestViews.cpp TestViews.h MultElementsTest.cpp \
	dVecMinMax.cpp dVecMinMax.h MatrixTest.cpp LapackTest.cpp GridSystem.cpp FileWFTest.cpp QDClock.cpp \
	WFLevelTest.cpp WFBufferTest.cpp OHermitianMatrixTest.cpp OMatDipoleTest.cpp OGridNablaSqTest.cpp OGridNablaTest.cpp \
	WaveFunctionTest.cpp WFGridCartesianTest.cpp WFGridSystemTest.cpp OGSPOTest.cpp \
	function_gens.cpp main.cpp
runtest_LDADD = $(top_builddir)/src/lib/libqdng.la \
	 -lcppunit

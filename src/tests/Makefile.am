INCLUDES = -I$(top_srcdir)/src -I$(FFTW_INCLUDE) -I/usr/include/cppunit \
	$(all_includes) -I/usr/include/libxml2
METASOURCES = AUTO
AM_CXXFLAGS = $(DEVEL_FLAGS) $(OPT_FLAGS) -Wno-long-long

if HAVEOPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LDFLAGS = $(OPENMP_CXXFLAGS)
endif


noinst_HEADERS = ComplexNumbers.h TmpFileTest.h FIFOTEst.h MatrixTest.h LapackTest.h GridSystem.h FileWFTest.h MultElementsTest.h \
        PeakFinderTest.h FFTnTest.h ParamContainerTest.h \
	OGridHarmonic.h OGridNablaSqTest.h OGridNablaTest.h QDClock.h TestCheby.h \
	TestGetopt.h TestLogger.h TestMathFunctions.h TestStridedVector.h TestViews.h \
	Vector.h WFGridCartesian.h WFGridCartesianTest.h WFGridHarmonic.h WFGridSystemTest.h \
	WaveFunctionTest.h defs.h function_gens.h OHermitianMatrixTest.h WFLevelTest.h OMatDipoleTest.h OGSPOTest.h \
	WFBufferTest.h MemoryTest.h ThreadTest.h OGridHOFDTest.h FileSingleTest.h

noinst_PROGRAMS = runtest mpitest

runtest_SOURCES = MemoryTest.cpp ComplexNumbers.cpp ParamContainerTest.cpp \
    TmpFileTest.cpp FIFOTEst.cpp Vector.cpp TestViews.cpp TestViews.h MultElementsTest.cpp \
	dVecMinMax.cpp dVecMinMax.h PeakFinderTest.cpp MatrixTest.cpp LapackTest.cpp GridSystem.cpp \
	FileSingleTest.cpp FileWFTest.cpp QDClock.cpp \
	FFTnTest.cpp \
	WFLevelTest.cpp WFBufferTest.cpp OHermitianMatrixTest.cpp OMatDipoleTest.cpp OGridNablaSqTest.cpp OGridNablaTest.cpp OGridHOFDTest.cpp \
	WaveFunctionTest.cpp WFGridCartesianTest.cpp WFGridSystemTest.cpp OGSPOTest.cpp TestCheby.cpp \
	function_gens.cpp main.cpp
runtest_LDADD = $(top_builddir)/src/lib/libqdng.la \
	 -lcppunit -lprotobuf

runtest_LDFLAGS =  -pthread	
mpitest_SOURCES = mpitest.cpp
mpitest_LDADD = $(top_builddir)/src/lib/libqdng.la

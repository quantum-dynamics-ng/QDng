INCLUDES = -I$(top_srcdir)/src -I/usr/include/cppunit \
	$(all_includes) -I/usr/include/libxml2
METASOURCES = AUTO
AM_CXXFLAGS = -O2 -Werror -pedantic -pedantic-errors

noinst_HEADERS = ComplexNumbers.h GridSystem.h OGridHarmonic.h QDClock.h Test.h \
	TestCheby.h TestGetopt.h TestLogger.h TestMathFunctions.h TestStridedVector.h \
	TestViews.h Vector.h WFGridHarmonic.h defs.h

noinst_PROGRAMS = Getopt runtest
runtest_SOURCES = ComplexNumbers.cpp GridSystem.cpp QDClock.cpp TestViews.cpp \
	TestViews.h Vector.cpp dVecMinMax.cpp dVecMinMax.h main.cpp
runtest_LDADD = $(top_builddir)/src/tools/libtools.la \
	$(top_builddir)/src/math/libmath.la $(top_builddir)/src/qdlib/libqd.la -lcppunit

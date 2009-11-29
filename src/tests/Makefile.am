INCLUDES = -I$(top_srcdir)/src -I/usr/include/cppunit \
	$(all_includes) -I/usr/include/libxml2
METASOURCES = AUTO
AM_CXXFLAGS = -O2 -Werror -pedantic -pedantic-errors

noinst_HEADERS = ComplexNumbers.h OGridHarmonic.h Test.h TestCheby.h \
	TestGetopt.h TestLogger.h TestMathFunctions.h TestStridedVector.h TestViews.h \
	WFGridHarmonic.h defs.h

noinst_PROGRAMS = Getopt runtest
runtest_SOURCES = ComplexNumbers.cpp main.cpp
runtest_LDADD = $(top_builddir)/src/math/libmath.la -lcppunit

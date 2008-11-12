INCLUDES = -I$(top_srcdir)/src \
	-I/home/mko/src/QDng/devel-main/src/cppunit/include -I/usr/include/libxml2 $(all_includes)
METASOURCES = AUTO
AM_CXXFLAGS = -O2 -Werror -pedantic -pedantic-errors
noinst_PROGRAMS = dVecMinMax
dVecMinMax_SOURCES = dVecMinMax.cpp main.cpp TestMathFunctions.cpp
dVecMinMax_LDFLAGS = ../$(top_builddir)/src/cppunit/lib/libcppunit.a
dVecMinMax_LDADD = $(top_builddir)/src/math/libmath.la -lg2c -lslatec -llapack \
	-lblas -ldl

noinst_HEADERS = TestMathFunctions.h


INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libmath.la
libmath_la_SOURCES = dcomplex.cpp math_functions.cpp
AM_CXXFLAGS = -O3 -pedantic -pedantic-errors -fPIC -ansi -ffast-math -funroll-loops $(BLAS_LIBS)
mathdir = $(pkgincludedir)/math
math_HEADERS = Matrix.h math_functions.h typedefs.h Vector.h VectorView.h dcomplex.h

if HAVEOPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
endif



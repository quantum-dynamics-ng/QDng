INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libmath.la
libmath_la_SOURCES = dcomplex.cpp math_functions.cpp
AM_CXXFLAGS = -O3 -pedantic -pedantic-errors -ansi -ffast-math -funroll-loops $(BLAS_LIBS)

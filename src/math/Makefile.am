INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libmath.la
libmath_la_SOURCES = dcomplex.cpp math_functions.cpp
AM_CXXFLAGS = -fomit-frame-pointer -Werror -pedantic -pedantic-errors -O3
libmath_la_LIBADD = $(top_builddir)/src/slatec/libslatec.la

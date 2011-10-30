INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libmath.la
libmath_la_SOURCES = dcomplex.cpp math_functions.cpp
AM_CXXFLAGS = $(BLAS_LIBS) $(DEVEL_FLAGS) $(OPT_FLAGS)
mathdir = $(pkgincludedir)/math
math_HEADERS = Matrix.h math_functions.h typedefs.h Vector.h VectorView.h dcomplex.h \
               m128dc.h m256dc.h VectorPritives.h

if HAVEOPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
endif



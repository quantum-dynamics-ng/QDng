
# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/fftw/api $(LIBXML2_CFLAGS) $(all_includes)

# the library search path.

OPT_DIRS=

if CPPUNIT
  OPT_DIRS+=tests
endif

if HAVE_MATLAB
  OPT_DIRS+=mex
endif

AM_CXXFLAGS = -O3 -ansi -pedantic-errors -Werror -DQDNG_BUILD_DATE="\"`date`\"" -DQDNG_BUILD_HOST="\"`uname -n`\"" \
	      -DQDNG_BUILD_MACH="\"`uname -m`\"" -DQDNG_REV="\"`git rev-list --max-count=1 HEAD`\""

if HAVE_OPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LDFLAGS = $(OPENMP_CXXFLAGS)
endif

SUBDIRS = fft linalg math modules qdlib slatec tools fftw lib $(OPT_DIRS)

bin_PROGRAMS = qdng qdz
qdng_SOURCES = GlobalOpList.cpp ChainLoader.cpp FiltersList.cpp ProgEigen.cpp \
	ProgOCT.cpp ProgPropa.cpp Reporter.cpp qdng.cpp
qdng_LDADD =  $(top_builddir)/src/lib/libqdng.la 
qdng_LDFLAGS = -Wl,-export-dynamic 

qdz_SOURCES = qdz.cpp
qdz_LDADD = $(top_builddir)/src/lib/libqdng.la

noinst_HEADERS = GlobalOpList.h ChainLoader.h FiltersList.h ProgAutocorr.h \
	ProgEigen.h ProgPropa.h Reporter.h ProgOCT.h



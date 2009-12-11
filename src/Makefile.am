
# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src $(LIBXML2_CFLAGS) $(all_includes)

# the library search path.

OPT_DIRS=

if CPPUNIT
  OPT_DIRS+=tests
endif

SUBDIRS = $(OPT_DIRS) fft linalg math modules qdlib slatec tools lib mex 


bin_PROGRAMS = qdng
qdng_SOURCES = ChainLoader.cpp FiltersList.cpp ProgPropa.cpp Reporter.cpp \
	qdng.cpp ProgEigen.cpp ProgOCT.cpp
qdng_LDADD = $(top_builddir)/src/lib/libqdng.la 
noinst_HEADERS = ChainLoader.h FiltersList.h ProgAutocorr.h ProgEigen.h \
		ProgPropa.h Reporter.h 
AM_CXXFLAGS = -O3 -ansi -pedantic-errors -Werror -DQDNG_BUILD_DATE="\"`date`\"" -DQDNG_BUILD_HOST="\"`uname -n`\"" \
	      -DQDNG_BUILD_MACH="\"`uname -m`\"" -DQDNG_REV="\"`git rev-list --max-count=1 HEAD`\""

#qdng_LDFLAGS = -all-static

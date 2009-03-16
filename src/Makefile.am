
# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src $(LIBXML2_CFLAGS) $(all_includes)

# the library search path.

SUBDIRS = math tools fft qdlib modules slatec

noinst_LTLIBRARIES = libqdng.la
libqdng_la_SOURCES = libqdng.cpp 
libqdng_la_LDFLAGS = -module
libqdng_la_LIBADD = $(top_builddir)/src/tools/libtools.la \
	$(top_builddir)/src/qdlib/libqd.la \
	$(top_builddir)/src/modules/libModuleLoader.a \
	$(top_builddir)/src/math/libmath.la $(top_builddir)/src/fft/libfft.la

bin_PROGRAMS = qdng
qdng_SOURCES = ChainLoader.cpp FiltersList.cpp ProgPropa.cpp Reporter.cpp \
	qdng.cpp ProgEigen.cpp
qdng_LDADD = $(top_builddir)/src/modules/libModuleLoader.a \
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/tools/libtools.la $(top_builddir)/src/math/libmath.la \
	$(top_builddir)/src/slatec/libslatecmin.a $(top_builddir)/src/fft/libfft.la \
	$(LAPACK_LIBS) $(FLIBS) $(FFTW_LIBS) $(LIBXML2_LIBS)
noinst_HEADERS = ChainLoader.h FiltersList.h ProgAutocorr.h ProgEigen.h \
	ProgPropa.h Reporter.h
AM_CXXFLAGS = -O3 -ansi -pedantic-errors


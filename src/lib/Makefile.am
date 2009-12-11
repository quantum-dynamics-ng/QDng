INCLUDES = $(LIBXML2_CFLAGS) $(all_includes) -I$(top_srcdir)/src
METASOURCES = AUTO
AM_CXXFLAGS = -ansi -O3 -fPIC -pedantic -Werror
noinst_LTLIBRARIES = libqdng.la
libqdng_la_SOURCES = libqdng.cpp
libqdng_la_LIBADD = $(top_builddir)/src/tools/libtools.la \
	$(top_builddir)/src/slatec/libslatecmin.la $(top_builddir)/src/qdlib/libqd.la \
	$(top_builddir)/src/modules/libModuleLoader.la $(top_builddir)/src/math/libmath.la \
	$(top_builddir)/src/linalg/liblinalg.la $(top_builddir)/src/fft/libfft.la $(FFTW_LIBS) $(FLIBS) \
	$(LAPACK_LIBS) $(LIBXML2_LIBS)
libqdng_la_LDFLAGS = -all-static
#libqdng_la_LDFLAGS = -Wl,-static
#-Wl,--whole-archive 

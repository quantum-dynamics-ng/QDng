INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 -I$(top_srcdir)/src/fftw/api $(all_includes)
METASOURCES = AUTO



#pkglib_LTLIBRARIES = libqd-OSPO.la libqd-OSOD.la libqd-OGridNablaSq.la \
#	libqd-OGridPotential.la libqdWFGridCartesian.la
#lib_LTLIBRARIES = qdOGridNablaSq.la
#OGridNablaSq_la_SOURCES = mod-OGridNablaSq.cpp
#OGridNablaSq_la_LDFLAGS = -module


AM_CXXFLAGS = -O3 -g -fPIC
#libqd_OGridNablaSq_la_LIBADD = $(top_builddir)/src/tools/libtools.la \
#	$(top_builddir)/src/slatec/libslatecmin.a $(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/math/libmath.la \
#	$(top_builddir)/src/fft/libfft.la $(LAPACK_LIBS)
noinst_LTLIBRARIES = libModuleLoader.la
libModuleLoader_la_SOURCES = ModuleLoader.cpp InternalMods.cpp

noinst_HEADERS = ModuleLoader.h Module.h InternalMods.h
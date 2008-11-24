INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 $(all_includes)
METASOURCES = AUTO
noinst_LIBRARIES = libModuleLoader.a

libModuleLoader_a_SOURCES = ModuleLoader.cpp


libqd_OCheby_la_LDFLAGS = -module
pkglib_LTLIBRARIES = libqd-OCheby.la libqd-OSPO.la libqd-OSOD.la \
	libqd-OGridNablaSq.la libqd-OGridPotential.la libqdWFGridCartesian.la
libqd_OSPO_la_LDFLAGS = -module
libqd_OSOD_la_LDFLAGS = -module
libqd_OGridNablaSq_la_LDFLAGS = -module
libqdWFGridCartesian_la_LDFLAGS = -module
libqd_OGridPotential_la_LDFLAGS = -module
libqd_OCheby_la_SOURCES = mod-OCheby.cpp
libqd_OCheby_la_LIBADD = $(top_builddir)/src/sys/libtools.la \
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/math/libmath.la $(top_builddir)/src/fft/libfft.la \
	-lg2c -lblas -lslatec -llapack
libqd_OGridNablaSq_la_SOURCES = mod-OGridNablaSq.cpp
libqd_OGridPotential_la_SOURCES = mod-OGridPotential.cpp
libqd_OSPO_la_SOURCES = mod-OSPO.cpp
libqd_OSOD_la_SOURCES = mod-OSOD.cpp
libqdWFGridCartesian_la_SOURCES = mod-WFGridCartesian.cpp

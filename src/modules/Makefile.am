INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 $(all_includes)
METASOURCES = AUTO
noinst_LIBRARIES = libModuleLoader.a

libModuleLoader_a_SOURCES = ModuleLoader.cpp

pkglib_LTLIBRARIES = modwfgrid.la
modwfgrid_la_SOURCES = Mod_WFGrid1D.cpp
modwfgrid_la_LDFLAGS = -module 
modwfgrid_la_LIBADD = $(top_builddir)/src/qdlib/libqd.la \
	$(top_builddir)/src/fft/libfft.la \
	$(top_builddir)/src/sys/libtools.la \
	$(top_builddir)/src/math/libmath.la -lfftw3 -lxml2


# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 $(all_includes)

# the library search path.

SUBDIRS = math sys fft qdlib modules

noinst_PROGRAMS = test_WFGrid test_WFLevel test_xml test_SPO test_modules
test_WFLevel_SOURCES = test_WFLevel.cpp
test_WFLevel_LDADD = $(top_builddir)/src/math/libmath.la \
	$(top_builddir)/src/fft/libfft.la $(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/sys/libtools.la
test_WFGrid_SOURCES = test_WFGrid.cpp
test_xml_SOURCES = test_xml.cpp
test_xml_LDADD = $(top_builddir)/src/sys/libtools.la -lxml2 -lz -lm
test_WFGrid_LDADD = $(top_builddir)/src/sys/libtools.la \
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/math/libmath.la $(top_builddir)/src/fft/libfft.la \
	-lfftw3
AM_CFLAGS = -Werror -pedantic -pedantic-errors
AM_CXXFLAGS = -Werror -pedantic -pedantic-errors -ansi
test_SPO_LDADD = $(top_builddir)/src/sys/libtools.la\
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/math/libmath.la\
	$(top_builddir)/src/fft/libfft.la -lfftw3
test_SPO_SOURCES = test_SPO.cpp
test_modules_SOURCES = test_modules.cpp
test_modules_LDADD = $(top_builddir)/src/sys/libtools.la \
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/math/libmath.la \
	$(top_builddir)/src/modules/libModuleLoader.a -ldl

noinst_LTLIBRARIES = libqdng.la
libqdng_la_SOURCES = libqdng.cpp 
libqdng_la_LDFLAGS = -module
libqdng_la_LIBADD = $(top_builddir)/src/sys/libtools.la \
	$(top_builddir)/src/qdlib/libqd.la \
	$(top_builddir)/src/modules/libModuleLoader.a \
	$(top_builddir)/src/math/libmath.la $(top_builddir)/src/fft/libfft.la


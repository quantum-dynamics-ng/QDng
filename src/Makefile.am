
# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 $(all_includes)

# the library search path.




noinst_PROGRAMS = test_WFGrid test_WFLevel test_xml test_SPO
test_WFLevel_SOURCES = test_WFLevel.cpp
test_WFLevel_LDADD = $(top_builddir)/src/math/libmath.la \
	$(top_builddir)/src/fft/libfft.la $(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/sys/libtools.la
SUBDIRS = math sys fft qdlib modules
test_WFGrid_SOURCES = test_WFGrid.cpp
test_xml_SOURCES = test_xml.cpp
test_xml_LDADD = $(top_builddir)/src/sys/libtools.la -lxml2 -lz -lm
test_WFGrid_LDADD = $(top_builddir)/src/sys/libtools.la \
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/math/libmath.la $(top_builddir)/src/fft/libfft.la \
	-lfftw3
AM_CFLAGS = -Werror -pedantic -pedantic-errors
AM_CXXFLAGS = -Werror -pedantic -pedantic-errors
test_SPO_LDADD = $(top_builddir)/src/sys/libtools.la\
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/math/libmath.la\
	$(top_builddir)/src/fft/libfft.la -lfftw3
test_SPO_SOURCES = test_SPO.cpp

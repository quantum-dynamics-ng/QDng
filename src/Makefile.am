
# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src $(all_includes)

# the library search path.




noinst_PROGRAMS = test_WFLevel
test_WFLevel_SOURCES = test_WFLevel.cpp
test_WFLevel_LDADD = $(top_builddir)/src/math/libmath.la \
	$(top_builddir)/src/fft/libfft.la $(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/sys/libtools.la
SUBDIRS = qdlib sys fft math

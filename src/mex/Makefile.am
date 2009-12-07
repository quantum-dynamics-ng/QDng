INCLUDES = $(LIBXML2_CFLAGS) $(all_includes) -I$(top_srcdir)/src -I$(MATLAB_DIR)/extern/include
EXEEXT = .mexa64
METASOURCES = AUTO
CXXFLAGS  = -g

#libexec_PROGRAMS = readwf
#readwf_SOURCES = test_read_wf.cpp
#readwf_CXXFLAGS  = $(MATLAB_FLAGS)
#readwf_LDADD = $(top_builddir)/src/fft/libfft.la \
#	$(top_builddir)/src/math/libmath.la $(top_builddir)/src/modules/libModuleLoader.a \
#	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/slatec/libslatecmin.a \
#	$(top_builddir)/src/tools/libtools.la $(MATLAB_LIB)
#readwf_LDFLAGS = $(MATLAB_LINK)

#bin_PROGRAMS = mextest
#mextest_CXXFLAGS  = $(MATLAB_FLAGS)
#mextest_SOURCES = mextest.cpp
#mextest_LDFLAGS = $(MATLAB_LINK)
#mextest_LDADD = $(MATLAB_LIB)

lib_LTLIBRARIES = libmextest2.la readwf.la
libmextest2_la_CXXFLAGS  = $(MATLAB_FLAGS)
libmextest2_la_LDFLAGS = -module $(MATLAB_LINK) -o mextest2.mexa64
libmextest2_la_LIBADD = $(MATLAB_LIB)
libmextest2_la_SOURCES = mextest.cpp

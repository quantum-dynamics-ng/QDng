INCLUDES = $(LIBXML2_CFLAGS) $(all_includes) -I$(top_srcdir)/src -I$(MATLAB_DIR)/extern/include
EXEEXT = .mexa64
METASOURCES = AUTO

libexec_PROGRAMS = readwf
readwf_SOURCES = test_read_wf.cpp
readwf_CXXFLAGS  = $(MATLAB_FLAGS)
readwf_LDADD = $(top_builddir)/src/fft/libfft.la \
	$(top_builddir)/src/math/libmath.la $(top_builddir)/src/modules/libModuleLoader.a \
	$(top_builddir)/src/qdlib/libqd.la $(top_builddir)/src/slatec/libslatecmin.a \
	$(top_builddir)/src/tools/libtools.la $(MATLAB_LIB)
readwf_LDFLAGS = $(MATLAB_LINK)
bin_PROGRAMS = mextest
mextest_SOURCES = mextest.cpp
mextest_LDFLAGS = $(MATLAB_LINK)
mextest_LDADD = $(MATLAB_LIB)

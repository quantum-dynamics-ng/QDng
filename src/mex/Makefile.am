INCLUDES = $(LIBXML2_CFLAGS) $(all_includes) -I$(top_srcdir)/src -I$(MATLAB_DIR)/extern/include
#EXEEXT = .mexa64
METASOURCES = AUTO
#CXXFLAGS  = -g

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

pkglib_LTLIBRARIES = libObjectHandle.la libget_data_wf_from_handle.la \
	libint_wf_file.la
#	libmextest2.la libreadwfstatic.la readwf.la

# libmextest2_la_CXXFLAGS  = $(MATLAB_FLAGS)
# libmextest2_la_LDFLAGS = -module -avoid-version -shrext=.mexa64 $(MATLAB_LINK)
# libmextest2_la_LIBADD = $(MATLAB_LIB)
# libmextest2_la_SOURCES = mextest.cpp
# 
# readwf_la_CXXFLAGS  = $(MATLAB_FLAGS)
# readwf_la_LDFLAGS = -module -avoid-version -shrext=".mexa64"  $(MATLAB_LINK) 
# readwf_la_SOURCES = readwf.cpp
# readwf_la_LIBADD = $(top_builddir)/src/fft/libfft.la \
# 	$(top_builddir)/src/linalg/liblinalg.la $(top_builddir)/src/math/libmath.la \
# 	$(top_builddir)/src/modules/libModuleLoader.la $(top_builddir)/src/qdlib/libqd.la \
# 	$(top_builddir)/src/slatec/libslatecmin.la $(top_builddir)/src/tools/libtools.la $(MATLAB_LIB)
# 
# 
# bin_PROGRAMS = debugmex
# debugmex_SOURCES = debugreadwf.cpp init_wf_file.cpp
# debugmex_CXXFLAGS = -I${MATLAB_DIR}/extern/include -I${MATLAB_DIR}/simulink/include
# debugmex_LDFLAGS =  -L/opt/adm/matlab7.9/bin/glnxa64 \
# 	-Wl,-rpath-link,/opt/adm/matlab7.9/bin/glnxa64 -L$(top_builddir)/src/mex/.libs/ -lObjectHandle 
# debugmex_LDADD = $(top_builddir)/src/lib/libqdng.la -lm -lmat -lmex -lmx \
#         $(LAPACK_LIBS) $(FLIBS) $(LIBXML2_LIBS) -L$(top_builddir)/src/mex/.libs/ -lObjectHandle 
 
# 
# libreadwfstatic_la_SOURCES = convert_to_mxArray.cpp readwf.cpp
# libreadwfstatic_la_CXXFLAGS  = $(MATLAB_FLAGS)
# libreadwfstatic_la_LDFLAGS = -module -avoid-version -shrext=.mexa64 $(MATLAB_LINK) -static
# libreadwfstatic_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(MATLAB_LIB)
# noinst_HEADERS = convert_to_mxArray.h

libint_wf_file_la_SOURCES =  init_wf_file.cpp
libint_wf_file_la_CXXFLAGS  = $(MATLAB_FLAGS)
libint_wf_file_la_LDFLAGS = -module -avoid-version -shrext=.mexa64 $(MATLAB_LINK)
libint_wf_file_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libObjectHandle.la $(MATLAB_LIB)

libget_data_wf_from_handle_la_SOURCES =  get_data_wf_from_handle.cpp
libget_data_wf_from_handle_la_CXXFLAGS  = $(MATLAB_FLAGS)
libget_data_wf_from_handle_la_LDFLAGS = -module -avoid-version -shrext=.mexa64 $(MATLAB_LINK)
libget_data_wf_from_handle_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libObjectHandle.la $(MATLAB_LIB)

libObjectHandle_la_SOURCES = Collector.cpp ObjectHandle.cpp convert_to_mxArray.cpp\
noinst_HEADERS = Collector.h convert_to_mxArray.h convert_to_mxArray.h
libObjectHandle_la_LDFLAGS = -module

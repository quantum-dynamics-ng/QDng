INCLUDES = $(LIBXML2_CFLAGS) $(all_includes) -I$(top_srcdir)/src -I$(MATLAB_DIR)/extern/include
#EXEEXT = .mexa64
METASOURCES = AUTO
CLEANFILES = *.mexa64
SUBDIRS = libs
SUFFIXES = .mexa64



all-local:
	echo "cp"
	cp $(top_srcdir)/src/mex/*.m .


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

pkglib_LTLIBRARIES = libget_wf_handle.la \
	libinit_wf_file.la libwrite_wf_file.la libdelete_wf_handle.la \
	libinit_wf_struct.la libwf_mod.la libget_all_wf_handle.la
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


libinit_wf_file_la_SOURCES =  init_wf_file.cpp  
libinit_wf_file_la_CXXFLAGS  = $(MATLAB_FLAGS)
#libinit_wf_file_la_LDFLAGS = -module -avoid-version -shrext .mexa64 $(MATLAB_LINK)
libinit_wf_file_la_LDFLAGS = -module -Wl,-oinit_wf_file.mexa64 $(MATLAB_LINK)
libinit_wf_file_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libs/libObjectHandle.la $(MATLAB_LIB)

libget_wf_handle_la_SOURCES =  get_wf_handle.cpp
libget_wf_handle_la_CXXFLAGS  = $(MATLAB_FLAGS)
libget_wf_handle_la_LDFLAGS = -module -Wl,-oget_wf_handle.mexa64 $(MATLAB_LINK)
libget_wf_handle_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libs/libObjectHandle.la $(MATLAB_LIB)

libwrite_wf_file_la_SOURCES =  write_wf_file.cpp  
libwrite_wf_file_la_CXXFLAGS  = $(MATLAB_FLAGS)
libwrite_wf_file_la_LDFLAGS = -module -Wl,-owrite_wf_file.mexa64 $(MATLAB_LINK)
libwrite_wf_file_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libs/libObjectHandle.la $(MATLAB_LIB)

libdelete_wf_handle_la_SOURCES = delete_wf_handle.cpp
libdelete_wf_handle_la_CXXFLAGS  = $(MATLAB_FLAGS)
libdelete_wf_handle_la_LDFLAGS = -module -Wl,-odelete_wf_handle.mexa64 $(MATLAB_LINK)
libdelete_wf_handle_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libs/libObjectHandle.la $(MATLAB_LIB)

libinit_wf_struct_la_SOURCES = init_wf_struct.cpp
libinit_wf_struct_la_CXXFLAGS  = $(MATLAB_FLAGS)
libinit_wf_struct_la_LDFLAGS = -module -Wl,-oinit_wf_struct.mexa64 $(MATLAB_LINK)
libinit_wf_struct_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libs/libObjectHandle.la $(MATLAB_LIB)

libwf_mod_la_SOURCES = wf_mod.cpp
libwf_mod_la_CXXFLAGS  = $(MATLAB_FLAGS)
libwf_mod_la_LDFLAGS = -Wl,-owf_mod.mexa64 -module  $(MATLAB_LINK)
libwf_mod_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libs/libObjectHandle.la $(MATLAB_LIB)

libget_all_wf_handle_la_SOURCES = get_all_wf_handle.cpp
libget_all_wf_handle_la_CXXFLAGS  = $(MATLAB_FLAGS)
libget_all_wf_handle_la_LDFLAGS = -Wl,-oget_all_wf_handle.mexa64 -module  $(MATLAB_LINK)
libget_all_wf_handle_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(top_builddir)/src/mex/libs/libObjectHandle.la $(MATLAB_LIB)
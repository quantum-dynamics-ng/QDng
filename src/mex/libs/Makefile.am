INCLUDES = $(LIBXML2_CFLAGS) $(all_includes) -I$(top_srcdir)/src -I$(MATLAB_DIR)/extern/include -I$(FFTW_INCLUDE)

METASOURCES = AUTO

AM_CXXFLAGS  = -O1
MEX_INTERFACE_VERSION=1:0:0

pkglib_LTLIBRARIES = libObjectHandle.la 

libObjectHandle_la_LDFLAGS = -module -version-info $(MEX_INTERFACE_VERSION)
libObjectHandle_la_SOURCES =  \
	wf_ObjectHandle_interface.cpp convert_wf_mxArray.cpp \
	modify_wf.cpp op_ObjectHandle_interface.cpp convert_op_mxArray.cpp DataConversion.cpp
libObjectHandle_la_LIBADD = $(top_builddir)/src/lib/libqdng.la $(FLIBS) $(FFTW_LIBS)



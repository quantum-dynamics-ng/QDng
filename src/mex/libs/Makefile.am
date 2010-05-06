INCLUDES = $(LIBXML2_CFLAGS) $(all_includes) -I$(top_srcdir)/src -I$(MATLAB_DIR)/extern/include

METASOURCES = AUTO

AM_CXXFLAGS  = -O1

pkglib_LTLIBRARIES = libObjectHandle.la 

libObjectHandle_la_LDFLAGS = -module
libObjectHandle_la_SOURCES = Collector.cpp ObjectHandle.cpp \
	wf_ObjectHandle_interface.cpp convert_wf_mxArray.cpp \
	modify_wf.cpp op_ObjectHandle_interface.cpp convert_op_mxArray.cpp



INCLUDES = $(LIBXML2_CFLAGS) $(all_includes) -I$(top_srcdir)/src -I$(MATLAB_DIR)/extern/include

METASOURCES = AUTO

pkglib_LTLIBRARIES = libObjectHandle.la 

libObjectHandle_la_LDFLAGS = -module
libObjectHandle_la_SOURCES = Collector.cpp ObjectHandle.cpp \
	wf_ObjectHandle_interface.cpp convert_wf_mxArray.cpp \
	modify_wf.cpp



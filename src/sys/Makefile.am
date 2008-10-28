INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 $(all_includes)
METASOURCES = AUTO
include_HEADERS = Exception.h FileSet.h FileSingleDefs.h FileSingle.h Getopt.h\
	helpers.h KeyValFile.h ParamContainer.h
noinst_LTLIBRARIES = libtools.la
libtools_la_SOURCES = Exception.h	FileSingle.cpp FileSingle.cpp Getopt.cpp \
	KeyValFile.cpp KeyValFile.cpp	ParamContainer.cpp ParamContainer.cpp QDClock.cpp \
	QDGlobalClock.cpp XmlNode.cpp XmlParser.cpp
noinst_HEADERS = QDClock.h QDGlobalClock.h

INCLUDES = -I$(top_srcdir)/src $(LIBXML2_CFLAGS) $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libtools.la
libtools_la_SOURCES = Exception.cpp Getopt.cpp KeyValFile.cpp Logger.cpp \
	ParamContainer.cpp QDClock.cpp QDGlobalClock.cpp XmlNode.cpp XmlParser.cpp fstools.cpp \
	helpers.cpp GlobalParams.cpp
AM_CXXFLAGS = -pedantic -fPIC -g -pedantic-errors -ansi
noinst_HEADERS = Exception.h FileSingleDefs.h FileSingle.h fstools.h Getopt.h \
	GlobalParams.h helpers.h KeyValFile.h Logger.h ParamContainer.h QDClock.h \
	QDGlobalClock.h tnt_io.h XmlNode.h XmlParser.h Collector.h

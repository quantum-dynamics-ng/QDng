INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libtools.la
libtools_la_SOURCES =  Getopt.cpp \
	KeyValFile.cpp ParamContainer.cpp QDClock.cpp \
	QDGlobalClock.cpp XmlNode.cpp XmlParser.cpp
AM_CXXFLAGS = -Werror -pedantic -pedantic-errors -Wall -ansi -O3

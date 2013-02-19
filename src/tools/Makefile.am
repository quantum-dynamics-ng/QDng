INCLUDES = -I$(top_srcdir)/src $(LIBXML2_CFLAGS) $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libtools.la
libtools_la_SOURCES = Exception.cpp Getopt.cpp Logger.cpp \
	ParamContainer.cpp QDClock.cpp QDGlobalClock.cpp XmlNode.cpp XmlParser.cpp fstools.cpp \
	helpers.cpp GlobalParams.cpp PeakFinder.cpp Memory.cpp FIFO.cpp FileSingleHeader.pb.cc
AM_CXXFLAGS = -fPIC $(DEVEL_FLAGS) $(OPT_FLAGS)
toolsdir = $(pkgincludedir)/tools
tools_HEADERS = Exception.h FileSingleDefs.h FileSingle.h fstools.h Getopt.h \
	GlobalParams.h helpers.h Logger.h ParamContainer.h QDClock.h \
	QDGlobalClock.h tnt_io.h XmlNode.h XmlParser.h Collector.h PeakFinder.h TmpFile.h \
	Memory.h Thread.h SharedVar.h FIFO.h FileSingleHeader.pb.h

noinst_DATA = FileSingleHeader.proto

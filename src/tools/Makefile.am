INCLUDES = -I$(top_srcdir)/src $(LIBXML2_CFLAGS) $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libtools.la


# protobuf related stuff
%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^
	
BUILT_SOURCES = FileSingleHeader.pb.cc FileSingleHeader.pb.h

MOSTLYCLEANFILES = FileSingleHeader.pb.cc FileSingleHeader.pb.h

dist_pkgdata_DATA = FileSingleHeader.proto


libtools_la_SOURCES = Exception.cpp Getopt.cpp Logger.cpp \
	ParamContainer.cpp QDClock.cpp QDGlobalClock.cpp XmlNode.cpp XmlParser.cpp fstools.cpp \
	helpers.cpp GlobalParams.cpp PeakFinder.cpp Memory.cpp FIFO.cpp ZCopyStream.cpp

nodist_libtools_la_SOURCES = FileSingleHeader.pb.cc FileSingleHeader.pb.h

AM_CXXFLAGS = -fPIC $(DEVEL_FLAGS) $(OPT_FLAGS) -Wno-long-long
toolsdir = $(pkgincludedir)/tools
tools_HEADERS = Exception.h FileSingleDefs.h FileSingle.h fstools.h Getopt.h \
	GlobalParams.h helpers.h Logger.h ParamContainer.h QDClock.h \
	QDGlobalClock.h tnt_io.h XmlNode.h XmlParser.h Collector.h PeakFinder.h TmpFile.h \
	Memory.h Thread.h SharedVar.h FIFO.h ZCopyStream.h





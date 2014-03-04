INCLUDES = -I$(top_srcdir)/src $(all_includes)
AM_YFLAGS = -d
AM_CXXFLAGS = -Wno-error=sign-compare -Wno-error=unused-function

noinst_LTLIBRARIES = libsimplexml.la
libsimplexml_la_SOURCES = grammar.yy simpleXml.ll Simple2Xml.cpp

noinst_HEADERS = Simple2Xml.h SyntacticTreeModel.h

BUILT_SOURCES = simpleXml.cc grammar.cc

bin_PROGRAMS = qds2qdi

qds2qdi_SOURCES = qds2qdi.cpp
qds2qdi_LDADD=libsimplexml.la $(top_builddir)/src/lib/libqdng.la  

clean-local:
	-rm grammar.cc grammar.h simpleXml.cc


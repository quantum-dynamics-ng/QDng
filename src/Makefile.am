
# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src $(LIBXML2_CFLAGS) -I$(FFTW_INCLUDE) $(all_includes)

# the library search path.

OPT_DIRS=

if CPPUNIT
  OPT_DIRS+=tests
endif

AM_CXXFLAGS += $(DEVEL_FLAGS) $(OPT_FLAGS) $(CXXFLAGS_GLOB) -DQDNG_BUILD_DATE="\"`date`\"" -DQDNG_BUILD_HOST="\"`uname -n`\"" \
	      -DQDNG_BUILD_MACH="\"`uname -m`\"" -DQDNG_REV="\"`git rev-list --max-count=1 HEAD`\""

if HAVEOPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LDFLAGS = $(OPENMP_CXXFLAGS)
endif

SUBDIRS = tools linalg slatec math qdlib fft modules lib simpleXml matlab $(OPT_DIRS)

# protobuf related stuff
%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^

BUILT_SOURCES = IACmds.pb.h IACmds.pb.cc
MOSTLYCLEANFILES = IACmds.pb.cc IACmds.pb.h

dist_pkgdata_DATA = IACmds.proto


bin_PROGRAMS = qdng qdz qdinfo
qdng_SOURCES = CmdHandler.cpp GlobalOpList.cpp GlobalWFList.cpp ChainLoader.cpp FiltersList.cpp ProgEigen.cpp \
	ProgOCT.cpp ProgPropa.cpp Reporter.cpp ProgFilter.cpp ProgCfun.cpp qdng.cpp
qdng_LDADD =  $(top_builddir)/src/lib/libqdng.la $(LAPACK_LIBS) $(BLAS_LIBS) $(FFTW_LIBS) $(top_builddir)/src/simpleXml/libsimplexml.la $(DL_LIBS) -lprotobuf

nodist_qdng_SOURCES = $(builddir)/IACmds.pb.cc $(builddir)/IACmds.pb.h

qdng_LDFLAGS = -Wl,-export-dynamic

qdz_SOURCES = qdz.cpp
qdz_LDADD = $(top_builddir)/src/lib/libqdng.la $(LAPACK_LIBS) $(BLAS_LIBS) $(FFTW_LIBS) $(DL_LIBS) -lprotobuf

qdinfo_SOURCES = qdng_info.cpp
qdinfo_LDADD = $(top_builddir)/src/tools/libtools.la -lprotobuf

noinst_HEADERS = GlobalOpList.h GlobalWFList.h ChainLoader.h FiltersList.h ProgAutocorr.h \
	ProgEigen.h ProgPropa.h Reporter.h ProgOCT.h ProgFilter.h ProgCfun.h CmdHandler.h


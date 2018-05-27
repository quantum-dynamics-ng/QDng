INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 -I$(FFTW_INCLUDE) $(all_includes)
METASOURCES = AUTO

AM_CXXFLAGS += -fPIC $(DEVEL_FLAGS) $(OPT_FLAGS)


noinst_LTLIBRARIES = libModuleLoader.la
libModuleLoader_la_SOURCES = ModuleLoader.cpp InternalMods.cpp

modulesdir = $(pkgincludedir)/modules
modules_HEADERS = ModuleLoader.h Module.h InternalMods.h


if DYNMODS

pkglib_LTLIBRARIES = \
libqd-OCheby.la \
libqd-OFlux.la \
libqd-OGobbler.la \
libqd-OGridDipole.la \
libqd-OGridGMat.la \
libqd-OGridHOFD.la \
libqd-OGridKick.la \
libqd-OGridNabla.la \
libqd-OGridNablaSq.la \
libqd-OGridNAC.la \
libqd-OGridPML.la \
libqd-OGridPosition.la \
libqd-OGridPotential.la \
libqd-OGridRaman.la \
libqd-OGridSum.la \
libqd-OGSPO.la \
libqd-OHermitianMatrix.la \
libqd-OMatDipole.la \
libqd-OMultistate.la \
libqd-ORK4.la \
libqd-OSIL.la


libqd_OCheby_la_SOURCES = mod-OCheby.cpp $(top_builddir)/src/qdlib/OCheby.cpp \
							$(top_builddir)/src/linalg/Bessel.cpp
libqd_OCheby_la_LIBADD = $(top_builddir)/src/slatec/libslatecmin.la
libqd_OCheby_la_LDFLAGS = -module

libqd_OFlux_la_SOURCES = mod-OFlux.cpp $(top_builddir)/src/qdlib/OFlux.cpp
libqd_OFlux_la_LDFLAGS = -module

libqd_OGobbler_la_SOURCES = mod-OGobbler.cpp $(top_builddir)/src/qdlib/OGobbler.cpp
libqd_OGobbler_la_LDFLAGS = -module

libqd_OGridDipole_la_SOURCES = mod-OGridDipole.cpp $(top_builddir)/src/qdlib/OGridDipole.cpp \
								$(top_builddir)/src/qdlib/OGridPotential.cpp
libqd_OGridDipole_la_LDFLAGS = -module

libqd_OGridGMat_la_SOURCES = mod-OGridGMat.cpp $(top_builddir)/src/qdlib/OGridGMat.cpp\
								$(top_builddir)/src/qdlib/Diff.cpp \
								$(top_builddir)/src/qdlib/PML.cpp \
								$(top_builddir)/src/qdlib/OGridPotential.cpp
libqd_OGridGMat_la_LDFLAGS = -module

libqd_OGridHOFD_la_SOURCES = mod-OGridHOFD.cpp $(top_builddir)/src/qdlib/OGridHOFD.cpp
libqd_OGridHOFD_la_LDFLAGS = -module

libqd_OGridKick_la_SOURCES = mod-OGridKick.cpp $(top_builddir)/src/qdlib/OGridKick.cpp
libqd_OGridKick_la_LDFLAGS = -module

libqd_OGridNabla_la_SOURCES = mod-OGridNabla.cpp $(top_builddir)/src/qdlib/OGridNabla.cpp
libqd_OGridNabla_la_LDFLAGS = -module

libqd_OGridNablaSq_la_SOURCES = mod-OGridNablaSq.cpp $(top_builddir)/src/qdlib/OGridNablaSq.cpp \
								$(top_builddir)/src/qdlib/Diff.cpp
libqd_OGridNablaSq_la_LDFLAGS = -module

libqd_OGridNAC_la_SOURCES = mod-OGridNAC.cpp $(top_builddir)/src/qdlib/OGridNAC.cpp
libqd_OGridNAC_la_LDFLAGS = -module

libqd_OGridPML_la_SOURCES = mod-OGridPML.cpp $(top_builddir)/src/qdlib/OGridPML.cpp
libqd_OGridPML_la_LDFLAGS = -module

libqd_OGridPosition_la_SOURCES = mod-OGridPosition.cpp $(top_builddir)/src/qdlib/OGridPosition.cpp
libqd_OGridPosition_la_LDFLAGS = -module

libqd_OGridPotential_la_SOURCES = mod-OGridPotential.cpp $(top_builddir)/src/qdlib/OGridPotential.cpp
libqd_OGridPotential_la_LDFLAGS = -module

libqd_OGridRaman_la_SOURCES = mod-OGridRaman.cpp $(top_builddir)/src/qdlib/OGridRaman.cpp \
								$(top_builddir)/src/qdlib/OGridPotential.cpp
libqd_OGridRaman_la_LDFLAGS = -module

libqd_OGridSum_la_SOURCES = mod-OGridSum.cpp $(top_builddir)/src/qdlib/OGridSum.cpp
libqd_OGridSum_la_LDFLAGS = -module

libqd_OGSPO_la_SOURCES = mod-OGSPO.cpp $(top_builddir)/src/qdlib/OGSPO.cpp
libqd_OGSPO_la_LDFLAGS = -module

libqd_OHermitianMatrix_la_SOURCES = mod-OHermitianMatrix.cpp $(top_builddir)/src/qdlib/OHermitianMatrix.cpp \
									$(top_builddir)/src/qdlib/TransformMat.cpp
libqd_OHermitianMatrix_la_LDFLAGS = -module

libqd_OMatDipole_la_SOURCES = mod-OMatDipole.cpp $(top_builddir)/src/qdlib/OMatDipole.cpp \
								    $(top_builddir)/src/qdlib/TransformMat.cpp \
									$(top_builddir)/src/qdlib/OHermitianMatrix.cpp
libqd_OMatDipole_la_LDFLAGS = -module

libqd_OMultistate_la_SOURCES = mod-OMultistate.cpp $(top_builddir)/src/qdlib/OMultistate.cpp
libqd_OMultistate_la_LDFLAGS = -module

libqd_ORK4_la_SOURCES = mod-ORK4.cpp $(top_builddir)/src/qdlib/ORK4.cpp
libqd_ORK4_la_LDFLAGS = -module

libqd_OSIL_la_SOURCES = mod-OSIL.cpp $(top_builddir)/src/qdlib/OSIL.cpp
libqd_OSIL_la_LDFLAGS = -module


endif

INCLUDES = -I$(top_srcdir)/src -I$(FFTW_INCLUDE) $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libqd.la
libqd_la_SOURCES = GridSystem.cpp Kspace.cpp Laser.cpp \
	OCheby.cpp OGobbler.cpp OGridDipole.cpp OGridGMat.cpp OGridNAC.cpp OGridNablaSq.cpp \
	OGridPosition.cpp OGridPotential.cpp OGridSum.cpp OGridsystem.cpp OHermitianMatrix.cpp \
	OMultistate.cpp OProjection.cpp OGridNabla.cpp \
	OScalar.cpp OSum.cpp OProduct.cpp WFGridCartesian.cpp WFGridSystem.cpp WFLevel.cpp OSIL.cpp\
	WFMultistate.cpp FileWF.cpp TransformFFT.cpp TransformMat.cpp TransformMSD.cpp \
	OGSPO.cpp ODMultistate.cpp OMatDipole.cpp OGridKick.cpp WaveFunction.cpp WFBuffer.cpp OLaser.cpp OList.cpp \
	OFlux.cpp ORK4.cpp OGridRaman.cpp OGridGStokes.cpp OGridStokesAlbrecht.cpp \
	OGridStokes.cpp ONormalize.cpp OGridHOFD.cpp Diff.cpp PML.cpp OGridPML.cpp OArnoldi.cpp
	
qdlibdir = $(pkgincludedir)/qdlib
qdlib_HEADERS = Conversion.h FileWF.h GridSystem.h Kspace.h Laser.h \
	OCheby.h ODMultistate.h ODSpace.h OGobbler.h OGridDipole.h OGridGMat.h OGridKick.h \
	OGridNabla.h OGridNablaSq.h OGridNAC.h OGridPosition.h OGridPotential.h OGridSum.h OGridsystem.h \
	OGSPO.h OHermitianMatrix.h OMatDipole.h OMultistate.h Operator.h OProjection.h \
	OPropagator.h OScalar.h OSIL.h OSum.h  TransformFFT.h Transform.h TransformMat.h \
	TransformMSD.h 	WaveFunction.h WFGridCartesian.h WFGridSystem.h WFLevel.h WFMultistate.h \
	WFBuffer.h OLaser.h OList.h OProduct.h OFlux.h ORK4.h OGridRaman.h \
	OGridStokes.h ONormalize.h OGridGStokes.h OGridStokesAlbrecht.h OGridHOFD.h HOFD.h Diff.h \
	PML.h OGridPML.h OArnoldi.h

noinst_HEADERS = cfd_table_1.h cfd_table_2.h

AM_CXXFLAGS = -fPIC $(DEVEL_FLAGS) $(OPT_FLAGS)
libqd_la_LDFLAGS = -module


if HAVEOPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
endif


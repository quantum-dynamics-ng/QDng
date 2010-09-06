INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/fftw/api $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libqd.la
libqd_la_SOURCES = GridSystem.cpp Kspace.cpp Laser.cpp \
	OCheby.cpp OGobbler.cpp OGridDipole.cpp OGridGMat.cpp OGridNAC.cpp OGridNablaSq.cpp \
	OGridPosition.cpp OGridPotential.cpp OGridSum.cpp OGridsystem.cpp OHermitianMatrix.cpp \
	OMultistate.cpp OProjection.cpp OSPO.cpp OGridNabla.cpp\
	OScalar.cpp OSum.cpp WFGridCartesian.cpp WFGridSystem.cpp WFLevel.cpp OSIL.cpp\
	WFMultistate.cpp FileWF.cpp TransformFFT.cpp TransformMat.cpp TransformMSD.cpp \
	OGSPO.cpp ODMultistate.cpp OMatDipole.cpp OGridKick.cpp WaveFunction.cpp WFBuffer.cpp OLaser.cpp OList.cpp
noinst_HEADERS = Butterworth.h WindowFuncs.h Conversion.h FileWF.h GridSystem.h Kspace.h Laser.h \
	OCheby.h ODMultistate.h ODSpace.h OGobbler.h OGridDipole.h OGridGMat.h OGridKick.h \
	OGridNabla.h OGridNablaSq.h OGridNAC.h OGridPosition.h OGridPotential.h OGridSum.h OGridsystem.h \
	OGSPO.h OHermitianMatrix.h OMatDipole.h OMultistate.h Operator.h OProjection.h \
	OPropagator.h OScalar.h OSPO.h OSIL.h OSum.h  TransformFFT.h Transform.h TransformMat.h \
	TransformMSD.h 	WaveFunction.h WFGridCartesian.h WFGridSystem.h WFLevel.h WFMultistate.h \
	WFBuffer.h OLaser.h OList.h



AM_CXXFLAGS = -pedantic -pedantic-errors -ansi -g -fPIC -O3 -funroll-loops -ffast-math
libqd_la_LDFLAGS = -module


if HAVE_OPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
endif


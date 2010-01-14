INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libqd.la
libqd_la_SOURCES = GridSystem.cpp InternalMods.cpp Kspace.cpp Laser.cpp \
	OCheby.cpp OGobbler.cpp OGridDipole.cpp OGridGMat.cpp OGridNAC.cpp OGridNabla.cpp \
	OGridNablaSq.cpp OGridPosition.cpp OGridPotential.cpp OGridSum.cpp OGridsystem.cpp \
	OMultistate.cpp OProjection.cpp OSPO.cpp OScalar.cpp OSum.cpp WFGridCartesian.cpp \
	WFGridSystem.cpp WFLevel.cpp WFMultistate.cpp WaveFunction.cpp
noinst_HEADERS = Butterworth.h GridSystem.h OCheby.h OGridDipole.h OGridNAC.h \
	OGridNabla.h OGridNablaSq.h OGridPosition.h OGridPotential.h OGridSum.h \
	OGridsystem.h OHerMat.h OMultistate.h OPropagator.h OSOD.h OSPO.h OScalar.h OSum.h \
	Operator.h WFGridCartesian.h WFGridSystem.h WFLevel.h WFMultistate.h \
	WaveFunction.h


AM_CXXFLAGS = -pedantic -pedantic-errors -ansi -g -fPIC -O3 -funroll-loops -ffast-math
libqd_la_LDFLAGS = -module


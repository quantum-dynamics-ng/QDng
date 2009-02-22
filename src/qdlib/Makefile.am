INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libqd.la
libqd_la_SOURCES = GridSystem.cpp InternalMods.cpp Kspace.cpp Laser.cpp \
			OCheby.cpp OGridNablaSq.cpp OGridPotential.cpp OGridSum.cpp OGridsystem.cpp \
			OMultistate.cpp OSPO.cpp OSum.cpp WFGridCartesian.cpp WFGridSystem.cpp WFLevel.cpp \
			WFMultistate.cpp OGridGMat.cpp OGobbler.cpp
noinst_HEADERS = Butterworth.h GridSystem.h OCheby.h \
		OGridDipole.h OGridNablaSq.h OGridPotential.h OGridSum.h OGridsystem.h OHerMat.h \
		OKSpace.h OPropagator.h OSOD.h OSPO.h OSum.h Operator.h \
		WFGridCartesian.h WFGridSystem.h WFLevel.h WFMultistate.h WaveFunction.h WFMultistate.h \
	OMultistate.h


AM_CXXFLAGS = -pedantic -pedantic-errors -ansi -O3 -funroll-loops
libqd_la_LDFLAGS = -module

INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libqd.la
libqd_la_SOURCES = GridSystem.cpp Laser.cpp OCheby.cpp OGobbler.cpp \
		OGrid1Dd2dx.cpp OGridNablaSq.cpp OGridPotential.cpp OGridSum.cpp OGridsystem.cpp \
		OHerMat.cpp OPropagator.cpp OSOD.cpp OSPO.cpp OSum.cpp WFGrid1D.cpp \
		WFGridCartesian.cpp WFGridSystem.cpp WFLevel.cpp WFMultistate.cpp OMultistate.cpp \
	InternalMods.cpp
noinst_HEADERS = Butterworth.h GridSystem.h OCheby.h OGrid1Dd2dx.h \
		OGridDipole.h OGridNablaSq.h OGridPotential.h OGridSum.h OGridsystem.h OHerMat.h \
		OKSpace.h OPropagator.h OSOD.h OSPO.h OSum.h Operator.h WFGrid1D.h \
		WFGridCartesian.h WFGridSystem.h WFLevel.h WFMultistate.h WaveFunction.h WFMultistate.h \
	OMultistate.h


AM_CXXFLAGS = -Werror -pedantic -pedantic-errors -Wall -ansi -O3 -funroll-loops
libqd_la_LDFLAGS = -module

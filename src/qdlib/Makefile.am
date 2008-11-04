INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libqd.la
libqd_la_SOURCES = ModuleLoader.cpp OGobbler.cpp OGrid1Dd2dx.cpp \
	OGridPotential.cpp OGridsystem.cpp OHerMat.cpp OPropagator.cpp OSPO.cpp OSum.cpp \
	WFGrid1D.cpp WFGridSystem.cpp WFLevel.cpp
noinst_HEADERS = ModuleLoader.h OGobbler.h OGrid1Dd2dx.h OGridDipole.h \
	OGridPotential.h OGridsystem.h OHerMat.h OKSpace.h OPropagator.h OSPO.h OSum.h \
	Operator.h WFGrid1D.h WFGridSystem.h WFLevel.h WaveFunction.h
AM_CXXFLAGS = -Werror -pedantic -pedantic-errors -Wall -O3 -march=opteron

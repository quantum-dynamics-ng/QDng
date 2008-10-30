INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libqd.la
libqd_la_SOURCES = OGrid1Dd2dx.cpp OGridsystem.cpp OHerMat.cpp OSum.cpp \
			WFGrid1D.cpp WFGridSystem.cpp WFLevel.cpp OPropagator.cpp OGridPotential.cpp \
	OSPO.cpp
noinst_HEADERS = OGrid1Dd2dx.h OGridDipole.h OGridPotential.h OGridsystem.h \
			OHerMat.h OKSpace.h Operator.h OSum.h WaveFunction.h WFGrid1D.h WFGridSystem.h \
			WFLevel.h OPropagator.h OGridPotential.h OSPO.h
AM_CXXFLAGS = -Werror -pedantic -pedantic-errors -Wall -O3 -march=opteron

INCLUDES = -I$(top_srcdir)/src $(LIBXML2_CFLAGS) $(all_includes)
METASOURCES = AUTO


# noinst_LIBRARIES = liblinalg.a
# liblinalg_a_SOURCES = Bessel.cpp BlasMatVec.cpp LapackDiag.cpp
AM_CXXFLAGS = -fPIC $(DEVEL_FLAGS) $(OPT_FLAGS)
linalgdir = $(pkgincludedir)/linalg
linalg_HEADERS = LapackDiag.h Bessel.h
noinst_LTLIBRARIES = liblinalg.la 
liblinalg_la_SOURCES = Bessel.cpp LapackDiag.cpp

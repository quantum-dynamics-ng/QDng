INCLUDES = -I$(top_srcdir)/src $(LIBXML2_CFLAGS) $(all_includes)
METASOURCES = AUTO


noinst_LIBRARIES = liblinalg.a
liblinalg_a_SOURCES = Bessel.cpp BlasMatVec.cpp LapackDiag.cpp
AM_CXXFLAGS = -Werror -ansi -pedantic-errors -fPIC
noinst_HEADERS = LapackDiag.h
noinst_LTLIBRARIES = liblinalg.la 
liblinalg_la_SOURCES = Bessel.cpp LapackDiag.cpp

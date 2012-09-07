INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libmath.la
libmath_la_SOURCES = dcomplex.cpp math_functions.cpp
AM_CXXFLAGS = $(BLAS_LIBS) $(DEVEL_FLAGS) $(OPT_FLAGS)
mathdir = $(pkgincludedir)/math
math_HEADERS = Matrix.h math_functions.h typedefs.h Vector.h VectorView.h dcomplex.h \
               m128dc.h m256dc.h VectorPrimitives.h \
				codelets/Abs.h \
				codelets/AddElementsCCc.h \
				codelets/AddElementsCCd.h \
				codelets/AddElementsCC.h \
				codelets/AddElementsEqCCC.h \
				codelets/ExpElementsCDc.h \
				codelets/ExpElements.h \
				codelets/ImCC.h \
				codelets/Im.h \
				codelets/MultElementsAddCCc.h \
				codelets/MultElementsAddCCDd.h \
				codelets/MultElementsAddCCd.h \
				codelets/MultElementsAddCCD.h \
				codelets/MultElementsCCDd.h \
				codelets/MultElementsCCd.h \
				codelets/MultElementsCCD.h \
				codelets/MultElementsCc.h \
				codelets/MultElementsCC.h \
				codelets/MultElementsCDc.h \
				codelets/MultElementsCDd.h \
				codelets/MultElementsCd.h \
				codelets/MultElementsCD.h \
				codelets/MultElementsComplexCCDd.h \
				codelets/MultElementsComplexCCd.h \
				codelets/MultElementsComplexCDd.h \
				codelets/MultElementsComplexEqCCDd.h \
				codelets/MultElementsConjugateCCCd.h \
				codelets/MultElementsConjugateCCd.h \
				codelets/MultElementsCopyCCCd.h \
				codelets/MultElementsCopyCCc.h \
				codelets/MultElementsCopyCCC.h \
				codelets/MultElementsCopyCCDd.h \
				codelets/MultElementsCopyCCd.h \
				codelets/MultElementsCopyCCD.h \
				codelets/MultElementsDd.h \
				codelets/ReCC.h \
				codelets/Re.h

if HAVEOPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
endif



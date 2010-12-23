INCLUDES = -I$(top_srcdir)/src -I$(FFTW_INCLUDE) $(FFTW_CFLAGS) $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libfft.la
libfft_la_SOURCES = fft.cpp

fftdir = $(pkgincludedir)/fft
fft_HEADERS = fft.h

AM_CXXFLAGS = $(DEVEL_FLAGS) $(OPT_FLAGS)

if HAVEOPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LDFLAGS = $(OPENMP_CXXFLAGS)
endif
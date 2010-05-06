INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/fftw/api $(FFTW_CFLAGS) $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libfft.la
libfft_la_SOURCES = fft.cpp

AM_CXXFLAGS = -ansi -O3 -fPIC -pedantic -Werror

if HAVE_OPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LDFLAGS = $(OPENMP_CXXFLAGS)
endif
INCLUDES = -I$(top_srcdir)/src -I/usr/include/libxml2 $(all_includes)
METASOURCES = AUTO
noinst_LTLIBRARIES = libdbesj.la libblasbasic.la
libdbesj_la_SOURCES = d1mach.f dasyjy.f dbesj.f djairy.f fdump.f i1mach.f \
	j4save.f xercnt.f xerhlt.f xermsg.f xerprn.f xersve.f xgetua.f
libblasbasic_la_SOURCES = dlamch.f lsame.f
libdbesj_la_LIBADD = $(top_builddir)/src/slatec/libblasbasic.la

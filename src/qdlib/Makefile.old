#
# Makefile
#


CC = gcc
CXX = g++

INCLUDE =  -I..
LIBS = -lm 
DEFS = -DPROJEKT_NAME='"QDng"' -DVERSION_STRING='"0.1"'
#LIBS = -lm -L/opt/adm/intel/cce/10.1.008/lib/ -L/opt/adm/intel/mkl/10.0.1.014/lib/em64t/  -lmkl_lapack -lmkl -lmkl_core

CXXFLAGS = -Wall -O3 -ffast-math -m64 -msse2 -DTNT_UNROLL_LOOPS
CFLAGS =

#CXXFLAGS = -O3 -DTNT_UNROLL_LOOPS  -parallel -vec-report=1 -par-report=1 -Wcheck -Wp64


#
# System Libraries
#

#
# LAPACK
LAPACK_LIB = -Llib -llapack -lblas -lg2c -lgfortran
LAPACK_INC =

#
# FFTW 3
FFTW_LIBS = -lfftw3
FFTW_INC = 

OBJS = WFLevel.o OHerMat.o WFGridSystem.o WFGrid1D.o OGrid1Dd2dx.o OSum.o
OBJ_INC = ../sys/paramcontainer.o ../math/dcomplex.o ../fft/fft.o


.SUFFIXES: .cpp


all: objects test

objects: $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(DEFS) -c $<

link:
	$(CXX) $(CXXFLAGS) $(LIBS) $(INCLUDE) -o $(NAMEBIN) $(LAPACK) $(GAINC)  $(OBJS)  ga_cavity_seq.cpp $(GALIB)
	$(CXX) $(CXXFLAGS) $(LIBS) $(INCLUDE) -o ga_eval_seq $(LAPACK) $(GAINC)  $(OBJS)  ga_eval_seq.cpp $(GALIB)

test:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJ_INC) $(OBJS) $(FFTW_LIBS) -o test_compile test_compile.cpp

clean:
	rm *.o

depend:
	@makedepend -- $(CXXFLAGS) $(INCLUDE) -- $(SRCS) 2> /dev/null



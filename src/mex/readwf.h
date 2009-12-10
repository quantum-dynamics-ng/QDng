#ifndef READWF_H
#define READWF_H

#include "mex.h"

#include <iostream>
#include <math.h>

extern void loadWF(mxArray **out1, const mxArray *in1 , const mxArray *in2);
extern void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] );

#endif
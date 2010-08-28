#ifndef FUNCTION_GENS_H
#define FUNCTION_GENS_H

#include "math/typedefs.h"

using namespace QDLIB;

extern void fgen_sin(cVec &vec, double min, double max);
extern void fgen_sin_norm(cVec &vec, double min, double max);
extern void fgen_cos_norm(cVec &vec, double min, double max);

/**
 * \f$ f(x) = e^{\frac{-(x-x_0)^2}{2*\sigma^2}}\f$ 
 */
template <class C>
void fgen_gauss(C &vec, double min, double max, double x0, double sigma)
{
   double dx = (max - min)/(vec.size()-1);
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = exp( -1*(double(i) * dx + min-x0)*(double(i) * dx + min-x0) / 2. / sigma / sigma );
   }

}

#endif
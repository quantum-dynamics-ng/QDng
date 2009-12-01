#include "function_gens.h"

using namespace QDLIB;


void fgen_sin(cVec &vec, double min, double max)
{
   double dx = (max - min)/(vec.size()-1);
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = sin( (double(i) * dx + min)  * 2 / M_PI);
   }
}

void fgen_sin_norm(cVec &vec, double min, double max)
{
   double dx = (max - min)/(vec.size()-1);
   dcomplex n(0,0);
   double norm;
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = sin( (double(i) * dx + min)  * 2 / M_PI) ;
      n += vec[i].conj() * vec[i];
   }
   
   n *= dx;
   norm = sqrt( n.real() );
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] /= norm;
   }
   
}
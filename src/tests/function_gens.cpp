#include "function_gens.h"

using namespace QDLIB;


void fgen_sin(cVec &vec, double min, double max)
{
   double dx = (max - min)/(vec.size()-1);
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = sin( (double(i) * dx + min)  * 2 / M_PI);
   }
}

/** 
 * Produces normalized, periodic sine
*/
void fgen_sin_norm(cVec &vec, double min, double max)
{
   double dx2 = (max - min)/(vec.size());
   double dx = (max - min)/(vec.size()-1);
   double L = max-min;
   dcomplex n(0,0);
   double norm;
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = sin( (double(i) * dx2 + min)  * 2 * M_PI/L) ;
      n += vec[i].conj() * vec[i];
   }
   
   n *= dx;
   norm = sqrt( n.real() );
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] /= norm;
   }
   
}

/** 
 * Produces normalized, periodic cosine
 */
void fgen_cos_norm(cVec &vec, double min, double max)
{
   double dx2 = (max - min)/(vec.size());
   double dx = (max - min)/(vec.size()-1);
   double L = max-min;
   dcomplex n(0,0);
   double norm;
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = cos( (double(i) * dx2 + min)  * 2 * M_PI/L) ;
      n += vec[i].conj() * vec[i];
   }
   
   n *= dx;
   norm = sqrt( n.real() );
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] /= norm;
   }
   
}
#include "function_gens.h"

using namespace QDLIB;


void fgen_sin(cVec &vec, double min, double max)
{
   double L = max-min;
   double dx = L / (vec.size()-1);
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = sin( (double(i) * dx + min-dx/2)  * 2 * M_PI/(L+dx)) ;
   }
}

void fgen_cos(cVec &vec, double min, double max)
{
   double L = max-min;
   double dx = L / (vec.size()-1);
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = cos( (double(i) * dx + min-dx/2)  * 2 * M_PI/(L+dx)) ;
   }
}


/** 
 * Produces normalized, periodic sine
*/
void fgen_sin_norm(cVec &vec, double min, double max)
{
   double L = max-min;
   double dx = L / (vec.size()-1);
   dcomplex n(0,0);
   double norm;
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = sin( (double(i) * dx + min-dx/2)  * 2 * M_PI/(L+dx)) ;
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
   double L = max-min;
   double dx = L / (vec.size()-1);
   dcomplex n(0,0);
   double norm;
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = cos( (double(i) * dx + min-dx/2)  * 2 * M_PI/(L+dx)) ;
      n += vec[i].conj() * vec[i];
   }
   
   n *= dx;
   norm = sqrt( n.real() );
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] /= norm;
   }
   
}
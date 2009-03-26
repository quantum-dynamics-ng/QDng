#include "OGridHarmonic.h"

namespace QDLIB
{

   OGridHarmonic::OGridHarmonic()
         : OGridPotential()
   {}


   OGridHarmonic::~OGridHarmonic()
   {}


}

void QDLIB::OGridHarmonic::Init( ParamContainer & params )
{
   OGridPotential::Init(params);
   string s;
   
   int i=0;
   char c[256];
   sprintf (c, "%d", i);
   s = string("k") + string(c);
   while ( _params.isPresent(s) && i < GridSystem::Dim()){
      _params.GetValue( string("k") + string(c), _k[i]);
      i++;
      sprintf (c, "%d", i);
   }
      
   /* Initialize grid */
   dVecView view( *((dVec*) this), GridSystem::Dim(), GridSystem::DimSizes());
   
   *((dVec*) this) = 0;
   
   dVec pot;
   double x;
   double a;
   for (int i=0; i < GridSystem::Dim(); i++){   /* initalize the dimensions */
      a = 0.5 * _k[i];
      pot.newsize(GridSystem::DimSizes(i));
      for (int j=0; j < GridSystem::DimSizes(i); j++){
	 x = GridSystem::Xmin(i) + GridSystem::Dx(i) * j;
	 pot[j] = a * x * x;     /* Harmonic oszillator Groundstate */
// 	 cout << j << "  " << pot[j] << endl;
      }
      view.ActiveDim(i);
      view += pot;
   }
}

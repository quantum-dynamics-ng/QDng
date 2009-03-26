#include "WFGridHarmonic.h"


namespace QDLIB
{

   WFGridHarmonic::WFGridHarmonic()
         : WFGridCartesian()
   {}


   WFGridHarmonic::~WFGridHarmonic()
   {}


}

void QDLIB::WFGridHarmonic::Init( ParamContainer & params )
{
   WFGridCartesian::Init(params);
   string s;
   
   int i=0;
   char c[256];
   sprintf (c, "%d", i);
   s = string("mass") + string(c);
   while ( _params.isPresent(s) && i < GridSystem::Dim()){
      _params.GetValue( string("mass") + string(c), _mass[i]);
      _params.GetValue( string("k") + string(c), _k[i]);

      i++;
      sprintf (c, "%d", i);
   }
   
   /* Initialize grid */
   cVecView view( *this, GridSystem::Dim(), GridSystem::DimSizes());
   
   cVec pot;
   double x;
   double a;
   
   *((cVec*) this) =  dcomplex(1,0);
   
   for (int i=0; i < GridSystem::Dim(); i++){   /* initalize the dimensions */
      a = sqrt(_k[i] / _mass[i]) * _mass[i];
      pot.newsize(GridSystem::DimSizes(i));
      for (int j=0; j < GridSystem::DimSizes(i); j++){
	 x = GridSystem::Xmin(i) + GridSystem::Dx(i) * j;
	 pot[j] = pow( a / M_PI, 0.25) * exp(-0.5*a * x*x );     /* Harmonic oszillator Groundstate */

	 
      }
      view.ActiveDim(i);
      view *= pot;
//       for (int i=0; i< this->size(); i++){
// 	 cout << i << "  " << (*this)[i].real() << endl;
//       }
	 
   }
}

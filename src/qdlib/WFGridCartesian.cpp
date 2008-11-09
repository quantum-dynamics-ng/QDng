#include "WFGridCartesian.h"

namespace QDLIB {

WFGridCartesian::WFGridCartesian()
   : WFGridSystem(), _name("WFGridCartesian")
{
}


WFGridCartesian::~WFGridCartesian()
{
   
}


}

WaveFunction * QDLIB::WFGridCartesian::NewInstance()
{
   WFGridCartesian *r =  new WFGridCartesian();
   r->Init(_params);
   return r;
}

WaveFunction * QDLIB::WFGridCartesian::Init(ParamContainer &params)
{
   string s;
   int size = 1;
   int n;
   
   _params = params;
      
   /* Get all the params from the ParamContainer */
   _params.GetValue( "dims", n );
   if ( n > MAX_DIMS ) throw ( EOverflow("More than MAX_DIMS for wave function defined") );
   if ( n <= 0 ) throw ( EParamProblem("Zero number of dimension defined") );
   GridSystem::Dims(n);
  
   int i=0;
   char c[256];
   sprintf (c, "%d", i);
   s = string("n") + string(c);
   while ( _params.isPresent(s) && i < _ndims){
      _params.GetValue( string("n") + string(c), _dims[i]);
      if ( _dims[i] <= 0) throw ( EParamProblem("Zero elements grid defined") );
      
      _params.GetValue( string("xmin") + string(c), d);
      GridSystem::(i, d);
      _params.GetValue( string("xmax") + string(c), d);
      GridSystem::(i, d);
      if ( (GridSystem::Xmax(i) - GridSystem::Xmin(i)) <= 0 )
	 throw ( EParamProblem("Zero length grid defined") );

      i++;
      sprintf (c, "%d", i);
   }
	 
   cVec::newsize(GridSystem::Size());
}

const string & QDLIB::WFGridCartesian::Name()
{
   return _name;
}

double QDLIB::WFGridCartesian::Norm()
{
   dcomplex c(0,0);
   
   c = *this * this;
   
   return c.real();
}

void QDLIB::WFGridCartesian::Normalize()
{
   (*this) * (1/Norm());
}

WaveFunction * QDLIB::WFGridCartesian::operator =(WaveFunction * Psi)
{
   /* WFGridSystem already knows what to copy */
   *((WFGridSystem*) this) = dynamic_cast<WFGridSystem*> (G);
}

dcomplex QDLIB::WFGridCartesian::operator *(WaveFunction *Psi)
{
   dcomplex c(0,0);
   WaveFunction *ket;
   
   /* Copy */
   ket = Psi->NewInstance();
   *ket = Psi;
   
   for(int i=0; i < size(); i++){
      c += ((*this)[i]).conj() * (*Psi)[i];
   }
   
   /* k-space has different Norm! */
   if (isKspace())
   {
      for(int i=0; i < GridSystem::Dims(); i++){
	 c *= 2*PI / (GridSystem::Xmax(i) - GridSystem::Xmin(i)) ;
      }

   } else {
      for(int i=0; i < GridSystem::Dims(); i++)
         c *= GridSystem::Dx(i);
   }
   
   return c;
}


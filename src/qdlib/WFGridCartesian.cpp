#include "WFGridCartesian.h"
#include "tools/Exception.h"

namespace QDLIB {

   WFGridCartesian::WFGridCartesian()
      : WFGridSystem(), _name("WFGridCartesian")
   {
   }
   
   
   WFGridCartesian::~WFGridCartesian()
   {
      
   }

   WaveFunction * WFGridCartesian::NewInstance()
   {
      WFGridCartesian *r =  new WFGridCartesian();
      r->Init(_params);
      return r;
   }

   void WFGridCartesian::Init(ParamContainer &params)
   {
      string s;
      int n;
      double d;
   
      _params = params;
      
      /* Get all the params from the ParamContainer */
      _params.GetValue( "dims", n );
      if ( n > MAX_DIMS ) throw ( EOverflow("More than MAX_DIMS for wave function defined") );
      if ( n <= 0 ) throw ( EParamProblem("Zero number of dimension defined") );
      GridSystem::Dim(n);
  
      int i=0;
      char c[256];
      sprintf (c, "%d", i);
      s = string("N") + string(c);
      while ( _params.isPresent(s) && i < GridSystem::Dim()){
	 _params.GetValue( string("N") + string(c), n);
	 
	 if ( n <= 0) throw ( EParamProblem("Zero elements grid defined") );
	 GridSystem::DimSizes(i, n);
	 _params.GetValue( string("xmin") + string(c), d);
	 GridSystem::Xmin(i, d);
	 _params.GetValue( string("xmax") + string(c), d);
	 GridSystem::Xmax(i, d);
	 if ( (GridSystem::Xmax(i) - GridSystem::Xmin(i)) <= 0 )
	    throw ( EParamProblem("Zero length grid defined") );

	 i++;
	 sprintf (c, "%d", i);
      }
      
      cVec::Align();
      cVec::newsize(GridSystem::Size());
      
      WFGridSystem::Init(_params);
   }

   const string & WFGridCartesian::Name()
   {
      return _name;
   }


   double WFGridCartesian::Norm()
   {
      dcomplex c(0,0);
   
      c = *this * this;
   
      return sqrt(c.real());
   }

   void WFGridCartesian::Normalize()
   {
      MultElements( (cVec*) this, 1/Norm() );
   }

   WaveFunction * WFGridCartesian::operator =(WaveFunction * Psi)
   {
      /* WFGridSystem already knows what to copy */
      *((WFGridSystem*) this) = dynamic_cast<WFGridSystem*> (Psi);
      
      return this;
   }

   dcomplex WFGridCartesian::operator *(WaveFunction *Psi)
   {
      dcomplex c(0,0);
//       WaveFunction *ket;
   
      /* Copy */
//       ket = Psi->NewInstance();
//       *ket = Psi;
   
      /** \todo make mpi-save */
      int s;
      dcomplex *a, *b;
      for (s=0; s < strides(); s++){
	 a = begin(s);
         b = Psi->begin(s);
	 for(int i=0; i < lsize(); i++){
	    c += a[i].conj() * b[i];
	 }
      }
   
      /* k-space has different Norm! */
      if (IsKspace())
      {
	 for(int i=0; i < GridSystem::Dim(); i++){
	    c *= 2*M_PI / (GridSystem::Xmax(i) - GridSystem::Xmin(i)) ;
	 }

      } else {
	 for(int i=0; i < GridSystem::Dim(); i++){
	    c *= GridSystem::Dx(i);
	 }
	 
      }
      
      return c;
   }


}


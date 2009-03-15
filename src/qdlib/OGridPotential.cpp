#include "OGridPotential.h"
#include "WFGridSystem.h"
#include "math/math_functions.h"
#include "tools/Exception.h"
#include <cstdio>

namespace QDLIB
{
   OGridPotential::OGridPotential()
      : OGridSystem(), _name("OGridPotential"), _init(false)
      {}
   
   
   OGridPotential::~OGridPotential()
   {}
   
   
   /* Interface implementation */
   Operator* OGridPotential::NewInstance()
   {
      OGridPotential *O;
      
      O = new OGridPotential();
      
      return O;
   }
	 	 
   void OGridPotential::Init(ParamContainer &params)
   {
      string s;
      int n;
      double d;


      _params = params;
      
      
      _params.GetValue( "dims", n );
      _params.GetValue( "file", s );
      if (n <= 0 && s.size() != 0 && !_init){
	 _init = true;
	 File()->Suffix(BINARY_O_SUFFIX);
	 File()->Name(s);
	 
	 bool set_zero=false;
	 if (_params.isPresent("setzero")) set_zero = true;
	 *( (FileOGrid*) File()) >> (OGridSystem*) this; /* Read potential */
	 
	 /* Put grid minimum to zero */
	 if (set_zero){
	    double min = Emin();
	    for (lint i=0; i < size(); i++){
	       (*this)[i] -= min;
	    }
	 }
	 
	 _init = false;
	 return;
      }
      
      
      if ( n > MAX_DIMS ) throw ( EOverflow("More than MAX_DIMS for Potential defined") );
      GridSystem::Dim(n);
      
      /* Get all the params from the ParamContainer */
      int i=0;
      char c[256];
      sprintf (c, "%d", i);
      s = string("N") + string(c);
      while ( _params.isPresent(s) && i < GridSystem::Dim() ){
	 _params.GetValue( string("N") + string(c), n);
	 if ( n < 1 )
	    throw ( EParamProblem("Zero points grid defined") );
	 GridSystem::DimSizes(i, n);
	 
	 _params.GetValue( string("xmin") + string(c), d);
	 GridSystem::Xmin(i, d);
	 _params.GetValue( string("xmax") + string(c), d);
	 GridSystem::Xmax(i, d);
	 if ( (GridSystem::Xmax(i) - GridSystem::Xmin(i))  <= 0)
	    throw ( EParamProblem("Zero length grid defined") );

	 i++;
	 sprintf (c, "%d", i);
      }
      
      dVec::Align();
      dVec::newsize(GridSystem::Size());
   }
	 
   void OGridPotential::Init( WaveFunction *Psi)
   {
      if( dynamic_cast<WFGridSystem*>(Psi) == NULL)
	 throw ( EIncompatible ("GridPotential can't operate on ", Psi->Name()));
   }
   
   const string& OGridPotential::Name()
   {
      return _name;
   }

   void OGridPotential::UpdateTime(){/* We are not time dependend*/}
   
   dcomplex OGridPotential::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      dcomplex d;
      
      WaveFunction *opket = PsiKet->NewInstance();
      
      Apply(opket, PsiKet);
      d = (*PsiBra) * opket;
      
      delete opket;
      
      return d;
   }
	 
   double OGridPotential::Expec(WaveFunction *Psi)
   {
      return MatrixElement(Psi, Psi).real();
   }
   
   	 
   double OGridPotential::Emax( )
   {
      return VecMax( *((dVec*) this) );
   }

   double OGridPotential::Emin( )
   {
      return VecMin( *((dVec*) this) );
   }
   
   WaveFunction* OGridPotential::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      MultElements((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this);
      return destPsi;
   }
	 
   WaveFunction* OGridPotential::Apply(WaveFunction *Psi)
   {
      MultElements((cVec*) Psi, (dVec*) this);
      return Psi;
   }
	 
   Operator* OGridPotential::operator=(Operator* O)
   {
      Copy(O);
      return this;
   }
	 
   Operator * OGridPotential::Copy(Operator * O)
   {
      OGridPotential *o = dynamic_cast<OGridPotential*> (O);
      if (o == NULL)
	 throw ( EIncompatible("O is not of type OGridPotential", O->Name()) );
      
      _params = o->_params;

      /* Copy Parents */
      OGridSystem::Copy(O);

      return this;
   }
   
   /**
    * This is compatible to dVec and cVec
    */
   Operator* OGridPotential::operator*(Operator* O)
   {
      
//       try {
// 	 dVec *n;
// 	 n = dynamic_cast<dVec*> (O);
// 	 Operator *o = O->NewInstance();
// 	 o = dynamic_cast<dVec*> (o);
// 	 
//       } 
//       catch (bad_cast b) {
// 	 try {
// 	    cVec *n;
// 	    n = dynamic_cast<cVec*> O;
// 	    Operator *o = O->NewInstance();
// 	    o = dynamic_cast<dVec*> o;
// 	    
// 	 } catch(bad_cast b) {
// 	    throw ( EIncompatible("Can't apply operator OGridPotential to other operator")  );
// 	 }
//       }
//       
//       
//       
//       for (int i=0; i < size(); i++){
// 	 (*n)[i] = (*this)[i] * (*o)[i];
//       }
//       return n;
      return NULL;
   }
   
}







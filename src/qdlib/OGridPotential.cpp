#include "OGridPotential.h"
#include "WFGridSystem.h"
#include "math/math_functions.h"
#include "tools/Exception.h"
#include <cstdio>

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridPotential)
	 
   OGridPotential::OGridPotential()
      : OGridSystem(), _name("OGridPotential"), _init(false)
      {}
   
   
   OGridPotential::~OGridPotential()
   {}
   
   
   /* Interface implementation */
	 	 
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
	 
	 bool set_zero;
	 _params.GetValue("setzero", set_zero);
	 *( (FileOGrid*) File()) >> (OGridSystem*) this; /* Read potential */
	 
	 /* Put grid minimum to zero */
	 if (set_zero){
	    double min = Emin().real();
	    for (lint i=0; i < size(); i++){
	       (*this)[i] -= min;
	    }
	 }
	 
	 _init = false;
	 return;
      }
      
      
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
	 GridSystem::DimSize(i, n);
	 
	 _params.GetValue( string("xmin") + string(c), d);
	 GridSystem::Xmin(i, d);
	 _params.GetValue( string("xmax") + string(c), d);
	 GridSystem::Xmax(i, d);
	 if ( (GridSystem::Xmax(i) - GridSystem::Xmin(i))  <= 0)
	    throw ( EParamProblem("Zero length grid defined") );

	 i++;
	 sprintf (c, "%d", i);
      }
      
      dVec::newsize(GridSystem::Size());
   }
	 
   void OGridPotential::Init( WaveFunction *Psi)
   {
      WFGridSystem* psi;
      
      psi=dynamic_cast<WFGridSystem*>(Psi);
      if( psi == NULL)
	 throw ( EIncompatible ("GridPotential can't operate on ", Psi->Name()));
      if ( *((GridSystem*) this) != *((GridSystem*) psi) )
         throw ( EIncompatible ("GridPotential parameters are not compatible with wave function") );
   }

   void OGridPotential::UpdateTime(){/* We are not time dependend*/}
   	 
   dcomplex OGridPotential::Emax( )
   {
      return dcomplex(VecMax( *((dVec*) this) ));
   }

   dcomplex OGridPotential::Emin( )
   {
      return dcomplex(VecMin( *((dVec*) this) ));
   }
   
   void OGridPotential::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      MultElements((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this);
   }
	 
   void OGridPotential::Apply(WaveFunction *Psi)
   {
      MultElements((cVec*) Psi, (dVec*) this);
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
      
}







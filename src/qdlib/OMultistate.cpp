

#include "OMultistate.h"
#include "WFMultistate.h"

namespace QDLIB
{

   OMultistate::OMultistate() : Operator(), Matrix<Operator*>(), _name("OMultistate")
   {}


   OMultistate::~OMultistate()
   {}

   Operator * QDLIB::OMultistate::NewInstance( )
   {
      OMultistate *r = new OMultistate();
   
      return r;
   }

   void QDLIB::OMultistate::Init( ParamContainer & params )
   {
   }

   

   void QDLIB::OMultistate::Init( WaveFunction * Psi )
   {
   } 
   
   const string & OMultistate::Name( )
   {
      return _name;
   }

   dcomplex OMultistate::MatrixElement( WaveFunction * PsiBra, WaveFunction * PsiKet )
   {
      return dcomplex(0,0);
   }

   double OMultistate::Expec( WaveFunction * Psi )
   {
      return 0;
   }

   void OMultistate::UpdateTime()
   {
      for (lint i=0; i < num_rows(); i++){
	 for (lint j=0; j < num_rows(); j++){
	    (*this)[i][j]->UpdateTime();
	 }
      }
   }
   
   double OMultistate::Emax()
   {
      double d=0;
      
      for (lint i=0; i < num_rows(); i++){
	 if ( (*this)[i][i]->Emax() > d) d = (*this)[i][i]->Emax();
      }
      
      return d;
   }
	 
   double OMultistate::Emin()
   {
      double d;
      d = (*this)[0][0]->Emin();
      
      for (lint i=1; i < num_rows(); i++){
	 if ( (*this)[i][i]->Emin() < d) d = (*this)[i][i]->Emin();
      }
      
      return d;
   }
   
   /**
    * \todo implement
    */
   WaveFunction * OMultistate::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {

      return destPsi;
   }

      /**
    * \todo implement
       */

   WaveFunction * OMultistate::Apply( WaveFunction * Psi )
   {
      return Psi;
   }
   

      /**
    * \todo implement
       */

   Operator * OMultistate::operator =( Operator * O )
   {
      return this;
   }

      /**
    * \todo implement
       */

   Operator * OMultistate::operator *( Operator * O )
   {
      return O;
   }

   
   Operator* OMultistate::Offset(const double d)
   {
      lint size = num_rows();
      for (lint i=0; i < size; i++){
	 for (lint j=0; j < size; j++){
	    (*this)[i][j]->Offset(d);
	 }
      }
      return this;
   }

   Operator* OMultistate::Scale(const double d)
   {
      lint size = num_rows();
      for (lint i=0; i < size; i++){
	 for (lint j=0; j < size; j++){
	    (*this)[i][j]->Scale(d);
	 }
      }
      return this;
   }

} /* namespace QDLIB */





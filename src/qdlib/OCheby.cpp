

#include "OCheby.h"

namespace QDLIB
{

   OCheby::OCheby()
      : OPropagator(), _name("OCheby"), _hamilton(NULL),
      _order(0), _coeff(0), Rdelta(0.0), Gmin(0)
   {}


   OCheby::~OCheby()
   {}


   Operator * OCheby::NewInstance( )
   {
      OCheby *r;
      r = new OCheby();
      return r;
   }
   
   void OCheby::Init( ParamContainer & params )
   {
   }

   const string & OCheby::Name( )
   {
      return _name;
   }

   double OCheby::Expec( WaveFunction * Psi )
   {
      return 0; /* Doesn't make sense */
   }

   WaveFunction * OCheby::operator *( WaveFunction * Psi )
   {
   }

   WaveFunction * OCheby::operator *=( WaveFunction * Psi )
   {
   }

   Operator * OCheby::operator =( Operator * O )
   {
   }

   Operator * OCheby::operator *( Operator * O )
   {
   }

   ParamContainer & OCheby::TellNeeds( )
   {
   }

   void OCheby::AddNeeds( string & Key, Operator * O )
   {
   }

   void OCheby::ReInit( )
   {
      
      if (_hamilton == NULL)
	 throw ( EParamProblem("Chebychev Progagator is missing a hamiltonian") );
      
      /* Energy range & offset */
      Rdelta = _hamilton->Emax() - _hamilton->Emin();
      Gmin =  _hamilton->Emin();
      
      /* Recursion order */
      if (order > 0 && order < int(10 * clock->Dt() * Rdelta) )
	 throw ( EParamProblem("Chebychev recursion is to low") );
      
      if (order = 0)
	 order = int(10 * clock->Dt() * Rdelta);
      
      /* Setup coefficients */
      dVec bessel;
      BesselJ0(order, Rdelta * clock->Dt(), bessel);
      
      _coeff.newsize(order);
      _coeff[0] = cexpI(Rdelta + Gmin) * bessel[0];
      for (int i=1; i < _coeff->size(); i++){
	 _coeff[i] = 2.0 * cexpI(Rdelta + Gmin) * bessel[0];
      }

   }

} /* namespace QDLIB */




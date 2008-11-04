#include "OSPO.h"
#include "Exception.h"

namespace QDLIB {

   OSPO::OSPO()
   : OPropagator(), _name("OSPO"), _Tkin(NULL), _Vpot(NULL), _expT(NULL), _expV(NULL),
		 _cV(0,0), _cT(0,0), _last_time(0)
   {
   }
   
   
   OSPO::~OSPO()
   {
      if (expT != NULL) delete _expT;
      if (expV != NULL) delete _expV;
   }
   
   /** Init expT. */
   void OSPO::_InitT( )
   {
      
      for (int i=0; i < Vpot->size(); i++)
      {
	 expT[i] = cexp(Tkin[i] * _cT);
      }
   }
   
   /** Init expV. */
   void OSPO::_InitV( )
   {
      
      for (int i=0; i < Vpot->size(); i++)
      {
	 expV[i] = cexp(Vpot[i] * _cV);
      }
            
   }
      
   void OSPO::ReInit( )
   {
      _cV = OPropagator::Exponent()/2;
      _cT = OPropagator::Exponent();
      
      if (_Tkin == NULL || _Vpot == NULL)
	 throw ( EParamProblem("Tkin or Vpot not initialized") );
      
      if (_Vpot->size() != _Tkin->size())
	 throw ( EParamProblem("Tkin and Vpot differ in size") );
      
      
      /* Init storage for exponentials */
      if (_expV == NULL) {
	 _expV = new cVec(Vpot->size());
      }
      if (_expT == NULL) {
	 _expT = new cVec(Tkin->size());
      }
      
      _InitT();
      _InitV();
      
   }


   
   Operator * OSPO::NewInstance( )
   {
      OSPO *O = new OSPO();
      
      O->Init(_params);
      
      return O;
   }

   void QDLIB::OSPO::Init( ParamContainer & params )
   {
   }
 
   const string& OSPO::Name( )
   {
      return _name;
   }
   
   dcomplex OSPO::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      throw ( EIncompatible("Sorry the SPO can't calculate a matrix element") );
   }

   double OSPO::Expec(WaveFunction *Psi)
   {
      throw ( EIncompatible("Sorry the SPO can't calculate an expectation value") );
   }
   
   WaveFunction* OSPO::operator *( WaveFunction * Psi )
   {
      WaveFunction *ket;
      WFGridSystem *psi;
      
      if (_expT == NULL || _expV == NULL) InitExp();
      if (_last_time != clock->TimeStep() && _Vpot->isTimeDependent()) /* Re-init Vpot if is time dep.*/
      {
	 _last_time = clock->TimeStep;
	 _InitV();
      }
      
      ket = Psi->NewInstance();
      *ket = Psi;       // Copy
      
      psi = dynamic_cast<WFGridSystem*>(ket);
      
      MultElements(psi, _expV);
      
      psi->ToKspace();
      MultElements(psi, _expT);
      psi->ToXspace();
      
      MultElements(psi, _expV);
      
      return psi;
   }

   WaveFunction* OSPO::operator *=(WaveFunction * Psi)
   {
      WFGridSystem *psi;
      
      if (_expT == NULL || _expV == NULL) InitExp();
      if (_last_time != clock->TimeStep() && _Vpot->isTimeDependent()) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep;
	 _InitV();
      }
      
      psi = dynamic_cast<WFGridSystem*>(Psi);
            
      MultElements(psi, _expV);
      
      psi->ToKspace();
      MultElements(psi, _expT);
      psi->ToXspace();
      
      MultElements(psi, _expV);
      
      return psi;
   }
   
   Operator * OSPO::operator =(Operator * O)
   {
      OSPO *org;
      
      org = dynamic_cast<OSPO*>(O);
	    
      *Tkin = org->Tkin;
      *Vpot = org->Vpot;

      *expT = org->expT;
      *expV = org->expV;
      
      *clock = org->clock;
      OPropagator::Exponent(org->Exponent());
   }
   
   Operator * OSPO::operator *(Operator * O)
   {
      throw ( EIncompatible("Can't apply the SPO to another operator") );
   }

}









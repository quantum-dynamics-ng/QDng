

#include "OSPO.h"
#include "sys/Exception.h"
#include "WFGridSystem.h"


namespace QDLIB {

   OSPO::OSPO()
      : OPropagator(), _name("OSPO"), _needs(NULL), _Tkin(NULL), _Tkin_kspace(NULL), _Vpot(NULL),
      _expT(NULL), _expV(NULL), _cV(0,0), _cT(0,0), _last_time(0)
   {
   }
   
   
   OSPO::~OSPO()
   {
      if (_expT != NULL) delete _expT;
      if (_expV != NULL) delete _expV;
      if (_needs != NULL) delete _needs;
   }
   
   
   /**
    * Set the kinetic energy operator.
    */
   void OSPO::AddTkin( OKSpace *T )
   {
      _Tkin = T;
   }

   
   /**
    * Set the potential energy operator.
    */
   void OSPO::AddVpot( OGridSum *V )
   {
      _Vpot = V;
   }
   
   
   /** Init expT. */
   void OSPO::_InitT( )
   {
      _Tkin_kspace = _Tkin->Kspace();
      if (_Tkin_kspace == NULL)
	 throw ( Exception("SPO got uninitialized KSpace") );
      
      for (int i=0; i < _Tkin_kspace->size(); i++)
      {
	 (*_expT)[i] = cexpI( (*_Tkin_kspace)[i] * _cT._imag );
      }
   }
   
   
   /** Init expV. */
   void OSPO::_InitV( )
   {
      _Vpot->UpdateTime();
      for (int i=0; i < _Vpot->size(); i++)
      {
	 (*_expV)[i] = cexpI( (*_Vpot)[i] * _cV._imag);
      }
            
   }
   
   
   void OSPO::ReInit( )
   {
      _cV = OPropagator::Exponent()/2;
      _cT = OPropagator::Exponent();
      
      /* Check if everythings complete */
      if (_Tkin == NULL || _Vpot == NULL)
	 throw ( EParamProblem("Tkin or Vpot not initialized") );
      
      if (_Tkin_kspace == NULL)
	 _Tkin_kspace = _Tkin->Kspace();
	 
      if (_Vpot->size() != _Tkin_kspace->size())
	 throw ( EParamProblem("Tkin and Vpot differ in size") );
      
      
      /* Init storage for exponentials */
      if (_expV == NULL) {
	 _expV = new cVec(_Vpot->size());
	 _expV->newsize(_Vpot->size());
      }
      if (_expT == NULL) {
	 _expT = new cVec(_Tkin_kspace->size());
	 _expT->newsize(_Tkin_kspace->size());
      }
      cout << _cT << " " << _cV << endl;
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
   
   
   void OSPO::UpdateTime( )
   {
      if (_last_time != clock->TimeStep() && _Vpot->isTimeDep()) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep();
	 _InitV();
      }
      
      _Tkin->UpdateTime();
      _Vpot->UpdateTime();
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
      
      if (clock == NULL)
	 throw ( EParamProblem("Propagator has no clock!") );

      
      if (_expT == NULL || _expV == NULL) ReInit();
      if (_last_time != clock->TimeStep() && _Vpot->isTimeDep()) /* Re-init Vpot if is time dep.*/
      {
	 _last_time = clock->TimeStep();
	 _InitV();
      }
      
      ket = Psi->NewInstance();
      *ket = Psi;       // Copy
      
      psi = dynamic_cast<WFGridSystem*>(ket);
      
      MultElements( (cVec*) psi, _expV);
      
      psi->ToKspace();
      MultElements( (cVec*) psi,  _expV, 1/ double(psi->size()) );
      psi->ToXspace();
      
      MultElements( (cVec*) psi, _expV );
      
      return psi;
   }

   
   WaveFunction* OSPO::operator *=(WaveFunction *Psi)
   {
      WFGridSystem *psi;
      
      if (clock == NULL)
	 throw ( EParamProblem("Propagator has no clock!") );
      
      if (_expT == NULL || _expV == NULL) ReInit();
      if (_last_time != clock->TimeStep() && _Vpot->isTimeDep()) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep();
	 _InitV();
      }
      
      psi = dynamic_cast<WFGridSystem*>(Psi);
            
      MultElements( (cVec*) psi, _expV );
      
      psi->ToKspace();
      MultElements( (cVec*) psi,  _expT, 1/ double(psi->size()) );
      psi->ToXspace();
      
      MultElements( (cVec*) psi,  _expV );
   
      return psi;
   }
   
   
   WFGrid1D* OSPO::operator *=(WFGrid1D *Psi)
   {
            
      if (_expT == NULL || _expV == NULL) ReInit();
      if (_last_time != clock->TimeStep() && _Vpot->isTimeDep()) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep();
	 _InitV();
      }
            
      MultElements( (cVec*) Psi, _expV );
      
      Psi->ToKspace();
      MultElements( (cVec*) Psi,  _expT, 1/ double(Psi->size()) );
      Psi->ToXspace();
      
      MultElements( (cVec*) Psi,  _expV );
   
      return Psi;
   }   
   
   
   Operator * OSPO::operator =(Operator * O)
   {
      OSPO *org;
      
      org = dynamic_cast<OSPO*>(O);
	    
      *((Operator*) _Tkin) = (Operator*) org->_Tkin;
      *((Operator*) _Vpot) = (Operator*) org->_Vpot;

      *_expT = *(org->_expT);
      *_expV = *(org->_expV);
      
      *clock = *(org->clock);
      OPropagator::Exponent(org->Exponent());
      
      return this;
   }
   
   
   Operator * OSPO::operator *(Operator * O)
   {
      throw ( EIncompatible("Can't apply the SPO to another operator") );
   }

   
   ParamContainer& OSPO::TellNeeds( )
   {
      
      if (_needs == NULL)
         _needs = new ParamContainer();
      
      _needs->SetValue( "Tkin", "");
      _needs->SetValue( "Vpot", "");
      
      return *_needs;
   }

   
   void OSPO::AddNeeds( string &Key, Operator *O )
   {
      if (Key != "Tkin" || Key != "Vpot" )
	 throw ( EParamProblem("SPO only knows Tkin or Vpot") );
      
      if (Key == "Tkin") {
	 _Tkin = dynamic_cast<OKSpace*>(O);
      }
      
      if (Key == "Vpot") {
	 _Vpot = dynamic_cast<OGridSum*>(O);
      }
      
   }

  
   
   
}














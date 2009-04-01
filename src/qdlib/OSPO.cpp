#include "OSPO.h"
#include "tools/Exception.h"
#include "WFGridSystem.h"
#include "OMultistate.h"


namespace QDLIB {

   OSPO::OSPO() : OPropagator(), _name("OSPO"), _needs(NULL),
                  _Tkin(NULL), _Tkin_kspace(NULL), _Vcoup(NULL),
                  _expT(NULL), _expV(NULL), _expVcoup(NULL), _buf(NULL),
		  _cV(0,0), _cT(0,0), _coupling(false), _last_time(0)
   {
      _Vpot[0] = NULL;
      _Vpot[1] = NULL;
   }
   
   
   OSPO::~OSPO()
   {
      if (_expT != NULL) delete _expT;
      if (_expV != NULL) delete _expV;
      if (_needs != NULL) delete _needs;
      if(_buf != NULL) delete _buf;
   }
   
   
   Operator * OSPO::NewInstance( )
   {
      OSPO *O = new OSPO();
      return O;
   }
   
   /**
    * Set the kinetic energy operator.
    */
   void OSPO::AddTkin( ODSpace *T )
   {
      _Tkin = T;
   }

   
   /**
    * Set the potential energy operator.
    */
   void OSPO::AddVpot( OGridSystem *V )
   {
      _Vpot[0] = V;
   }
   
   
   /** Init expT. */
   void OSPO::_InitT( )
   {
      _Tkin_kspace = _Tkin->Dspace();
      if (_Tkin_kspace == NULL)
	 throw ( Exception("SPO got uninitialized KSpace") );
      
      ExpElements(_expT, _Tkin_kspace, _cT);
   }
   
   
   /** Init expV. */
   void OSPO::_InitV( )
   {
      if (_coupling) { /* Multistate */
	 WFMultistate *expV = dynamic_cast<WFMultistate*>(_expV);
	 WFMultistate *expVcoup = dynamic_cast<WFMultistate*>(_expVcoup);
	 WFMultistate *V1 = dynamic_cast<WFMultistate*>(_V1);
	 
	 /* Gen: V(t) * 1  =  V_t */
	 ((Operator*) (_Vpot[1]))->UpdateTime();
	 _Vpot[0]->Apply(expV->State(0), V1->State(0));
	 ExpElements((cVec*) expV->State (0), (cVec*) expV->State(0), _cV);
	 _Vpot[1]->Apply(expV->State(1), V1->State(1));
	 ExpElements((cVec*) expV->State(1), (cVec*) expV->State(1), _cV);	 
	 /* Coupling - only held once */
	 if (_Vcoup != NULL){
	    ((Operator*) (_Vcoup))->UpdateTime();
	    _Vcoup->Apply(expVcoup->State(0), V1->State(0));
	    _Vcoup->Apply(expVcoup->State(1), V1->State(1));
	    ExpElements((cVec*) expVcoup->State(0), (cVec*) expVcoup->State(0), -1*_cV);
	    ExpElements((cVec*) expVcoup->State(1), (cVec*) expVcoup->State(1), _cV);
	 }
      } else { /* Single state */
	 ((Operator*) (_Vpot[0]))->UpdateTime();
	 _Vpot[0]->Apply(_expV, _V1);
	 ExpElements((cVec*) _expV, (cVec*) _expV, _cV);
      }
   }
   
   
   void OSPO::Init( WaveFunction * Psi)
   {
      if (clock == NULL)
	 throw ( EParamProblem("Propagator has no clock!") );
      
      WFMultistate* psim = dynamic_cast<WFMultistate*>(Psi);
      
      if (psim != NULL){
	 if (psim->States() != 2)
	    throw ( EIncompatible("SPO can only handle 2 states") );
	 WFGridSystem *psig = dynamic_cast<WFGridSystem*>(psim->State(0));
	 if (psig == NULL)
	    throw ( EIncompatible("SPO with two levels needs a GridSystem WF", Psi->Name()) );
      } else {
         WFGridSystem *psig = dynamic_cast<WFGridSystem*>(Psi);
         if (psig == NULL)
	  throw ( EIncompatible("SPO needs a GridSystem WF", Psi->Name()) ); 
      }
            
      if (_coupling) {
         _cV = OPropagator::Exponent();
	 _cT = OPropagator::Exponent()/2;
      } else {
	 _cV = OPropagator::Exponent()/2;
	 _cT = OPropagator::Exponent();
      }
      
      if (_buf == NULL) _buf = Psi->NewInstance();
      
      /* Check if everythings complete */
      if (_Tkin == NULL || _Vpot[0] == NULL)
	 throw ( EParamProblem("Tkin or Vpot not initialized") );
      
      if (_Vpot[1] == NULL && _coupling)
	 throw ( EParamProblem("SPO: Coupling request but only one potential given") );
      
      if (_Tkin_kspace == NULL)
	 _Tkin_kspace = _Tkin->Dspace();
	 
      if (_Vpot[0]->size() != _Tkin_kspace->size()) 
	 throw ( EParamProblem("Tkin and Vpot differ in size") );
      
      if (_coupling)
         if (_Vpot[0]->size() != _Vpot[1]->size())
	    throw ( EParamProblem("Ground and excited state potential differ in size") );
      
      if (_coupling && _Vcoup != NULL)
	 if(_Vcoup->size() != _Tkin_kspace->size())
	     throw ( EParamProblem("Vcoup differs in size") );
      
      /* Init storage for exponentials */
      if (_expT == NULL) {
	 _expT = new cVec(_Tkin_kspace->size());
	 _expT->newsize(_Tkin_kspace->size());
      }
      if (_expV == NULL) {
	 _expV = Psi->NewInstance();
	 _V1 = Psi->NewInstance();
	 *_V1 = 1;
      }
      if (_expVcoup == NULL) {
	 _expVcoup = Psi->NewInstance();
	 _Vcoup1 = Psi->NewInstance();
	 *_Vcoup1 = 1;
      }
	 
      _InitT();
      _InitV();

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
      if (_last_time != clock->TimeStep() && (_Vpot[0]->isTimeDep() || _Vcoup->isTimeDep()) ) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep();
	 _InitV();
	 //if (_coupling) _initVcoup();
      }
      
   }
   
   
   dcomplex OSPO::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      throw ( EIncompatible("Sorry the SPO can't calculate a matrix element") );
   }

   
   double OSPO::Expec(WaveFunction *Psi)
   {
      throw ( EIncompatible("Sorry the SPO can't calculate an expectation value") );
   }
   
   /**
    * \todo implement
    */
   WaveFunction* OSPO::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {      
      return sourcePsi;
   }

   WaveFunction* OSPO::Apply(WaveFunction *Psi)
   {

      if (_last_time != clock->TimeStep() && _isTimedependent) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep();
	 _InitV();
      }
      
      
      
      if (_coupling){ /* With coupling */
	 WFMultistate *psi = dynamic_cast<WFMultistate*>(Psi);
	 WFMultistate *buf = dynamic_cast<WFMultistate*>(_buf);
	 WFMultistate *expVcoup = dynamic_cast<WFMultistate*>(_expVcoup);
	 
	 /* exp(-i/2 T dt) */
	 for (int i=0; i < 2; i++){
	    dynamic_cast<WFGridSystem*>(psi->State(i))->ToKspace(); 
	    MultElements( (cVec*) psi->State(i),  (cVec*) _expT, 1/ double(psi->size()) );
	    dynamic_cast<WFGridSystem*>(psi->State(i))->ToXspace();
	 }
	 
	 /* exp(-i V dt) */
	 MultElements( (cVec*) psi,  (cVec*) _expV);
	 
	 /* exp(-i Vcoup dt) */
	 if (expVcoup != NULL){
	    /* transform psi to Vcoup diag basis */
	    *(buf->State(0)) = psi->State(1); 
	    *(buf->State(0)) -= psi->State(0);
	    *(buf->State(1)) = psi->State(1);
	    *(buf->State(1)) += psi->State(0);
	 
	    MultElements( (cVec*) buf,  (cVec*) expVcoup);
	 
	    /* transform psi back from  Vcoup diag basis */
	    *(psi->State(0)) = buf->State(1); 
	    *(psi->State(0)) -= buf->State(0);
	    *(psi->State(1)) = buf->State(1);
	    *(psi->State(1)) += buf->State(0);
	 }
	 
	 /* exp(-i/2 T dt) */
	 for (int i=0; i < 2; i++){
	    dynamic_cast<WFGridSystem*>(psi->State(i))->ToKspace(); 
	    MultElements( (cVec*) psi->State(i),  (cVec*) _expT, 1/ double(psi->State(i)->size()) );
	    dynamic_cast<WFGridSystem*>(psi->State(i))->ToXspace();
	 }
	 return psi;
      } else { /* Without coupling */
	 WFGridSystem *psi = dynamic_cast<WFGridSystem*>(Psi);
	 /* exp(-i/2 V dt) */
	 MultElements( (cVec*) psi, (cVec*) _expV );
	 
	 /* exp(-i T dt) */
	 psi->ToKspace();
	 MultElements( (cVec*) psi,  (cVec*) _expT, 1/ double(psi->size()) );
	 psi->ToXspace();
	 
	 /* exp(-i/2 V dt) */
	 MultElements( (cVec*) psi,  (cVec*) _expV );
	 return psi;
      }
   }
   
   
   Operator * OSPO::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }
   
   Operator * OSPO::Copy(Operator * O)
   {
      OSPO *org;
      
      org = dynamic_cast<OSPO*>(O);
	    
      if (org == NULL)
	 throw ( EIncompatible("Incompatible Assignment", this->Name(), O->Name()) );
      
      *((Operator*) _Tkin) = (Operator*) org->_Tkin;
      *((Operator*) _Vpot) = (Operator*) org->_Vpot;

      *_expT = *(org->_expT);
      *_expV = *(org->_expV);
      
      *clock = *(org->clock);
      OPropagator::Exponent(org->Exponent());
      
      /* Copy parents */
      OPropagator::Copy(O);
      
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
      _needs->SetValue( "Vcoup", "opt");
      
      return *_needs;
   }

   
   void OSPO::AddNeeds( string &Key, Operator *O )
   {
      if (Key != "Tkin" && Key != "Vpot"  && Key != "Vcoup")
	 throw ( EParamProblem("SPO only knows Tkin, Vpot or Vcoup") );
      
      if (Key == "Tkin") {
	 OMultistate *om = dynamic_cast<OMultistate*>(O);
         if ( om != NULL)
	    _Tkin = dynamic_cast<ODSpace*>(om->State(0,0));
	 else
	    _Tkin = dynamic_cast<ODSpace*>(O);
	 if (_Tkin == NULL)
	    throw ( EParamProblem("Tkin is invalid", O->Name()) );
      }
      
      /* Check for Potentials: 1 or 2 */
      if (Key == "Vpot") {
	 OMultistate *mpot = dynamic_cast<OMultistate*>(O);
	 if (mpot == NULL) {
	    _Vpot[0] = dynamic_cast<OGridSystem*>(O);
	    if (_Vpot[0] == NULL)
	       throw ( EParamProblem("Vpot is invalid", O->Name()) );
	    if (_Vpot[0]->isTimeDep()) _isTimedependent=true;
	 } else {
	    if (mpot->States() > 2)
	       throw ( EParamProblem("SPO only supports two states") );
	
	    _Vpot[0] = dynamic_cast<OGridSystem*>(mpot->State(0,0));
	    if (_Vpot[0] == NULL)
	       throw ( EParamProblem("Vpot is invalid", O->Name()) );
	    if (_Vpot[0]->isTimeDep()) _isTimedependent=true;
	    if (mpot->States() == 2){
	       _Vpot[1] = dynamic_cast<OGridSystem*>(mpot->State(1,1));
	       if (_Vpot[1] == NULL)
		  throw ( EParamProblem("Vpot is invalid", O->Name()) );
	       _coupling = true;
	       if (_Vpot[1]->isTimeDep()) _isTimedependent=true;
	    }
	 }
      }
	  
      if (Key == "Vcoup") {
	 OMultistate *mcoup = dynamic_cast<OMultistate*>(O);
	 if (mcoup == NULL)
	    throw ( EParamProblem("SPO: Expected a Multistate operator for the coupling ", O->Name()) );
	 else {
	    _Vcoup = dynamic_cast<OGridSystem*>(mcoup->State(0,1));
	    if (_Vcoup == NULL)
	       _Vcoup = dynamic_cast<OGridSystem*>(mcoup->State(1,0));
	    if (_Vcoup == NULL)
	       throw ( EParamProblem("Vcoup is invalid ", mcoup->Name()) );
	 }
	 if (_Vcoup->isTimeDep()) _isTimedependent=true;
	 _coupling = true;
      }
      
   }

  
   
   
} /* namespace QDLIB */




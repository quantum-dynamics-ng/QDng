#include "OSPO.h"
#include "tools/Exception.h"
#include "WFGridSystem.h"
#include "OMultistate.h"
#include "math/math_functions.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OSPO)
   
   OSPO::OSPO() : OPropagator(), _name("OSPO"), _needs(NULL),
                  _Tkin(NULL), _Tkin_kspace(NULL), _Vcoup(NULL),
                        _expT(NULL), _expV(NULL), _V1(NULL), _expVcoup(NULL), _expVcoupI(NULL),_buf(NULL), _buf2(NULL),
                        _coupling(false), _coupdiag(false),_cT(0,0), _cV(0,0), _last_time(0)
   {
      _Vpot[0] = NULL;
      _Vpot[1] = NULL;
   }
   
   
   OSPO::~OSPO()
   {
      if (_expT != NULL) delete _expT;
      DELETE_WF(_expV);
      DELETE_WF(_expVcoup);
      DELETE_WF(_V1);
      if (_needs != NULL) delete _needs;
      DELETE_WF(_buf);
      DELETE_WF(_buf2);
      DELETE_OP(_H);
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
	 _Vpot[1]->Apply(expV->State(1), V1->State(1));
         if (_Vcoup == NULL){ /* Exponents for coupling are build directly */
            ExpElements((cVec*) expV->State(0), (cVec*) expV->State(0), _cV);
            ExpElements((cVec*) expV->State(1), (cVec*) expV->State(1), _cV);
         }
	 /* Coupling - only held once */
	 if (_Vcoup != NULL){
	    ((Operator*) (_Vcoup))->UpdateTime();
	    _Vcoup->Apply(expVcoup->State(0), V1->State(0));
	    _Vcoup->Apply(expVcoup->State(1), V1->State(1));
	    //ExpElements((cVec*) expVcoup->State(0), (cVec*) expVcoup->State(0), -1*_cV);
	    //ExpElements((cVec*) expVcoup->State(1), (cVec*) expVcoup->State(1), _cV);
	 }
      } else { /* Single state */
	 ((Operator*) (_Vpot[0]))->UpdateTime();
	 _Vpot[0]->Apply(_expV, _V1);
	 ExpElements((cVec*) _expV, (cVec*) _expV, _cV);
         if (_coupdiag) {
            _VcoupI->UpdateTime();
            _VcoupI->Apply(_expVcoupI,_V1);
            ExpElements((cVec*) _expVcoupI, (cVec*) _expVcoupI, _cV);
         }
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
	 
      _X = _Tkin->Transformation();
      
      if (_Vpot[0]->size() != _Tkin_kspace->size()) 
	 throw ( EParamProblem("Tkin and Vpot differ in size") );
      
      if (_coupling)
         if (_Vpot[0]->size() != _Vpot[1]->size())
	    throw ( EParamProblem("Ground and excited state potential differ in size") );
      
      if (_coupling && _Vcoup != NULL)
	 if(_Vcoup->size() != _Tkin_kspace->size())
	     throw ( EParamProblem("Vcoup differs in size") );
      
      if (_coupdiag && _VcoupI != NULL)
         if(_VcoupI->size() != _Tkin_kspace->size())
            throw ( EParamProblem("VcoupI differs in size") );
      
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
      }
	 
      if (_expVcoupI == NULL && _coupdiag){
         _expVcoupI = Psi->NewInstance();
      }
      _InitT();
      _InitV();
   }
   
   void QDLIB::OSPO::Init( ParamContainer & params )
   {
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
   void OSPO::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {      
   }

   void OSPO::Apply(WaveFunction *Psi)
   {

      if (_last_time != clock->TimeStep() && _isTimedependent) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep();
	 _InitV();
      }
      
      
      
      if (_coupling){ /* With coupling */
	 WFMultistate *psi = dynamic_cast<WFMultistate*>(Psi);
//	 WFMultistate *buf = dynamic_cast<WFMultistate*>(_buf);
	 WFMultistate *expVcoup = dynamic_cast<WFMultistate*>(_expVcoup);
         WFMultistate *expV = dynamic_cast<WFMultistate*>(_expV);
	 
	 /* exp(-i/2 T dt) */
	 for (int i=0; i < 2; i++){
	    _X->Forward(dynamic_cast<WFGridSystem*>(psi->State(i))); 
	    MultElements( (cVec*) psi->State(i),  (cVec*) _expT, 1/ double(psi->State(i)->size()) );
            _X->Backward(dynamic_cast<WFGridSystem*>(psi->State(i)));
	 }
	 	 
	 /* exp(-i Vcoup dt) */
	 if (expVcoup != NULL){
            double eval[2];
            double evec1[2];
            double evec2[2];
            double* evec[2] ={evec1,evec2};
            cVec *V = (cVec*) expV;
            cVec *Vcoup  = (cVec*) expVcoup;
            int size= V->size()/2;
            cVec *wf= (cVec*) psi;
            dcomplex wfb0;
            dcomplex wfb1;
            
            /* Diagonalize V and exponentiate */
            for (int i=0; i < size; i++){
               diag22symm((*V)[i].real(), (*V)[i+size].real(), (*Vcoup)[i].real(), eval, evec);
               wfb0 = cexp(eval[0] * _cV) * (evec[0][0] * (*wf)[i] + evec[1][0] * (*wf)[i+size]);
               wfb1 = cexp(eval[1] * _cV) * (evec[0][1] * (*wf)[i] + evec[1][1] * (*wf)[i+size]);
               (*wf)[i] = evec[0][0] * wfb0 + evec[0][1] * wfb1;
               (*wf)[i+size] = evec[1][0] * wfb0 + evec[1][1] * wfb1;

            }
         } else  
            MultElements( (cVec*) psi,  (cVec*) _expV); /* exp(-i V dt) */
         
            //cout << endl;
            
	 /* exp(-i/2 T dt) */
	 for (int i=0; i < 2; i++){
            _X->Forward(dynamic_cast<WFGridSystem*>(psi->State(i))); 
	    MultElements( (cVec*) psi->State(i),  (cVec*) _expT, 1/ double(psi->State(i)->size()) );
            _X->Backward(dynamic_cast<WFGridSystem*>(psi->State(i)));
	 }
      } else { /* Without coupling */
	 WFGridSystem *psi = dynamic_cast<WFGridSystem*>(Psi);
	 /* exp(-i/2 V dt) */
	 MultElements( (cVec*) psi, (cVec*) _expV );
         if (_coupdiag)
            MultElements( (cVec*) psi, (cVec*) _expVcoupI );
	 
	 /* exp(-i T dt) */
         _X->Forward(psi);
	 MultElements( (cVec*) psi,  (cVec*) _expT, 1/ double(psi->size()) );
         _X->Backward(psi);
	 
	 /* exp(-i/2 V dt) */
	 MultElements( (cVec*) psi,  (cVec*) _expV );
         if (_coupdiag)
            MultElements( (cVec*) psi, (cVec*) _expVcoupI );	
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
      *((Operator*) _Vpot[0]) = (Operator*) org->_Vpot[0];
      *((Operator*) _Vpot[1]) = (Operator*) org->_Vpot[1];

      *_expT = *(org->_expT);
      *_expV = *(org->_expV);
      
      *clock = *(org->clock);
      OPropagator::Exponent(org->Exponent());
      
      /* Copy parents */
      OPropagator::Copy(O);
      
      return this;
   }
      
   ParamContainer& OSPO::TellNeeds( )
   {
      
      if (_needs == NULL)
         _needs = new ParamContainer();
      
      _needs->SetValue( "Tkin", "");
      _needs->SetValue( "Vpot", "");
      _needs->SetValue( "Vcoup", "opt");
      _needs->SetValue( "VcoupI", "opt");
      
      return *_needs;
   }

   
   void OSPO::AddNeeds( string &Key, Operator *O )
   {
      /* Generate sum for mother class */
      if (H == NULL){
         _H = new OSum();
         H = _H;
      }
      
      _H->Add(O);
      
      if (Key != "Tkin" && Key != "Vpot"  && Key != "Vcoup" && Key != "VcoupI")
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
	 if (mpot == NULL) { /* Single state */
	    _Vpot[0] = dynamic_cast<OGridSystem*>(O);
	    if (_Vpot[0] == NULL)
	       throw ( EParamProblem("Vpot is invalid", O->Name()) );
	    if (_Vpot[0]->isTimeDep()) _isTimedependent=true;
	 } else { /* Multistate */
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
      
      /* IR coupling */
      if (Key == "VcoupI") {
         _VcoupI = dynamic_cast<OGridSystem*>(O);
         if (_VcoupI == NULL)
            throw ( EParamProblem("VcoupI is invalid", O->Name()) );
         if (_VcoupI->isTimeDep()) _isTimedependent=true;
         _coupdiag = true;
      }
   }

   bool OSPO::Valid(WaveFunction * Psi)
   {
      bool valid = true;
      
      if ( Psi == NULL ) return false;
      
      if (_coupling) {
         WFMultistate *wfm = dynamic_cast<WFMultistate*>(Psi);
         
         if (wfm == NULL) return false;
         if (wfm->States() != 2) return false;
         
         valid = valid & _Tkin->Valid(wfm->State(0));
         valid = valid & _Vpot[0]->Valid(wfm->State(0));
      } else {
         valid = valid & _Tkin->Valid(Psi);
         valid = valid & _Vpot[0]->Valid(Psi);
      }
      
      return valid;
   }
   
   
} /* namespace QDLIB */






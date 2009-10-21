#include "WFMultistate.h"
#include "tools/Exception.h"

namespace QDLIB
{

   WFMultistate::WFMultistate() : WaveFunction(), _name("WFMultistate"), _nstates(0)
   {
      for (lint i=0; i < QD_MAX_STATES; i++){
	 _states[i] = NULL;
      }
   }


   WFMultistate::~WFMultistate()
   {
      _destroy();
   }

   /**
    * Remove all wfs
    */   
   void WFMultistate::_destroy()
   {
      for (lint i=0; i < _nstates; i++){
	 if (_states[i] != NULL) { 
	    delete _states[i];
	 }
      }
      _nstates = 0;
   }

   /**
    * Add a WaveFunction to the multistate container.
    * 
    * The storage of the added WaveFunction will be linked
    * into the storage of the multistate vector.
    * (Representet as strided vector)
    */
   void WFMultistate::Add( WaveFunction * Psi, lint index)
   {
      if (index+1 >= QD_MAX_STATES )
	 throw( EOverflow( "More states requested than possible (QD_MAX_STATES)") );
      
      if (index+1 > _nstates)
	 _nstates = index+1;
      
       _states[index] = Psi;
      
      /* link it into our space */
      if (Psi->strides() > 1)
	 throw( EOverflow( "WFMultistate can't handle strided vectors") );
     
      if (! cVec::StrideRef(*((cVec*) Psi), 0, index))
	 throw(Exception("Stride Failure"));
   }
      
   lint WFMultistate::States( )
   {
      return _nstates;
   }

   WaveFunction*  WFMultistate::State(lint index)
   {
      return _states[index];
   }
   
   WaveFunction * WFMultistate::NewInstance( )
   {
      WFMultistate *r = new WFMultistate();
      
      r->_nstates=_nstates;
      for (int i=0; i < _nstates; i++){
	 r->_states[i] =  _states[i]->NewInstance();
	 r->StrideRef(*(r->_states[i]), 0, i);
      }

     return r;
   }

   
   void WFMultistate::Init( ParamContainer & params )
   {
      int reqsize;
      WaveFunction *wflast = NULL;
      
      _params = params;
      
      _params.GetValue("states", reqsize);
      
      if (reqsize >= QD_MAX_STATES )
	 throw( EOverflow( "More states requested than possible (QD_MAX_STATES)") );
      
      if (reqsize < _nstates )
	 throw( EParamProblem( "Less states requested than provided!?") );
      
      /* look for empty states */
      for (int i=0; i < _nstates; i++){
	 if (_states[i] != NULL)
	    wflast = _states[i];
	 else if (wflast != NULL){
	    _states[i] = wflast->NewInstance();
	    *((cVec*) _states[i]) = dcomplex(0,0);
	    cVec::StrideRef(*((cVec*) (_states[i])), 0, i);
	 }
      }
      /* same thing again to refill lower states */
      for (int i=0; i < _nstates; i++){
	 if (_states[i] != NULL)
	    wflast = _states[i];
	 else if (wflast != NULL){
	    _states[i] = wflast->NewInstance();
	    *((cVec*) _states[i]) = dcomplex(0,0);
	    cVec::StrideRef(*((cVec*) (_states[i])), 0, i);
	 }
      }
      
      if (wflast == NULL)
	 throw (EParamProblem("Empty multi state wave function"));
      
      /* resize if needed */
      if (_nstates != reqsize){
	 for (int i=_nstates; i < reqsize; i++){
	    _states[i] = wflast->NewInstance();
	    *((cVec*) _states[i]) = dcomplex(0,0);
	    cVec::StrideRef(*((cVec*) (_states[i])), 0, i);
	 }
      }
      _nstates = reqsize;
      
      
      
   }

   const string & WFMultistate::Name( )
   {
      return _name;
   }

   double WFMultistate::Norm( )
   {
      dcomplex d=0;
      
      for(lint i=0; i < _nstates; i++){
	 d += *(_states[i]) * _states[i];
      }
      
      return d.real();
   }

   void WFMultistate::Normalize( )
   {
      double norm=Norm();
      for(lint i=0; i < _nstates; i++){
	 MultElements( (cVec*) _states[i] , 1/sqrt(norm) );
      }
   }

   WaveFunction * WFMultistate::operator =( WaveFunction * Psi )
   {
      WFMultistate *psi = dynamic_cast<WFMultistate*>(Psi);
      
      if (psi == NULL)
	 throw( EIncompatible("Incompatible in assignment", this->Name(), Psi->Name()) ) ;
      
      if (_nstates != psi->_nstates)
	 throw( EIncompatible("Multistate WFs differ in number of states") ) ;
      
     // _destroy(); /* remove old content */
      
      for(lint i=0; i < psi->_nstates; i++){
	 *(_states[i]) = psi->_states[i];
	 //cVec::StrideRef(*(_states[i]), 0, i);
      }
      _params = psi->_params;
      return this;
   }

   dcomplex WFMultistate::operator *( WaveFunction * Psi )
   {
      dcomplex c(0,0);
      WFMultistate *psi;
      
      psi = dynamic_cast<WFMultistate*>(Psi);
      
      if (psi == NULL)
	 throw( EIncompatible("Not a Multistate wave function", Psi->Name()) ) ;
      
      for(lint i=0; i < _nstates; i++){
	 c += *(_states[i]) * psi->_states[i];
      }
      
      return c;
   }
   

   
}

/* namespace QDLIB */


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
	    _states[i] = NULL;
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
   void WFMultistate::Add( WaveFunction * Psi )
   {
      if (_nstates >= QD_MAX_STATES )
	 throw( EOverflow( "More states requested than possible (QD_MAX_STATES)") );
      
      _states[_nstates] = Psi;
      
      /* link it into our space */
      if (Psi->strides() > 1)
	 throw( EOverflow( "WFMultistate can't handle strided vectors") );
     
      if (! cVec::StrideRef(*((cVec*) Psi), 0, _nstates))
	 throw(Exception("Stride Failure"));
      
      _nstates++;
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
      
      for (int i=0; i < _nstates; i++)
	 r->Add(_states[i]->NewInstance());

     return r;
   }

   void WFMultistate::Init( ParamContainer & params )
   {
   }

   const string & WFMultistate::Name( )
   {
      return _name;
   }

   double WFMultistate::Norm( )
   {
      double d=0;
      
      for(lint i=0; i < _nstates; i++){
	 d += _states[i]->Norm();
      }
      
      return d;
   }

   void WFMultistate::Normalize( )
   {
      double norm = Norm();
      for(lint i=0; i < _nstates; i++){
	 *(_states[i]) *= 1/norm;
      }
   }

   WaveFunction * WFMultistate::operator =( WaveFunction * Psi )
   {
      WFMultistate *psi = dynamic_cast<WFMultistate*>(Psi);
      WaveFunction *add;
      
      if (psi == NULL)
	 throw( EIncompatible("Incompatible in assignment", this->Name(), Psi->Name()) ) ;
      
      if (_nstates != psi->_nstates)
	 throw( EIncompatible("Multistate WFs differ in number of states") ) ;
      
     // _destroy(); /* remove old content */
      
      for(lint i=0; i < psi->_nstates; i++){
	 *(_states[i]) = psi->_states[i];
      }
      
      return this;
   }

   dcomplex WFMultistate::operator *( WaveFunction * Psi )
   {
      dcomplex c(0,0);
      
      for(lint i=0; i < _nstates; i++){
	 c += *(_states[i]) * _states[i];
      }
      
      return c;
   }
   

   
} /* namespace QDLIB */


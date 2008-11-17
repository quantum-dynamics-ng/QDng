#include "WFMultistate.h"
#include "sys/Exception.h"

namespace QDLIB
{

   WFMultistate::WFMultistate() : WaveFunction(), _name("WFMultistate"), _size(0)
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
      for (lint i=0; i < _size; i++){
	 if (_states[i] != NULL) { 
	    delete _states[i];
	    _states[i] = NULL;
	 }
      }
      _size = 0;
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
      if (_size >= QD_MAX_STATES )
	 throw( EOverflow( "More states requested than possible (QD_MAX_STATES)") );
      
      _states[_size] = Psi;
      
      /* link it into our space */
      if (Psi->strides() > 1)
	 throw( EOverflow( "WFMultistate can't handle strided vectors") );
     
      cVec::StrideRef(*((cVec*) Psi), 0, _size);
      
      _size++;
   }
   
   lint WFMultistate::Size( )
   {
      return _size;
   }

   WaveFunction*  WFMultistate::State(lint index)
   {
      return _states[index];
   }
   
   WaveFunction * WFMultistate::NewInstance( )
   {
      WFMultistate *r = new WFMultistate();
      
      if (_size > 0){
	 for (int i=0; i < _size; i++)
	    r->Add(_states[i]->NewInstance());
      }
      
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
      
      for(lint i=0; i < _size; i++){
	 d += _states[i]->Norm();
      }
      
      return d;
   }

   void WFMultistate::Normalize( )
   {
      double norm = Norm();
      for(lint i=0; i < _size; i++){
	 *(_states[i]) *= 1/norm;
      }
   }

   WaveFunction * WFMultistate::operator =( WaveFunction * Psi )
   {
      WFMultistate *psi = dynamic_cast<WFMultistate*>(Psi);
      WaveFunction *add;
      
      _destroy(); /* remove old conent */
      
      for(lint i=0; i < psi->_size; i++){
	 add = psi->_states[i]->NewInstance();
	 *add = psi;  /* copy */
	 Add(add);
      }
      
      return this;
   }

   dcomplex WFMultistate::operator *( WaveFunction * Psi )
   {
      dcomplex c(0,0);
      
      for(lint i=0; i < _size; i++){
	 c += *(_states[i]) * _states[i];
      }
      
      return c;
   }
   

   
} /* namespace QDLIB */


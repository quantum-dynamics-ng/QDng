// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "WFMultistate.h"
#include "tools/Exception.h"
#include "tools/ZCopyStream.h"

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
	 DELETE_WF( _states[i] );
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
      
      if (index+1 > _nstates){
	 _nstates = index+1;
         _params.SetValue("states", _nstates);
      }
      
       _states[index] = Psi;
      
      /* link it into our space */
      if (Psi->strides() > 1)
	 throw( EOverflow( "WFMultistate can't handle strided vectors") );
     
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
      
      r->_nstates = _nstates;
#ifdef HAVE_MPI
      r->newsize(0, _nstates, GetComm());
#else
      r->newsize(0, _nstates);
#endif
      for (int i = 0; i < _nstates; i++) {
         r->_states[i] = _states[i]->NewInstance();
         r->StrideRef(*(r->_states[i]), 0, i);
      }
      r->_params = _params;
      return r;
   }

   void WFMultistate::Init( int reqsize )
   {
      
      WaveFunction *wflast = NULL;

      /* look for empty states */
      for (int i=0; i < _nstates; i++){
       if (_states[i] != NULL)
          wflast = _states[i];
       else if (wflast != NULL){
          _states[i] = wflast->NewInstance();
          *((cVec*) _states[i]) = dcomplex(0,0);
       }
      }

      /* same thing again to refill lower states */
      for (int i=0; i < _nstates; i++){
       if (_states[i] != NULL)
          wflast = _states[i];
       else if (wflast != NULL){
          _states[i] = wflast->NewInstance();
          *((cVec*) _states[i]) = dcomplex(0,0);
       }
      }

      if (wflast == NULL)
       throw (EParamProblem("Empty multi state wave function"));
      
      /* resize if needed */
      if (_nstates < reqsize){
       for (int i=_nstates; i < reqsize; i++){
          _states[i] = wflast->NewInstance();
          *((cVec*) _states[i]) = dcomplex(0,0);
       }
         _nstates = reqsize;
      }
      
      /* Link states into our vector space */

#ifdef HAVE_MPI
      cVec::newsize(0, _nstates, &MPI::COMM_WORLD);
#else
      cVec::newsize(0,_nstates);
#endif
      for (int i=0; i < _nstates; i++){
         if (! cVec::StrideRef(*((cVec*) (_states[i])), 0, i))
         throw(Exception("Stride Failure"));
      }
      
      SyncStrides();
   }

   void WFMultistate::Init( ParamContainer & params )
   {
      int reqsize;
      _params = params;

      _params.GetValue("states", reqsize);

      Init(reqsize);

      _params.SetValue("states", _nstates);
   }

   const string & WFMultistate::Name( )
   {
      return _name;
   }

   double WFMultistate::Norm( )
   {
      dcomplex d=0;
      
      for(lint i=MPIrank(); i < _nstates; i += MPIsize()){
         d += *(_states[i]) * _states[i];
      }
      
#ifdef HAVE_MPI
      if (GetComm() != NULL){
         double sum;
         GetComm()->Allreduce(&(d._real), &sum, 1, MPI::DOUBLE, MPI::SUM);
         d._real = sum;
      }
#endif
      return sqrt(d.real());
   }

   void WFMultistate::Normalize( )
   {
      double norm=Norm();
      for(lint i=MPIrank(); i < _nstates; i += MPIsize()){
         MultElements( (cVec*) _states[i] , 1/norm );
      }
   }

   WaveFunction * WFMultistate::operator =( WaveFunction * Psi )
   {
      WFMultistate *psi = dynamic_cast<WFMultistate*>(Psi);
      
      if (psi == NULL)
	 throw( EIncompatible("Incompatible in assignment", this->Name(), Psi->Name()) ) ;
      
      if (_nstates != psi->_nstates)
	 throw( EIncompatible("Multistate WFs differ in number of states") ) ;
      
      
      for(lint i=0; i < psi->_nstates; i++){
	 *(_states[i]) = psi->_states[i];
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
      
      for(lint i=MPIrank(); i < _nstates; i+=MPIsize()){
         c += *(_states[i]) * psi->_states[i];
      }
#ifdef HAVE_MPI
      dcomplex sum(0);
      if (GetComm() != NULL){
         //GetComm()->Allreduce(&c, &sum, 1, MPIdcomplex::Instance(), MPI::SUM);
         GetComm()->Allreduce(&c, &sum, 2, MPI::DOUBLE, MPI::SUM);
         c = sum;
      }
#endif
      return c;
   }
   
   /** \todo This is an ugly hack to treat k-space consistently 
    *  \bug GetSpaceBuffer will fail here!!! */
   void WFMultistate::IsKspace(bool is)
   {
      /* We need a special handling, for strided init of k-space links */
      if (_IsKspace != is){
         dcomplex **strides = cVec::begin();
         for (int i=0; i < _nstates; i++){
            _states[i]->IsKspace(is);
            strides[i] = _states[i]->begin(0); /* Relink strides to spacebuffers of states */
         }
         _IsKspace = is;
      }
   }

   
   WaveFunction* WFMultistate::Reduce(double tolerance)
   {
      throw(EIncompatible("WFMultistate::Reduce should never be called"));
      return NULL;

//      for(lint i=0; i < _nstates; i++){
//         _states[i]->Reduce(tolerance);
//      }
//      IsKspace(true); /* Set our self also to k-space representation */
   }

   void WFMultistate::Restore(WaveFunction*)
   { 
      throw(EIncompatible("WFMultistate::Restore should never be called"));
//      for(lint i=0; i < _nstates; i++){
//         _states[i]->Restore();
//      }
//      IsKspace(false); /* Set our self also to x-space representation */
   }

   void WFMultistate::Retire()
   {
      for(lint i=0; i < _nstates; i++)
         _states[i]->Retire();

      RetireStorage();
   }

   void WFMultistate::Reaquire()
   {
      for(lint i=0; i < _nstates; i++)
         _states[i]->Reaquire();

      ReaquireStorage();
   }


   void WFMultistate::Serialize (std::ostream& os)
   {
      // Just write the number of states here.
      // The rest has to be done by the FileWF class, since we
      // don't own the sub data.
      uint32_t states = States();
      os.write(reinterpret_cast<char*>(&states), sizeof(uint32_t));
   }

   void WFMultistate::DeSerialize (std::istream& is)
   {
      // The info here is useless right now. Ignore it.
      uint32_t states = 0;
      is.read(reinterpret_cast<char*>(&states), sizeof(uint32_t));
   }

}

/* namespace QDLIB */




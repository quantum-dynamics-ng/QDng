// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "OList.h"

namespace QDLIB {

   OList::OList()
      : Operator(), _size(0)
   {
      for(int i=0; i < MAX_OPS; i++){
         _O[i] = NULL;
      }

   }
   
   
   OList::~OList()
   {
   }

   /**
    * Access elements by index.
    * 
    * Note this is read only. For assigning use Add()
    */
   Operator* OList::operator[](int i)
   {
      return Get(i);
   }

   /**
    * Access elements by name.
    *
    * Note this is read only. For assigning use Add()
    */
   Operator* OList::operator[](const string& name)
   {
      return Get(name);
   }

   /**
    * Access elements by index.
    * This is pretty fast.
    */
   Operator * OList::Get(int index)
   {
      if (index >= _size)
         throw( EIncompatible("Access to empty element") );
      return _O[index];
   }
   
   /**
    * Access elements by key.
    * This is done via a lookpup map and thus slower than get-by-index.
    * Should only be used during init stage.
    */
   Operator* OList::Get(const string& name)
   {
      return _O[GetIndex(name)];
   }

   int OList::GetIndex(const string& name)
   {
      if (_NamedEntries.find(name) == _NamedEntries.end())
         throw( EIncompatible("Element not found", name) );

      return _NamedEntries[name];
   }

   bool OList::Exists(const string& name)
   {
      if (_NamedEntries.find(name) == _NamedEntries.end())
         return false;
      else
         return true;
   }

   /**
    * Replace an element.
    */
   void OList::Set(int index, Operator *O)
   {
      if (index >= _size)
         throw( EIncompatible("Access to empty element") );
      
      _O[index] = O;
      _O[index]->Conj(_conj);
   }
   
   void OList::Set(const string& name, Operator *O)
   {
      int index = GetIndex(name);

      _O[index] = O;
      _O[index]->Conj(_conj);
   }

   /**
    * Number of operators.
    */
   int OList::Size() const
   {
      return _size;
   }
   
   /**
    * Add a new, anonymous operator.
    *
    * The added operator will have no name.
    */
   void OList::Add(Operator *O)
   {
      if (_size >= MAX_OPS) {
         throw ( EOverflow("Maximum numbers of operators reached") );
      }
      if (O == NULL){
         throw ( EIncompatible("Operator not initialized") );
      }
      
      if (O->isTimeDep()) _isTimedependent = true;
     
      _O[_size] = O;

      _O[_size]->Conj(_conj);
      if (! O->Hermitian() ) _hermitian = false;

      _size++;
   }


   /**
    * Add a new, named operator.
    */
   void OList::Add(const string& name, Operator *O)
   {
      if ( _NamedEntries.find(name)  != _NamedEntries.end() )
         throw ( EParamProblem("Operator name already exists in list", name) );

      Add(O);

      _NamedEntries[name] = _size-1;
   }
   
   void OList::Clock( QDClock * cl )
   {
      clock = cl;
      for(int i=0; i < _size; i++){
         _O[i]->Clock(clock);
      }
   }
   
   void OList::UpdateTime()
   {
      for(int i=0; i < _size; i++){
         _O[i]->UpdateTime();
      }
   }

   void OList::Conj(bool conj)
   {
      for(int i=0; i < _size; i++){
         _O[i]->Conj(conj);
      }

      _conj = conj;
   }

   void OList::Init( WaveFunction *Psi)
   {
      for(int i=0; i < _size; i++){
         _O[i]->Init(Psi);
      }
   }

   Operator* OList::operator=(Operator* O)
   {
      Copy(O);
      return this;
   }
   
   Operator * OList::Copy(Operator * O)
   {
      OList *r;
      
      r = dynamic_cast<OList*> (O);
      if (r == NULL)
         throw( EIncompatible ("Incompatible in Assignment", this->Name(), O->Name() ) );
      
      _size = r->_size;
      
      for (int i=0; i < _size; i++){
         _O[i] = r->Get(i)->NewInstance();
         *(_O[i]) = r->_O[i];
      }

      return r;
   }
   
   bool OList::Valid(WaveFunction * Psi)
   {
      bool valid = true;
      
      if ( Psi == NULL ) return false;
      
      for (int i=0; i < _size; i++){
         valid = valid & _O[i]->Valid(Psi);
         if ( !valid ) return false;
      }
      return valid;
   }
   
   void OList::RecalcInternals(bool on)
   {
      Operator::RecalcInternals(on);
      for(int i=0; i < _size; i++){
         _O[i]->RecalcInternals(on);
      }
   }
}




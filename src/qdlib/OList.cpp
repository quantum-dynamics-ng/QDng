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
    * Access elements.
    * 
    * Note this is read only. For assingning use Add()
    */
   Operator* OList::operator[](int i)
   {
      return Get(i);
   }
   
   /**
    * Access elements.
    */
   Operator * OList::Get(int index)
   {
      if (index >= _size)
         throw( EIncompatible("Access to empty element") );
      return _O[index];
   }
   
   void QDLIB::OList::Set(int index, Operator *O)
   {
      if (index >= _size)
         throw( EIncompatible("Access to empty element") );
      
      _O[index] = O;
   }
   
   /**
    * Number of operators.
    */
   int OList::Size() const
   {
      return _size;
   }
   
   /**
    * Add a new operator.
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
      _size++;
      
      
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




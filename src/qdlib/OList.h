#ifndef QDLIBOLIST_H
#define QDLIBOLIST_H

#include "qdlib/Operator.h"

#define MAX_OPS 16

namespace QDLIB {

   /**
    * Abstract operator container class for all kind of linear operator sequences.
    */
   class OList : public Operator
   {
      private:
         int _size;
         Operator* _O[MAX_OPS];
      public:
         OList();
         ~OList();
         
         Operator* operator[](int i);
         
         Operator* Get(int index);
         
         void Set(int index, Operator *O);
         
         int Size() const;
         
         void Add(Operator *O);
   
         /* Overide Clock setter */
         virtual void Clock (QDClock *cl);
         
         virtual void UpdateTime();
         
         virtual void Init(ParamContainer &params) {}
         
         virtual void Init( WaveFunction * Psi );
            
         virtual double Expec(WaveFunction *Psi);
         
         virtual Operator* operator=(Operator* O);
         
         virtual Operator* Copy(Operator* O);
         
         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif

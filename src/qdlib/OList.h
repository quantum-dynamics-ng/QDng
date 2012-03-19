#ifndef QDLIBOLIST_H
#define QDLIBOLIST_H

#include "qdlib/Operator.h"
#include <map>

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
         map<string,uint> _NamedEntries; /* holds name<->index mapping*/
      public:
         OList();
         ~OList();
         
         Operator* operator[](int i);
         
         Operator* operator[](const string& name);

         Operator* Get(int index);
         
         Operator* Get(const string& name);

         int GetIndex(const string& name);

         bool Exists(const string& name);

         void Set(int index, Operator *O);
         
         void Set(const string& name, Operator *O);

         int Size() const;
         
         void Add(Operator *O);
         
         void Add(const string& name, Operator *O);

         /* Overide Clock setter */
         virtual void Clock (QDClock *cl);
         
         virtual void UpdateTime();
         
         virtual void Init(ParamContainer &params) {}
         
         virtual void Init( WaveFunction * Psi );
         
         virtual Operator* operator=(Operator* O);
         
         virtual Operator* Copy(Operator* O);
         
         virtual bool Valid(WaveFunction *Psi);
         
         virtual void RecalcInternals(bool on);
   };

}

#endif

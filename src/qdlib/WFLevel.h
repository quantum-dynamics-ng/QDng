#ifndef WFLEVEL_H
#define WFLEVEL_H

#include "qdlib/WaveFunction.h"

namespace QDLIB {
   
   /**
    * Wavefunction class for a simple level system.
    */
   class WFLevel: public WaveFunction
   {
      private:
         string _name;

      public:
         /* Interface implementation */
         WFLevel();

         ~WFLevel();

         WaveFunction* NewInstance();

         virtual void Init(ParamContainer &params);

         virtual const string& Name();

         WaveFunction* operator=(WaveFunction *Psi);

         dcomplex operator*(WaveFunction *Psi);

         virtual void Reduce(double tolerance);

         virtual void Restore();

         /* Own methods */
         void Size(int size);

         WFLevel& operator=(WFLevel &Psi);

         virtual void Serialize (::google::protobuf::io::ZeroCopyOutputStream& os);

         virtual void DeSerialize (::google::protobuf::io::ZeroCopyInputStream& is);

   }; /* class WFLevel */
   
} /* namespace QDLIB */

#endif /* #ifndef WFLEVEL_H */

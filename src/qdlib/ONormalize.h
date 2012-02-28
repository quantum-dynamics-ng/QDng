/*
 * ONormalize.h
 *
 *  Created on: 04.07.2011
 *      Author: mko
 */

#ifndef ONORMALIZE_H_
#define ONORMALIZE_H_

#include "qdlib/Operator.h"
#include "qdlib/WaveFunction.h"

using namespace std;
namespace QDLIB
{

   /**
    * Simple Dummy Operator which can normalize a
    */
   class ONormalize : public Operator
   {
      private:
         string _name;
      public:
         ONormalize() : _name("ONormalize") {}
         virtual ~ONormalize() {}

         virtual Operator* NewInstance();
         virtual const string& Name();

         virtual void Init(ParamContainer &params) { _params = params; }
         virtual void Init(WaveFunction *Psi) {}

         virtual dcomplex Emin() { return 0; }
         virtual dcomplex Emax() { return 1; }

         virtual void UpdateTime();

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
         {
            destPsi->FastCopy(*sourcePsi);
            destPsi->Normalize();
         }

         virtual void Apply(WaveFunction *Psi) { Psi->Normalize(); }

         virtual Operator* operator=(Operator* O) { return this; }
         virtual Operator* Copy(QDLIB::Operator*) { return this; }

         virtual bool Valid(WaveFunction *Psi) { return true; }
   };

}

#endif /* ONORMALIZE_H_ */

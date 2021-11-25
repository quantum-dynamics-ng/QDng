#ifndef QDLIBOJUMP_H
#define QDLIBOJUMP_H

#include "qdlib/OList.h"

namespace QDLIB {

   /**
    * Random jump operator.
    *
    * Takes a set of Lindblad jump operators.
    * Note, that the rates have to be included in the Lindblad operators.
    * The operator renormalizes at each application and after each jump.
    *
    * Parameters:
    * \param seed       Seed value for random number generator. Integer greater than zero [0]
    * \param max_pjump  Maximum accepted summed jump propability. An error occurs if a greater propability is calculated [0.5]
    *
    *
    *  @author Markus Kowalewski <markus.kowalewski@fysik.su.se>
    */
   class OJump : public OList
   {
      private:
         string _name;
	 double _max_pjump;
      public:
         OJump();

         ~OJump();

         /* Interface implementation */
         virtual Operator* NewInstance();

         virtual dcomplex Emax() {
	   throw EIncompatible("OJump::Emax is not implemented");
	 }

         virtual dcomplex Emin(){
	   throw EIncompatible("OJump::Emin is not implemented");
	 }

	 virtual void Init(ParamContainer &params);

         virtual const string& Name();

         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual void Apply(WaveFunction *Psi);
   };

}

#endif

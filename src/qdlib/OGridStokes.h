#ifndef QDLIBOGRIDSTOKES_H
#define QDLIBOGRIDSTOKES_H

#include "OGridNabla.h"
#include "OGridPosition.h"

namespace QDLIB {

   /**
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    *
    * Non-linear friction operator.
    *
    * \f$ \hat S = 6\pi\eta R \frac{1}{m} \left[
    *  c\left( \frac{1}{2}
    *  \left(\hat x \hat p + \hat p \hat x \right ) - \langle x \rangle \hat p \right)
    *  + (1-c) \langle p \rangle (x-\langle x \rangle) \right]\f$
    *
    *  parameters:
    *  \li mass reduced mass
    *  \li R    Particle radius
    *  \li eta  friction coeffiecient of the solvent
    *  \li c    weight factor of the non-linear potentials.
    */
   class OGridStokes : public OGridNabla, public OGridPosition
   {
      private:
         string _name;
	 WaveFunction *_buf;
         double _mass;
         double _eta;
         double _R;
         double _c;
      public:
         OGridStokes();
         ~OGridStokes();
         
         virtual Operator* NewInstance();
         virtual const string& Name();
         virtual void Init(ParamContainer &params);
         virtual void Init(WaveFunction *Psi);
         virtual void UpdateTime(){}
         virtual dcomplex Emax();
         virtual dcomplex Emin() { return dcomplex(0); }
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         virtual void Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
         virtual Operator* Copy(Operator* O);
         virtual bool Valid(WaveFunction *Psi);
   
   };

}

#endif

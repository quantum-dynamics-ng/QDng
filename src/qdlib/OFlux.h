#ifndef QDLIBOFLUX_H
#define QDLIBOFLUX_H

#include <OGridNabla.h>

namespace QDLIB {

   /**
    * Flux Integration Operator.
    * 
    *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OFlux : public OGridNabla
   {
      private:
         string _name;
         double _mass;
         WaveFunction *_buf;
         bool _useGm;
         OGridPotential* _Gm[MAX_DIMS];    /* Reciprocal, coordinate dependent mass elements. */
      public:
         OFlux();
         ~OFlux();
         
         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
                 
         virtual void Init(ParamContainer &params);
         
         virtual void Init(WaveFunction *Psi);
         
         virtual const string& Name();
         
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
         
         virtual double Expec(WaveFunction *Psi);
        
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         
         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);
         
         virtual Operator* Copy(Operator* O);
   
   };

}

#endif

#ifndef QDLIBOGRIDSTOKES_H
#define QDLIBOGRIDSTOKES_H

#include "OGridNabla.h"
#include "OGridPosition.h"

namespace QDLIB {

   /**
   * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class OGridStokes : public OGridNabla, public OGridPosition
   {
      private:
         string _name;
         double _mass;
         double _eta;
         double _R;
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

#ifndef QDLIBOGRIDPOSITION_H
#define QDLIBOGRIDPOSITION_H

#include "qdlib/OGridsystem.h"

namespace QDLIB {

   /**
    * Position Operator
    * 
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    * 
    * prams:
    * \li dim  Use only for dimension i
    */
   class OGridPosition : public OGridSystem
   {
      private:
         string _name;
         int _dim;         /* The dimension to operate on */
         void _InitDim(dVecView &view, const int dim);
      public:
         OGridPosition();

         OGridPosition(int dim);
      
         ~OGridPosition();
   
	 virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params);
	 virtual void Init(WaveFunction *Psi);
	 virtual const string& Name();
         virtual void UpdateTime(){}
	 virtual dcomplex Emax();
	 virtual dcomplex Emin();
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 virtual void Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
	 virtual Operator* Copy(Operator* O);
   };

}

#endif

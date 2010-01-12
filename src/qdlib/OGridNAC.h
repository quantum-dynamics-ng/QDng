#ifndef QDLIBOGRIDNAC_H
#define QDLIBOGRIDNAC_H

#include <OGridNabla.h>
#include "OGridPotential.h"

namespace QDLIB {

   /**
    * Coupling operator for Non-Adiabatic coupling.
    * 
    * params:
    * \li dims  Number of grid dimensions
    * \li file  Numeric representation of the NAMCE
    * \li sign  pre-factor (chose 1 or -1)
    * \li mass  reduced mass
    * 
    * \todo implement for N-dims
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OGridNAC : public OGridNabla
   {
      private:
         string _name;
         OGridPotential _NACME; /* Holds the NAC Matrix elements*/
         dVec* _pNAMCE;          /* Content pointer to _NACME */
         double _sign;           /* Sign of the coupling */
         double _mass;            /* mass for 1D-NACs */
         WaveFunction *_buf;
//          WaveFunction *_buf2;
//          FFT _fft;
      public:
         OGridNAC();
      
         ~OGridNAC();
         
         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
         virtual void Init(WaveFunction *Psi);
	 
         virtual const string& Name();
	 
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
         virtual double Expec(WaveFunction *Psi);
	
         virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
         virtual WaveFunction* Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);
	 
         virtual Operator* Copy(Operator* O);
	 
         virtual Operator* operator*(Operator* O);
   };

}

#endif

// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOCHEBY_H
#define QDLIBOCHEBY_H

#include "OPropagator.h"

#define BESSEL_DELTA 1e-32       /* Lower limit for convergence of the Bessel series convergence (default) */
#define DEFAULT_PREC 1e-10       /* Default convergence criteria on norm */
#define BESSEL_MAX_ORDER 1e6     /* Maximum recursion order to used (by automatic determination) */


namespace QDLIB
{

   /**
    * Chebychev Propagator for WaveFunctions.
    * 
    * Init parameters:
    * \li order   desired max order of recursion, if left empty it's chosen automatically.
    * \li scaling The scaling of the Hamiltonian. Can only be used in combination with order.
    * \li prec    The lower limit precission for the series convergence on the norm \f$ 1-\langle psi|psi \rangle \f$
    *
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OCheby : public OPropagator
   {
      private:
	 string _name;
	 
	 int _order;          /* Recursion depth */
	 cVec _coeff;         /* Coefficients including the Bessel functions at k'th order */
	 dcomplex _exp;       /* Scaled exponent:  e.g. -i*dt/Rdelta */
	 double _scaling;     /* Energy range of the hamiltonian => the Scaling*/
	 double _prec;        /* Convergence criteria on norm */
	 dcomplex _offset;    /* Energy offset of the hamiltonian */
         WaveFunction *ket0, *ket1, *ket2, *buf;
	 
         double raw_norm(WaveFunction *Psi);
         void real_time_step(WaveFunction *Psi);
         void imag_time_step(WaveFunction *Psi);
      public:
         OCheby();
         ~OCheby();
	 
	 int Recursion();
	 
	 void Hamiltonian(Operator *H);
	 
	 /* Interface implementation */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual void Init(WaveFunction *Psi);
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime(){ if (H != NULL) H->UpdateTime(); }
	 
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);

         virtual bool Valid(WaveFunction *Psi);

   };

}

#endif

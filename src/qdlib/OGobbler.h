// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOGOBBLER_H
#define QDLIBOGOBBLER_H

#include "qdlib/OGridsystem.h"

namespace QDLIB {

   /**
    * Gobbler operator which works with Butter-worth filter functions.
    * 
    * You have to specify the cut-off x-values. in the n'th dimension.
    * 
    * Gobbler takes the following parameters:
    * \li dims    Number of dimensions.
    * \li rp#     a "right pass" leaving everything on the right hand side (larger x values).
    * \li lp#     a "left pass"  leaving everything on the left hand side (smaller x values).
    * \li order   filter order. determines the cutoff sharpness.
    * \li nip     If present use the gobbler as negative imaginary potential.
    * \li gain    Supply a gain value. 1 per default. Only makes sense in combination with nip
    * \li residue Don't calculate a expectation value but the residue after application
    *
    * 
    * @author Markus Kowalewski
    */
   class OGobbler : public OGridSystem
   {
      private:
	 string _name;
	 bool _lp[MAX_DIMS];    /* build left pass for dim */
	 double _lpx[MAX_DIMS];  /* cut-off center */
	 bool _rp[MAX_DIMS];    /* build right pass for dim */
	 double _rpx[MAX_DIMS];  /* cut-off center */
	 int _order;           /* filter order */
	 double _gain;         /* Gain value */
	 bool _nip;            /* Negative imaginary potential */
         bool _residue;        /* Don't calculate a expectation value but the residue after application */
	 
	 void _Init();
      public:
	 OGobbler();
      
	 ~OGobbler();
	 
	 /* Interface implementation, Operator */ 
	 virtual Operator* NewInstance();
		  
	 virtual void Init(ParamContainer &params);
	 
	 virtual void Init(WaveFunction *Psi);
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime(){};
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual dcomplex Emax();
	 
	 virtual dcomplex Emin();
	 
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual void Apply(WaveFunction *Psi);
	 
	 virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);

   };

}

#endif

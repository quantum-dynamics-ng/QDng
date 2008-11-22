#ifndef QDLIBOGOBBLER_H
#define QDLIBOGOBBLER_H

#include "OGridsystem.h"

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
    * 
    * \todo TEST
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
	 bool _nip;            /* Negative imaginary potential */
	 
	 void _Init(GridSystem *Psi);
      public:
	 OGobbler();
      
	 ~OGobbler();
	 
	 /* Interface implementation, Operator */ 
	 virtual Operator* NewInstance();
		  
	 virtual void Init(ParamContainer &params);
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime(){};
	 
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual double Emax(){return 0;}
	 
	 virtual double Emin(){return 0;}
	 
	 virtual WaveFunction* operator*(WaveFunction *Psi);
	 
	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 
	 virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);

   };

}

#endif

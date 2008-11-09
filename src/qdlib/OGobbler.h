#ifndef QDLIBOGOBBLER_H
#define QDLIBOGOBBLER_H

#include <OKSpace.h>

namespace QDLIB {

   /**
    * Gobbler operator which works with Butter-worth filter functions.
    * @author Markus Kowalewski
    */
   class OGobbler : public OGridSystem
   {
      private:
	 string _name;
	 int _dims[MAX_DIMS];
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
	 
	 virtual WaveFunction* operator*(WaveFunction *Psi);
	 
	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 
	 virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);

   };

}

#endif

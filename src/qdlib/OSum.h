#ifndef OSUM_H
#define OSUM_H

#include "Operator.h"

#define MAX_OPS 16

namespace QDLIB {
   
   /**
    * Represents a sum of arbitrary Operators.
    * 
    * The class stores operators and use it as a sum of them.
    * 
    * \f$ O_{sum} = \sum_i^{Size} \hat O_i \f$
    * 
    * All Operator objects will be destructed.
    */
   class OSum: public Operator {
      private:
	 string _name;
         int _size;
         Operator* _O[MAX_OPS];
	 WaveFunction* _WFbuf[MAX_OPS];    /* Use a buffer for wfs, to get rid of much mallocs */
      public:
         
         ~OSum();
	 
         OSum();
         
         Operator* operator[](int i);
         
         int Size();
         
         void Add(Operator *O);
         
         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual void UpdateTime();
	 	 
         virtual const string& Name();
	                
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
         virtual double Expec(WaveFunction *Psi);
	 
	 virtual double Emax();
	 
	 virtual double Emin();
	 
         virtual WaveFunction* operator*(WaveFunction *Psi);
	 
         virtual WaveFunction* operator*=(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
         virtual Operator* operator*(Operator* O);
            
	 virtual Operator* Offset(const double d);

	 virtual Operator* operator-=(const double d);

	 virtual Operator* Scale(const double d);

   };
   
} /* namespace QDLIB */

#endif /* #ifndef OSUM_H */

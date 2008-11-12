#ifndef OGrid1DD2DX_H
#define OGrid1DD2DX_H

#include "OKSpace.h"

namespace QDLIB {
   
   /**
    * Kinetic energy operator for 1D cartesian grid.
    * 
    * \f$ - \frac{1}{2m} \frac{d^2}{dx} \f$ -Operator.
    * Where \f$ m \f$ is the reduced mass.
    * Compatible with WFGrid1D.
    * 
    * Takes m as parameter.
    */
   class OGrid1Dd2dx : public OKSpace
   {
      private:
         string _name;
	 
	 /* remember what we did last time => no need to re-init k-space */
         double _L_last;
	 int _Nx_last; 
                  
         /** Reduced mass */
         double _mass;
      public:
         OGrid1Dd2dx() : _name("OGrid1Dd2dx"),
                     _L_last(0), _Nx_last(0), _mass(0)
	 {
	    _isTimedependent = false;
	    
	 }
         
         double Mass();
         void Mass(double mass);
         
         /* Interface Implementation - KSpace */
         
         virtual void InitKspace();
         
         /* Interface Implementation - Operator */
         
         virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
         virtual const string& Name();
	 
	 virtual void UpdateTime();
	 
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
         virtual double Expec(WaveFunction *Psi);
	 
	 virtual double Emax();
	 
	 virtual double Emin();
	 
         virtual WaveFunction* operator*(WaveFunction *Psi);
	 
         virtual WaveFunction* operator*=(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
         virtual Operator* operator*(Operator* O);
         
   };
   
} /* namespace QDLIB */

#endif /* #ifndef OGrid1DD2DX_H */

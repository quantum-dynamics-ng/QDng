#ifndef QDLIBOGRIDGMAT_H
#define QDLIBOGRIDGMAT_H

#include "OGridPotential.h"
#include "WFGridSystem.h"
#include "TransformFFT.h"

namespace QDLIB {

   /**
    * Kinetic energy Operator for the G-matrix method.
    * 
    * parameters:
    * \li dims       Number of internal coordiantes
    * \li gmat   G-matrix element file name
    * \li coup   Turn on kinetic coupling [default: true]
    * 
    * \todo possibility to turn of specific matrix elements
    * \todo enable constant-g mode / mixed g-mode
    * 
    * @author Markus Kowalewski
    */
   class OGridGMat : public Operator, public GridSystem
   {
      private:
	 typedef Matrix<OGridPotential> Gmatrix_t;
	 typedef Vector<dVec> Gkspace_t;
	 
	 string _name;
	 lint _size;
	 OGridPotential*** _Gmat;     /* 2D Array coordinate dependent elements*/
	 double** _GmatC;             /* 2D Array constant G elements */
	 dVec* _kspace;
	 WFGridSystem** _wfbuf;      /* 1D Array */
	 WFGridSystem* buf;
	 
	 void _InitKspace (WFGridSystem *Psi);
	 TransformFFT _FFT;
         
	 bool _KinCoup;
      public:
	 OGridGMat();
      
	 ~OGridGMat();
	 
	 /* Interface implementation */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual void Init(WaveFunction *Psi);
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime(){} /* Nothing to do here */
	                
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual dcomplex Emax();
	 
	 virtual dcomplex Emin();
	 
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual WaveFunction* Apply(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);
	 
	 virtual Operator* Offset(const double d){return this;} /* No need to do this */
	 
	 virtual Operator* Scale(const double d);
         
         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif

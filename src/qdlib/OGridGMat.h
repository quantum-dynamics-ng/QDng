#ifndef QDLIBOGRIDGMAT_H
#define QDLIBOGRIDGMAT_H

#include "OGridPotential.h"
#include "WFGridSystem.h"

namespace QDLIB {

   /**
    * Kinetic energy Operator for the G-matrix method.
    * 
    * parameters:
    * \li dims       Number of internal coordiantes
    * \li gmat   G-matrix element file name
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
	 OGridPotential*** _Gmat;     /* 2D Array*/
	 dVec* _kspace;
	 WFGridSystem** _wfbuf;      /* 1D Array */
	 WFGridSystem* buf;
	 
	 void _InitKspace (WFGridSystem *Psi);
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
	 
	 virtual double Emax();
	 
	 virtual double Emin();
	 
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual WaveFunction* Apply(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);
	 
	 virtual Operator* Offset(const double d){return this;} /* No need to do this */
	 
	 virtual Operator* Scale(const double d);
	 
   
   };

}

#endif

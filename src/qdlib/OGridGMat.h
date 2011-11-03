#ifndef QDLIBOGRIDGMAT_H
#define QDLIBOGRIDGMAT_H

#include "qdlib/OGridPotential.h"
#include "qdlib/WFGridSystem.h"
#include "qdlib/TransformFFT.h"

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
	                
	 virtual dcomplex Emax();
	 
	 virtual dcomplex Emin();
	 
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual void ApplyAdd(WaveFunction *destPsi, WaveFunction *sourcePsi);

	 virtual void Apply(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	          
         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif

#ifndef QDLIBOSPO_H
#define QDLIBOSPO_H

#include "OPropagator.h"
#include "Exception.h"

namespace QDLIB {

   /**
    * SPO propagator for GridSystem.
    * 
    * Relies on a OKSpace operator and a OGridSystem.
    * 
    * \todo optimize for imaginary time propagation (no need for complex exponentials).
    */
   class OSPO : public OPropagator
      {
	 private:
	    string _name;
	    OKSpace *_Tkin;
	    OGridSystem *_Vpot;
	    cVec *_expT;
	    cVec *_expV;
	    
	    dcomplex _cV, _cT;
	    double _last_time;
	    
	    void _initV();
	    void _initT();
	    
	 public:
	    OSPO();
	 
	    ~OSPO();
	    	    
	    
	    /* Interface implementation, Operator */
	    virtual Operator* NewInstance();
		  
	    virtual void Init(ParamContainer &params);
	    
	    virtual const string& Name();
			   
	    virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	    
	    virtual double Expec(WaveFunction *Psi);
	    
	    virtual WaveFunction* operator*(WaveFunction *Psi);
	    
	    virtual WaveFunction* operator*=(WaveFunction *Psi);
	    
	    virtual Operator* operator=(Operator* O);
	    
	    virtual Operator* operator*(Operator* O);
	    
	    /* Interface implementation, Propagator */
	    virtual ParamContainer& TellNeeds();
	 
	    virtual void AddNeeds(string &Key, Operator *O);
	    
	    virtual ReInit();
	 	    
      };

}

#endif

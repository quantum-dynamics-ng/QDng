#ifndef QDLIBOSPO_H
#define QDLIBOSPO_H

#include "OPropagator.h"
#include "Exception.h"

namespace QDLIB {

   /**
    * SPO propagator for GridSystem.
    */
   class OSPO : public OPropagator
      {
	 private:
	    string _name;
	    OKSpace *Tkin;
	    OGridSystem *Vpot;
	    cVec *expT;
	    dVec *expV;
	 public:
	    OSPO();
	 
	    ~OSPO();
	    
	    /**
	     * Initialize the exponentials.
	     */
	    void InitExp();
	    
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
	 	    
      };

}

#endif

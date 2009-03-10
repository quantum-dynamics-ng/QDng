#ifndef QDLIBOSPO_H
#define QDLIBOSPO_H

#include "OKSpace.h"
#include "OPropagator.h"
#include "OGridSum.h"
#include "tools/Exception.h"


namespace QDLIB {

   /**
    * SPO propagator for GridSystem.
    * 
    * Relies on a OKSpace operator and a OGridSystem.
    * 
    * \todo optimize for imaginary time propagation (no need for complex exponentials).
    * \todo add support for complex k-Space operators.
    */
   class OSPO : public OPropagator
      {
	 private:
	    string _name;
	    ParamContainer *_needs;
	    
	    OKSpace *_Tkin;
	    dVec    *_Tkin_kspace;
	    OGridSystem *_Vpot;
	    cVec *_expT;
	    cVec *_expV;
	    
	    dcomplex _cV, _cT;
	    int _last_time;
	    
	    void _InitV();
	    void _InitT();
	    
	 public:
	    OSPO();
	 
	    ~OSPO();
	    	    
	    
	    void AddTkin( OKSpace *T );
	    void AddVpot( OGridSystem *V );
	   
	    
	    /* Interface implementation, Operator */
	    virtual Operator* NewInstance();
		  
	    virtual void Init(ParamContainer &params);
	    
	    virtual void Init(WaveFunction *Psi);
	    
	    virtual const string& Name();
			   
	    virtual void UpdateTime();
	    
	    virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	    
	    virtual double Expec(WaveFunction *Psi);
	   	    
// 	    virtual WaveFunction* operator*(WaveFunction *Psi);
	    virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	    
// 	    virtual WaveFunction* operator*=(WaveFunction *Psi);
	    virtual WaveFunction* Apply(WaveFunction *Psi);

	    
	    virtual Operator* operator=(Operator* O);
	    
	    virtual Operator* operator*(Operator* O);
	    
	    /* Interface implementation, Propagator */
	    virtual ParamContainer& TellNeeds();
	 
	    virtual void AddNeeds(string &Key, Operator *O);
	    
	 	    
      };

}

#endif

#ifndef QDLIBOPROJECTION_H
#define QDLIBOPROJECTION_H

namespace QDLIB {

   /**
    * Projection operator.
    * 
    * \f$ \sum_i |\Psi_i><\Psi_i> \f$
    * 
    * Options:
    * \li fname   Base name for file series
    * 
    * @author Markus Kowalewski
    */
   class OProjection{
      private:
	 int _size;
      public:
	 OProjection();
      
	 ~OProjection();
      
	 void Add(WaveFunction* Psi);
	 
	 int Length() { return _size; };
	 
	 /* Interface implementation, Operator */ 
   
	 virtual Operator* NewInstance() ;
		  
	 virtual void Init(ParamContainer &params);
	    
	 virtual void Init(WaveFunction *Psi);
   
	 virtual void UpdateTime();
	    
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	    
	 virtual double Expec(WaveFunction *Psi);
	    
	 virtual double Emax() {return 0;}
	    
	 virtual double Emin() {return 0;}
	    
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	    
	 virtual WaveFunction* Apply(WaveFunction *Psi);
		  
	 virtual Operator* operator=(Operator* O);
	    
	 virtual Operator* operator*(Operator* O);
      
	 virtual Operator* Offset(const double d);
   
	 virtual Operator* Scale(const double d);
      
      
   };

}

#endif

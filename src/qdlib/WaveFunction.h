#ifndef WAVEFUNCTION_H
#define WAVEFUNCTION_H


#include "tools/ParamContainer.h"
#include "math/typedefs.h"
#include "tools/Exception.h"


namespace QDLIB { 
   /**
    * Abstract Wavefunction class.
    * 
    * Rules and Guidelines:
    * -If a new cVec component is constructed, it will be not initialized with zeroes.
    * -Setting of all parameters must be possible trough the ParamContainer interface.
    * Aditional Getter and Setter methods may be implementet.
    * 
    * -Optional: Define operator= also for derived class.
    * 
    * \todo check tnt classes for init methods (zeroes or not)
    */
   class WaveFunction: public cVec
   {
      private:
         bool _IsKspace;
      protected:
	 ParamContainer _params;
      public:
         WaveFunction() :cVec() ,_IsKspace(false) {}
	 WaveFunction(ParamContainer &params) : _params(params) {}
	 /**
	  * Make the class pure virtual.
	  */
 	 virtual ~WaveFunction() {}
	 
	 /**
	  * This method should create a new instance.
	  * 
	  * The parameters should be same, but the storage
	  * will not be initialized.
	  */
	 virtual WaveFunction* NewInstance() = 0;
	       
	 /**
	  * Initializer.
	  * 
	  * This should also be used for an updated or changed parameter set.
	  */
	 virtual void Init(ParamContainer &params) = 0;
	 
	 /**
	  * Return the parameter set.
	  */
	 ParamContainer& Params() { return _params; }
	 
	 /**
	  * Human readable identifier.
	  * \return Name of the implementet wave function
	  */
	 virtual const string& Name() = 0;
	 	    
	 /**
	  * Norm of the wave function.
	  * 
	  * \f$ N = \sqrt{ \langle \Psi | \Psi \rangle} \f$
	  */
	 virtual double Norm() = 0;
	 
	 
	 virtual void Normalize() = 0;
      
         /**
          * Check if WF is in different representation/basis.
         */
         virtual bool IsKspace() {return _IsKspace;}
         
         /**
          * Tell the WF that it is in different representation/basis.
          */
         virtual void IsKspace(bool is) {_IsKspace = is;}
                  
         /**
          * Transform WF into basis of an Operator.
          * 
          * The Implementation has to decide into which basis the transformation is done.
          * The choice may depend on O. 
          */
//          virtual void ToKSpace(Operator *O) = 0;
         
         /**
          * Transform back into the original representation or basis.
          * 
          * This is the inverse of ToKSpace.
          */
//          virtual void ToXSpace(Operator *O) = 0;

         
	 /** Copy. */
	 virtual WaveFunction* operator=(WaveFunction* Psi) = 0;
	 
	 /** Scalar product. */
	 virtual dcomplex operator*(WaveFunction* Psi) = 0;
         
         /** Multiply with scalar */
         void operator*=(const double d)
         {
	    MultElements((cVec*) this, d);
         }
         
	 /**
	  * Assign scalar value.
	  */
	 WaveFunction* operator=(double d)
	 {
	    *((cVec*) this) = dcomplex(d,0);
	    return this;
	 }
	 
	 /**
	  * Assign scalar value.
	  */
	 WaveFunction* operator=(dcomplex d)
	 {
	    *((cVec*) this) = d;
	    return this;
	 }

	 /** Multiply with complex number */
	 WaveFunction* operator*=(const dcomplex d)
	 {
      
	    int size = cVec::size();
	    for (int i=0; i < size; i++){
	       (*this)[i] *= d;
	    }
	    
	    return this;
	 }
	 
	 /** Sum */
	 WaveFunction* operator+(WaveFunction* Psi)
	 {
            WaveFunction *wf;
      
            wf = this->NewInstance();
	    
            for (int i=0; i < cVec::size(); i++){
               (*(cVec*)wf)[i] = (*this)[i] + (*Psi)[i];
	    }
	    
	    return wf;
	 }
	 
	 /** Sum */
	 WaveFunction* operator+=(WaveFunction* Psi)
	 {
	    if (cVec::size() != Psi->size()) throw ( EParamProblem("WFs differ in size") );
	    
	    AddElements((cVec*) this, (cVec*) Psi);
	    return this;
	    
	 }
	 
	 /** Difference */
	 WaveFunction* operator-=(WaveFunction* Psi)
	 {
	    if (cVec::size() != Psi->size()) throw ( EParamProblem("Grids differ in size") );
	    for (int i=0; i < cVec::size(); i++){
	       (*(cVec*)this)[i] += -1*(*(cVec*) Psi)[i];
	    }
	    return this;
	    
	 }
	 
         /** Pointwise multiplication.
          * In place method.
          */
         void DirectProduct(WaveFunction* Psi)
         {
            if (cVec::size() != Psi->size()) throw ( EParamProblem("WFs differ in size") );
	    
            MultElements((cVec*) this, (cVec*) Psi);
	    
         }
	 
	 /** Pointwise Complex multiplication. Psi* x Psi
          * In place method.
          */
         void DirectProducConugatet(WaveFunction* Psi)
         {
            if (cVec::size() != Psi->size()) throw ( EParamProblem("WFs differ in size") );
            MultElementsConugate((cVec*) this, (cVec*) Psi, (double) 1.0);
	    
         }
         
   }; /* class WaveFunction */

   /** Pointwise multiplication. */
   WaveFunction* DirectProduct(WaveFunction* PsiA, WaveFunction* PsiB);

   /** Pointwise Complex multiplication.Psi* x Psi */
   WaveFunction* DirectProductConugate(WaveFunction* PsiA, WaveFunction* PsiB);


  
} /* namespace QDLIB */


#endif /* ifndef WAVEFUNCTION_H */


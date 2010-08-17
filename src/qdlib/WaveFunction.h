#ifndef WAVEFUNCTION_H
#define WAVEFUNCTION_H


#include "tools/ParamContainer.h"
#include "math/typedefs.h"
#include "tools/Exception.h"
#include "tools/Collector.h"

#define DELETE_WF(WF) Collector<WaveFunction>::Instance()->Delete(WF)
#define DELETE_ALL_WF() Collector<WaveFunction>::Instance()->Delete()

namespace QDLIB { 
   /**
    * Abstract Wavefunction class.
    * 
    * Rules and Guidelines:
    * \li If a new cVec component is constructed, it will be not initialized with zeroes.
    * \li Setting of all parameters must be possible trough the ParamContainer interface.
    * Aditional Getter and Setter methods may be implementet.
    * 
    * -Optional: Define operator= also for derived class.
    * 
    * This class is maintained by the Collector. Don't create instances on the stack.
    * Explictly delete them via the macro DELETE_WF
    * 
    */
   class WaveFunction: public cVec
   {
      protected:
         bool _IsKspace;      /* To remember in which basis we are */
         cVec *_spacebuffer;   /* transformation buffer for different basis */
	 ParamContainer _params;
      public:
	 WaveFunction() :cVec() ,_IsKspace(false), _spacebuffer(NULL) { Collector<WaveFunction>::Instance()->Register(this); }
	 /**
	  * Make the class pure virtual.
	  */
         virtual ~WaveFunction() {if (_spacebuffer != NULL) delete _spacebuffer;}
	 
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
         bool IsKspace() {return _IsKspace;}
         
         /**
          * Tell the WF that it is in different representation/basis.
          * 
          * This also switches the buffer space.
          */
         void IsKspace(bool is)
         {  
            if (!_spacebuffer) _spacebuffer = new cVec(size());
            if (_IsKspace != is){
               cVec::swap(*_spacebuffer);
               _IsKspace = is;
            }
         }
         
         /**
          * Returns the buffer for different basis.
          * 
          * Initialization (size) is done automatically.
          */
         cVec* GetSpaceBuffer()
         {
            if (!_spacebuffer) _spacebuffer = new cVec(size(),strides());
            return _spacebuffer;
         }

         /**
          * Reduce/Clean up the wavefunction as preparation to compression.
          * 
          * The result must be stored in the k-space buffer as
          * in most cases the reduction will be combinded with a transformation.
          * The orginal Wavefunction must not be affected by this operation.
          */
         virtual void Reduce(double tolerance) = 0;
         
         /**
          * This is the inverse operation of Reduce.
          * 
          * The input is present in the k-space buffer.
          * The result must be stored in real space.
          */
         virtual void Restore() = 0;
         
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
	 WaveFunction* operator*=(const dcomplex c)
	 {
      
/*	    int size = cVec::size();
	    for (int i=0; i < size; i++){
	       (*this)[i] *= d;
	    }*/
	    MultElements((cVec*) this, c);
	    return this;
	 }
	 
	 /** Sum */
	 WaveFunction* operator+(WaveFunction* Psi)
	 {
            WaveFunction *wf;
      
            wf = this->NewInstance();
	    
	    AddElementsEq((cVec*) wf, (cVec*) this, (cVec*) Psi);

/*            for (int i=0; i < cVec::size(); i++){
               (*(cVec*)wf)[i] = (*this)[i] + (*Psi)[i];
	    }*/
	    
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
	    if (cVec::size() != Psi->size()) throw ( EParamProblem("WFs differ in size") );
	    
	    AddElements((cVec*) this, (cVec*) Psi, -1);
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


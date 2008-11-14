#ifndef WAVEFUNCTION_H
#define WAVEFUNCTION_H


#include "sys/ParamContainer.h"
#include "math/typedefs.h"
#include "sys/Exception.h"





namespace QDLIB {
  
   /**
    * Identifies the basic type of the wave function.
    * 
    */
   enum WFType
   {  /** vector format. */
      vector, 
      /** grid DVR representation. */
      grid 
   };
      
   /**
    * \struct WFDesc
    * General description of wave function content
    */
//    typedef struct _wfdesc {
//       /** System ID / wf ID */
//       int id;
//       /** basic wave function type */
//       WFType type;
//       /** number of dimensions.
//        * May have different meaning for grid / vector types
//        */
//       int num_dims;
//       /** Size of each dimension.
//        * number of elements defined by num_dims.
//        */
//       int dims[MAX_DIMS];
//    } WFDesc;
// 	 
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
      protected:
	 ParamContainer _params;
      public:
	 WaveFunction() {}
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
	  * Should return a unique ID which idenfies the wave function.
	  */
	 virtual int ID() = 0;
	    
	 /**
	  * Norm of the wave function.
	  * 
	  * \f$ N = \sqrt{ \langle \Psi | \Psi \rangle} \f$
	  */
	 virtual double Norm() = 0;
	 
	 
	 virtual void Normalize() = 0;
      
	 
	 /** Copy. */
	 virtual WaveFunction* operator=(WaveFunction* Psi) = 0;
	 
	 /** Scalar product. */
	 virtual dcomplex operator*(WaveFunction* Psi) = 0;
         
         /** Multiply with scalar */
         void operator*=(const double d)
         {
            for (int i=0; i < cVec::size(); i++){
               (*this)[i] *= d;
            }
         }
         
         /** Multiply with scalar */
         WaveFunction* operator*(const double d)
         {
            WaveFunction *wf;
      
            wf = this->NewInstance();
      
            for (int i=0; i < cVec::size(); i++){
               (*wf)[i] = (*this)[i] * d;
            }
	    
	    return wf;
         }
         
	 /** Multiply with complex number */
	 WaveFunction* operator*=(const dcomplex d)
	 {
      
	    for (int i=0; i < cVec::size(); i++){
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
	    if (cVec::size() != Psi->size()) throw ( EParamProblem("Grids differ in size") );
            for (int i=0; i < cVec::size(); i++){
	       (*(cVec*)this)[i] += (*(cVec*) Psi)[i];
	    }
	    return this;
	    
	 }
   }; /* class WaveFunction */

  
} /* namespace QDLIB */


#endif /* ifndef WAVEFUNCTION_H */


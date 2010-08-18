#ifndef OPERATOR_H
#define OPERATOR_H

#include "WaveFunction.h"
#include "tools/Exception.h"
#include "tools/QDClock.h"
#include "tools/Collector.h"

#define DELETE_OP(OP) Collector<Operator>::Instance()->Delete(OP)
#define DELETE_ALL_OP() Collector<Operator>::Instance()->Delete()

#define QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(CLASSNAME) \
   Operator* CLASSNAME::NewInstance() \
{ \
      CLASSNAME *p; \
      p = new CLASSNAME (); \
      CollectorOp::Instance()->Register(p); \
      return p; \
}

namespace QDLIB {


   /**
    * Abstract operator class.
    * 
    * Defines what an operator must provide.
    * Since this is an abstract class, all the defined operations apply to pointers.
    * The left handed argument (in this case the operator) has to dereferenced with *O.
    * 
    * An operator has to initialized in the following way:
    * \li Give the operator a clock (if needed)
    * \li Call Init with parameter set
    * \li Call Init with WaveFunction
    * \li update the clock
    * 
    * Re-Inits with different parameters or WaveFunctions are forbidden!
    * 
    * Use the DELETE_OP macro to destroy Operator objects.
    * Operator Objects are registered with Collector at construction.
    * Don't create an Operator object on the stack, use new instead.
    */
   class Operator
   {

      protected:
	 /**
          * Parameter storage for the implementation.
	  */
	 ParamContainer _params;
	 /**
	  * Indicates time dependence of operator.
	  * Must be set by the implementing class.
	  */
	 bool _isTimedependent;
	 /**
	  * The operators clock.
	  * This is needed by time dependent operators.
	  */
	 QDClock *clock;
      public:
	 /**
          * Make class pure virtual
          */
         virtual ~Operator() {}
         
	 /**
	  * Standard constructor
	  */
	 Operator() :  _isTimedependent(false), clock(NULL)
	 {
         }
	  
	 /**
	  * Constructor with full parameter set.
	  */
	 Operator(ParamContainer &params):  _params(params), _isTimedependent(false)
	 {
         }
         
	 /**
	  * This method should create a new instance.
	  * 
	  * The parameters should be same, but the storage
	  * will not be copied.
	  */
	 virtual Operator* NewInstance() = 0;
	 	 
         /**
          * Initialize and update the parameters of the operator.
          */
         virtual void Init(ParamContainer &params) = 0;
	 
	 /**
	  * Initialize the operator with a wavefunction.
	  * 
	  * This should be done after initalisation with all parameters and
	  * before using the Operator with any wave function.
	  */
	 virtual void Init(WaveFunction *Psi) = 0;
	 
	 /**
	  * Return the parameter set.
	  */
	 ParamContainer& Params() { return _params; };
	 	 
	 /**
          * Should return a unique ID which idenfies the operator.
          */
	 virtual const string& Name() = 0;
	 
	 /**
	  * Time dependence of Operator.
	  */
	 bool isTimeDep() { return _isTimedependent; }
         
	 /**
	  * Tell the operator to update to a new time step.
	  * 
	  * This is used to to tell the operator that it has to move to a new time step.
	  */
         virtual void UpdateTime() = 0;
	 
	 /**
	  * Set the Clock of the operator.
	  * 
	  * Do this after everything is initialized and the operator tells that it is time dependent.
	  */
	 virtual void Clock (QDClock *cl)
	 {
	    clock = cl;
	 }
	 
	 /**
          * \return The operators clock.
	  */
	 QDClock* Clock()
	 {
	    return clock;
	 }

	 /**
	  * Complex Bra.Op.Ket value of two Wavefunctions.
          * 
          * \todo This is general function. Move to Base class?
	  */
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet) = 0;
	 
	 /**
	  * Expectation value.
          * 
          * \todo This is general function. Move to Base class?
	  */
	 virtual double Expec(WaveFunction *Psi) = 0;
	 
	 /**
	  * Maximum eigenvalue of the operator.
	  * This is used by e.g. by propagators for Hamiltonian scaling.
	  * Min and Max referes to the absoulute value of the complex number.
	  */
	 virtual dcomplex Emax() = 0;
	 
	 /**
	  * Minimum eigenvalue of the operator.
	  * This is used by e.g. by propagators for Hamiltonian scaling.
	  * Min and Max referes to the absoulute value of the complex number.
	  */
	 virtual dcomplex Emin() = 0;
	 
	 /**
	  * Apply operator to wavefunction.
	  * 
	  * \param destPsi   The result is stored here
	  * \param sourcePsi Input wave function
	  * 
	  * \return destPsi
	  */
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi) = 0;
	 
	 /**
	  * Apply operator to wavefunction in place.
	  */
	 virtual void Apply(WaveFunction *Psi) = 0;
	 	 
	 /**
	  * Copy operator.
	  */
         virtual Operator* operator=(Operator* O) = 0;
	 
 	 /**
	  * Copy function.
	  * 
	  * Alternative to copy operator to provide full
	  * polymorphism.
	  */
	 virtual Operator* Copy(Operator* O) = 0;

	 
	 /**
	  * Apply Operator to another operator.
	  * 
	  * Doesn't always make sense to provide an implementation for
	  * this type of operation. In this case the method should throw
	  * and exception.
	  */
         virtual void Apply(Operator* destOp, Operator* sourceOp)
         {
            throw (EIncompatible("Not implemented"));
         }
         
         /**
          * Apply Operator to another operator.
          *
          * In place variant
          */         
         virtual void Apply(Operator* Op)
         {
            throw (EIncompatible("Not implemented"));
         }
                  
         /**
          * Check if the provided Wavefunction is valid with respect to initialization.
          * 
          * \param Psi The wavefunction to check (may be NULL).
          * 
          * \return true if is compatible with initalized. If incompatible or NULL => false.
          */
         virtual bool Valid(WaveFunction *Psi) = 0;
         	 
   }; /* class Operator */

   
   typedef Collector<Operator> CollectorOp;

   /**
    * C++ operator for application to WFs.
    */
   inline void operator*=(Operator &O, WaveFunction *Psi)
   {
      O.Apply( Psi );
   }
   
   /**
    * C++ operator for application to WFs.
    */
//    inline WaveFunction* operator*(Operator &O, WaveFunction *Psi)
//    {
//       O.Apply( destPsi, Psi );
//       
//       return
//    }

   
   
   /**
    * Matrix-Vector Multiplication for Wavefunctions.
    * 
    * Error checking is done (check sizes).
    * \todo find the right place.
    */
   template <class T, class U>
   void MatrixVectorMult(WaveFunction *Psi, const Matrix<T> &A, const Vector<U> &B)
   {
      lint N;
   
       
      if ( B.size() != A.num_cols() || A.num_cols() != Psi->size() ) {
	 throw( EIncompatible("num_cols != num_elements of vector") );
      }
	  
      if (Psi == NULL)
	 throw ( EIncompatible("Invalid Psi in Mat * Vec") );
      
      N = B.size();

      for (int i=0; i<N; i++){
	 (*Psi)[i] = 0;
	 for (int j=0; j<N; j++){
            (*Psi)[i] += A[i][j] * B[j];
	 }
      }
   
   }
   

   /**
    * Matrix-Matrix Multiplication for Operators.
    * 
    * Error checking is done for A and B but not for O(sizes).
    * \todo find the right place.
    */
   template <class T, class U>
   void MatrixMatrixMult(Matrix<T> *O, Matrix<T> *A, Matrix<U> *B)
   {
       lint AN, BN;
      
       
      if (B->rows() != A->cols()){
	 throw( EIncompatible("num_cols != num_rows of Matrices") );
      }
	  
      AN = A->cols();
      BN = A->rows();
      
      for (int k=0; k < BN ; k++){      //Spalte O
	 for (int i=0; i < BN ; i++){      // Zeile O
	   (*O)(i,k) = 0;
	    for (int j=0; j< AN; j++){   // Spalte A
	       (*O)(i,k) += (*A)(i,j) * (*B)(j,k);
	    }
	 }
      }
   
      
   }
   
}
#endif /* #ifndef OPERATOR_H */


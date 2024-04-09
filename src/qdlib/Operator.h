// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef OPERATOR_H
#define OPERATOR_H

#include "qdlib/WaveFunction.h"
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
      return p; \
} \
   const string& CLASSNAME::Name() \
{ \
      return _name; \
}

// CollectorOp::Instance()->Register(p);

#define QDNG_OPERATOR_NAME_FUNCTION(CLASSNAME) \
   Operator* CLASSNAME::NewInstance() \
{ \
      CLASSNAME *p; \
      p = new CLASSNAME (); \
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

      private:
         bool _recalc; /* Indicates if operator internal should be recalculted or not. */
         WaveFunction *_buf, *_buf1;
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

	       /**
	        * Use the complex conjugate.
	        */
	       bool _conj;

	       /**
	        * Indicates that the operator is hermitian.
	        */
	       bool _hermitian;
      public:
          /**
          * Make class pure virtual
          */
         virtual ~Operator()
         {
            DELETE_WF(_buf);
            DELETE_WF(_buf1);
         }
         
	 /**
	  * Standard constructor
	  */
         Operator() : _recalc(true), _buf(NULL), _buf1(NULL), _isTimedependent(false), clock(NULL),
                      _conj(false), _hermitian(true)
	  {
     }
	  
	 /**
	  * Constructor with full parameter set.
	  */
	 Operator(ParamContainer &params):  _params(params), _isTimedependent(false)
	 {
         }


	 /**
	  * Overloaded new Operator.
	  * Registers the object with the collector.
	  */
	 void* operator new(size_t size)
	 {
	    Operator* p = (Operator*) malloc(size);
	    Collector<Operator>::Instance()->Register(p);
	    return(p);
	 }

	 /**
	  * Removes Object from memory
	  */
	 void operator delete(void *ptr)
	 {
	    free(ptr);
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
     *
     * This is the first Method which must be called.
     */
    virtual void Init(ParamContainer &params) = 0;
	 
	 /**
	  * Initialize the operator with a wavefunction.
	  * 
	  * This must be done after initialization with Init(params) and
	  * before using the Operator with any wave function.
	  * If the Operator is time dependent it needs a valid clock before this
	  * method is called.
	  * Second usage of this method must not lead an error or different behavoir
	  * of the class instance.
	  */
	 virtual void Init(WaveFunction *Psi) = 0;
	 
	 /**
	  * Return the parameter set.
	  */
	 ParamContainer& Params() { return _params; };
	 	 
	 /**
      * Should return a unique ID which identifies the operator.
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
     virtual void UpdateTime() {}
	 
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
	  *  Check if operator is hermitian
	  */
	 bool Hermitian() const { return _hermitian; }

	 /**
	  * Check if complex conjugate is set.
	  */
	 bool Conj() const { return _conj; }

	 /**
	  * Set/unset complex conjugate.
	  *
	  * If set to true than a consecutive apply should carry H^dagger Psi
	  * instead of H Psi
	  */
	 virtual void Conj(bool conj) { _conj = conj; }

	 /**
	  * Complex Bra.Op.Ket value of two Wavefunctions.
     *
     * This is the standard way to this task. Might be overloaded.
	  */
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
	 {
	    if (_buf == NULL) _buf = PsiKet->NewInstance();
	    else _buf->Reaquire();

	    dcomplex result;

	    Apply(_buf, PsiKet);
	    result = *PsiBra * _buf;

	    _buf->Retire();

	    return result;
	 }
	 
	 /**
	  * Expectation value.
     *
     * Gives the expectation value of the operator.
     * If your operator has an complex expectation value the use MatrixElement
     * otherwise the only the real part is returned.
     *
	  */
	 virtual double Expec(WaveFunction *Psi)
	 {
	    return MatrixElement(Psi,Psi).real();
	 }
	 
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
	 virtual void Apply(WaveFunction *Psi)
	 {
      if (_buf == NULL) _buf = Psi->NewInstance();
      else _buf->Reaquire();

      *_buf = Psi;
      Apply(Psi, _buf);

      _buf->Retire();
	 }
	 	 
    /**
     * Use the Apply method of the parent operator.
     *
     * This is needed for the OCT-Coupling operator.
     */
    virtual void ApplyParent(WaveFunction *destPsi, WaveFunction *sourcePsi)
    {
       Apply(destPsi, sourcePsi);
    }

    /**
     * Apply operator to wavefunction and add to destPsi.
     *
     * This method is for optimization purposes.
     */
    virtual void ApplyAdd(WaveFunction *destPsi, WaveFunction *sourcePsi)
    {
       if (_buf1 == NULL) _buf1 = sourcePsi->NewInstance();
       else _buf1->Reaquire();

       Apply(_buf1, sourcePsi);
       AddElements(destPsi, _buf1);

       _buf1->Retire();
    }

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
         
         /**
          * This is intended for non-linear operators which depend on expectation values.
          * 
          * This is needed by recursive operators, to prevent the calculation of non-sense
          * internally used expectation values.
          */
         virtual void RecalcInternals(bool on) { _recalc = on; }
         
         /** Check if Internal should be recalculated. */
         virtual bool RecalcInternals() { return _recalc; }
         	 
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


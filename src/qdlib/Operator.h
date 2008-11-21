#ifndef OPERATOR_H
#define OPERATOR_H

#include "WaveFunction.h"
#include "sys/Exception.h"
#include "sys/QDClock.h"

namespace QDLIB {


   /**
   * Abstract operator class.
   * 
   * Defines what an operator must provide.
   * Since this is an abstract class, all the defined operations apply to pointers.
   * The left handed argument (in this case the operator) has to dereferenced with *O.
   * Application to a WaveFunction could look like:
   * 
   * \code
   * ...
   * Operator *O;
   * WaveFunction *WF1, *WF2;
   *
   * //Initialize O & WF with specific types.
   * ...
   * WF2 = (*O) * WF1;
   * WF2 = (*O) * ((*O) * WF1);  <-- The brackets are important to conserve the right order
   * ...
   * \endcode
   * 
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
	 
	 /**
	  * The operators offset.
	  * for e.g. by the Cheby.
	  */
	 double offset;
      public:
	 /**
          * Make class pure virtual
          */
         virtual ~Operator(){}
         
	 /**
	  * Standard constructor
	  */
	 Operator() :  _isTimedependent(false), clock(NULL){}
	  
	 /**
	  * Constructor with full parameter set.
	  */
	 Operator(ParamContainer &params):  _params(params), _isTimedependent(false){}
         
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
	 void Clock (QDClock *cl)
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
	  */
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet) = 0;
	 
	 /**
	  * Expectation value.
	  */
	 virtual double Expec(WaveFunction *Psi) = 0;
	 
	 /**
	  * Maximum energy of the operator.
	  * This is used by e.g. by propagators for Hamiltonian scaling.
	  */
	 virtual double Emax() = 0;
	 
	 /**
	  * Minimum energy of the operator.
	  * This is used by e.g. by propagators for Hamiltonian scaling.
	  */
	 virtual double Emin() = 0;
	 
	 /**
	  * Apply operator to wavefunction.
	  */
	 virtual WaveFunction* operator*(WaveFunction *Psi) = 0;
	 
	 /**
	  * Apply operator to wavefunction in place.
	  */
	 virtual WaveFunction* operator*=(WaveFunction *Psi) = 0;
	 
	 /**
	  * Copy operator.
	  */
         virtual Operator* operator=(Operator* O) = 0;
	 
	 /**
	  * Apply Operator to another operator.
	  * 
	  * Doesn't always make sense to provide an implementation for
	  * this type of operation. In this case the method should throw
	  * and exception.
	  */
	 virtual Operator* operator*(Operator* O) = 0;
   
	 /**
	  * Change the Operators offset.
	  * 
	  * This is not a usual +=. It sets the value, no addup!
	  */
//          virtual Operator operator+=(const double d) = 0;
// 	 {
// 	    offset = d;
// 	 }
      
	 /**
	  * Change the Operators offset.
	  * 
	  * This is not a usual +=. It sets the value, no addup!
	  */
// 	 virtual Operator operator-=(const double d) = 0;
// 	 {
// 	    offset = -1*d;
// 	 }

         
         
   }; /* class Operator */

   
   
   using namespace TNT;
   
   /**
    * Matrix-Vector Multiplication for Wavefunctions.
    * 
    * Error checking is done (check sizes).
    * \todo find the right place.
    */
   template <class T, class U>
   void MatrixVectorMult(WaveFunction *Psi, const Matrix<T> &A, const Vector<U> &B)
   {
       Subscript N;
   
       
       if ( B.size() != A.num_cols() || A.num_cols() != Psi->size() ) {
	 throw( EIncompatible("num_cols != num_elements of vector") );
      }
	  
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
   void MatrixMatrixMult(Matrix<T> *O, const Matrix<T> *A, const Matrix<U> *B)
   {
       Subscript AN, BN;
      
       
      if (B->num_rows() != A->num_cols()){
	 throw( EIncompatible("num_cols != num_rows of Matrices") );
      }
	  
      cout << "Matrix Ok" << endl;
      AN = A->num_cols();
      BN = A->num_rows();
      
      for (int k=0; k < BN ; k++){      //Spalte O
	 for (int i=0; i < BN ; i++){      // Zeile O
	   ( *O)[i][k] = 0;
	    for (int j=0; j< AN; j++){   // Spalte A
	       (*O)[i][k] += (*A)[i][j] * (*B)[j][k];
	    }
	 }
      }
   
      
   }
   
}
#endif /* #ifndef OPERATOR_H */


#ifndef OPERATOR_H
#define OPERATOR_H

#include "WaveFunction.h"
#include "sys/Exception.h"

namespace QDLIB {

  /**
   * Basic classification for operators.
   */
  enum _OperatorType {differential, spatial, matrix, scalar};
  typedef _OperatorType OperatorType;
  
  
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
      private:
         double _dt;
         int _total_time;
      protected:
	 ParamContainer _params;
	 /**
	  * Indicates time dependence of operator.
	  * Must be set by the implementing class.
	  */
	 bool _isTimedependent;
         
      public:
	 /**
          * Make class pure virtual
          */
         virtual ~Operator(){}
         
	 /**
	  * Standard constructor
	  */
	 Operator() :  _dt(0), _total_time(0), _isTimedependent(false){}
	  
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
          * Set the time step.
          * 
          * A time depenend operator will possibly need this.
          */
         void Dt(double dt) { _dt = dt; }
         
         double Dt() { return _dt; }
	 
         /**
          * Next time step.
          */
         void StepForward() { _total_time++; }
         
         /**
          * Step back in time.
          */
         void StepBackward() { _total_time--; }
         
         /**
          * Set total time.
          */
         void TotalTime(int time) { _total_time = time; }
         
         /**
          * Get the total time.
          */
         double TotalTime() { return  _total_time * _dt; }
               
	 /**
	  * Complex Bra.Op.Ket value of two Wavefunctions.
	  */
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet) = 0;
	 
	 /**
	  * Expectation value.
	  */
	 virtual double Expec(WaveFunction *Psi) = 0;
	 
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
   
         
      
         
         
   }; /* class Operator */

   
   
   using namespace TNT;
   
   /**
    * Matrix-Vector Multiplication for Wavefunctions.
    * 
    * Error checking is done (check sizes).
    */
   template <class T, class U>
   void MatrixVectorMult(WaveFunction *Psi, const Matrix<T> &A, const Vector<U> &B)
   {
       Subscript N;
   
       
      if ( B.size() != A.num_cols() ) {
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
    */
   template <class T, class U>
   void MatrixMatrixMult(Matrix<T> *O, const Matrix<T> *A, const Matrix<U> *B)
   {
       Subscript AN, BN;
      
       
      if (B->num_rows() != A->num_cols()){
	 throw( EIncompatible("num_cols != num_rows of Matrices") );
      }
	  
      
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


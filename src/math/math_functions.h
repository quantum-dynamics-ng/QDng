#ifndef QDLIBMATH_FUNCTIONS_H
#define QDLIBMATH_FUNCTIONS_H
#include "typedefs.h"



namespace QDLIB {
   
   /**
    * Butterworth filter generator.
    * 
    * The template works with every vector type.
    * 
    * For mathematical details \see Wikipedia
    * 
    * @author Markus Kowalewski
    */
   template <class VECT>
   class FunctionGenerator
   {
      public:
         static void BwHighpass(VECT &vec, double center, int order);
         static void BwLowpass(VECT &vec, double center, int order);
         static void Sigmoid(VECT &vec, double x0, double b);
         static void Hann(VECT &vec);
   };

   
   
   /**
    * Generates a butterworth highpass transfer function.
    * For mathematical details \see Wikipedia
    * 
    * \param vec     The vector which should be filled
    * \param center  Center gridpoint 
    * \param order   filter order
    */
   template <class VECT>
   void FunctionGenerator<VECT>::BwHighpass(VECT & vec, double center, int order)
   {
      double k = double(order);
      int size = vec.size();
      
      for (int i=0; i < size; i++){
         vec[i] = 1 / (1 + pow( center / double(i) , 2 * k) );
      }
      
   }

   /**
    * Generates a butterworth highpass transfer function.
    * For mathematical details \see Wikipedia
    * 
    * \param vec     The vector which should be filled
    * \param center  Center gridpoint 
    * \param order   filter order
    */   
   template <class VECT>
   void FunctionGenerator<VECT>::BwLowpass(VECT & vec, double center, int order)
   {
      double k = double(order);
      int size = vec.size();
      
      for (int i=0; i < size; i++){
         vec[i] = 1 / (1 + pow(  double(i) / center, 2 * k) );
      }
      
   }
   
   
   /**
    * Sigmoid function.
    * 
    * \param x0 in units of grid points.
    * \f$ f(x) = \frac{1}{1+e^{-b(x-x_0}}\f$
    */
   template <class VECT>
   void FunctionGenerator<VECT>::Sigmoid(VECT &vec, double x0, double b)
   {
      int size = vec.size();
      for (int n=0; n < size; n++)
         vec[n] = 1/( 1 + exp(-b * (double(n) - x0) ) );
   }
   
   
   /**
    * Hann window.
    *
    * Apply is in place.
    * 
    * \f$ w(n) = 0.5 \left( 1-cos(\frac{2\pi n}{N-1}) \right) \f$
    */
   template <class VECT>
   void FunctionGenerator<VECT>::Hann(VECT &vec)
   {
      int size = vec.size();
      for (int n=0; n < size; n++)
         vec[n] *= 0.5 * (1. - cos( 2*M_PI*double(n) / double(size-1) ));
   }
   
   extern double VecMin(dVec &v);
   extern double VecMax(dVec &v);
   
   extern double VecMax(cVec &v);
   
   extern double Faculty (int arg);
   
   //extern "C" { int dbesj_(double *x, double *alpha, int *n, double *coeffs, int *nz); }
      
   
   /**
    * Diagonalize a symmetric 2x2 matrix
    * 
    * \param m1 Matrix element 11
    * \param m2 Matrix element 22
    * \param m1 Matrix element 12 = 21
    * \param eval Eigenvalues
    */
   inline void diag22symm(double m1, double m2, double m12, double *eval)
   {
      /* Eigenvalues */
      eval[0] = 0.5 * ( m1 + m2 - sqrt( (m1-m2)*(m1-m2) + 4*m12*m12) );
      eval[1] = 0.5 * ( m1 + m2 + sqrt( (m1-m2)*(m1-m2) + 4*m12*m12) );
   }
   
   /**
    * Diagonalize a symmetric 2x2 matrix
    * 
    * \param m1 Matrix element 11
    * \param m2 Matrix element 22
    * \param m1 Matrix element 12 = 21
    * \param eval Eigenvalues
    * \param evec Eigenvectors
    */
   inline void diag22symm(double m1, double m2, double m12, double *eval, double **evec)
   {
      if ( fpclassify(m12) != FP_ZERO ){
         diag22symm(m1, m2, m12, eval);
   
         /* Eigenvectors */
         double theta = (m2-m1)/(2*m12);
         double sgntheta =0.0;
         if(theta>=0.0)
            sgntheta = 1.0;
         else if(theta<0.0)
            sgntheta = -1.0;
         double t = sgntheta/( fabs(theta)+sqrt(theta*theta+1) );
         double cos = 1.0/(sqrt(t*t+1));
         
         if(m2<m1) {
            evec[0][0]=t*cos;
            evec[1][1]=-t*cos;
            evec[1][0]=cos;
            evec[0][1]=cos;  
         }else {
            evec[0][0]= evec[1][1]=cos;
            evec[1][0]=-t*cos;
            evec[0][1]=t*cos;
         }
      } else {
         eval[0] = m1;
         eval[1] = m2;
         evec[0][0] = 1.;
         evec[0][1] = 0.;
         evec[1][0] = 0.;
         evec[1][1] = 1.;
      }
      
   }
}

#endif
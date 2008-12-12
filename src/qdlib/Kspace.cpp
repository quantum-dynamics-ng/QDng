#include "Kspace.h"



namespace QDLIB
{
   /**
    * k-space representation of \f$ \frac{1}{2m} \frac{\partial^2}{\partial x^2} \f$.
    * 
    * \param mass   reduced mass
    * \param length Desired length of the grid.
    * \param Nx     number of points.
    */
   dVec* Kspace::Init1Dd2dx2(const double mass, const double length, const int Nx)
   {
      dVec *kspace;
   
      kspace = new dVec(Nx);
   
      double dp = Dk(length);    // Setup dp for kspace
      dp *= dp;   /* dp^2 */
   
      /* We include all the factors in the k-space function => Do it only once */
      /* The minus cancels with minus from -kx^2. */
      dp *= 0.5 / mass ;
     
      if (Nx % 2 == 0){ /* even + odd grid points */
	 for (int i=0; i < Nx / 2; i++){ //run from [-p..+p]
	    (*kspace)[i] = (double(i) * double(i)) * dp;
	    (*kspace)[Nx - i - 1] = (double(i+1) * double(i+1)) * dp;
	 }
      } else {
	 (*kspace)[0] = 0; 
	 for (int i=1; i < (Nx+1) / 2; i++){ //run from [-p..+p]
	    (*kspace)[i] = (double(i) * double(i)) * dp;
	    (*kspace)[Nx - i] = (double(i+1) * double(i+1)) * dp;
	 }
      }
      
      return kspace;
   }
   
   /**
    * k-space representation of \f$ \frac{\partial}{\partial x} \f$.
    * 
    * No minus is included here! The method returns real numbers
    * which have to be interpreted as imaginary part.
    * 
    * \f$ FT[\frac{\partial}{\partial x}] = ik \f$
    * 
    * \param length Desired length of the grid.
    * \param Nx     number of points.
    */
   dVec* Kspace::Init1Dddx(const double length, const int Nx)
   {
      dVec *kspace;
   
      kspace = new dVec(Nx);
      
      double dp = Dk(length);    // Setup dp for kspace
      
     
      if (Nx % 2 == 0){ /* even + odd grid points */
	 for (int i=0; i < Nx / 2; i++){ //run from [-p..dp] and [0..p-dp]
	    (*kspace)[i] = double(i) * dp;
	    (*kspace)[Nx - i - 1] = double(i+1)* dp;
	 }
      } else {
	 (*kspace)[0] = 0; 
	 for (int i=1; i < (Nx+1) / 2; i++){ //run from [-p..+p]
	    (*kspace)[i] = double(i) * dp;
	    (*kspace)[Nx - i] = double(i+1) * dp;
	 }
      }
      
      return kspace;
   }
}

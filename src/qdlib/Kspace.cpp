// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "Kspace.h"


namespace QDLIB
{
   /**
    * k-space representation of \f$ \frac{1}{2m} \frac{\partial^2}{\partial x^2} \f$.
    * 
    * \param mass   reduced mass
    * \param Grid   Grid system description. 
    * \param dim     Desired Dimension.
    */
   dVec* Kspace::Init1Dd2dx2(const double mass, const GridSystem &Grid, const int dim)
   {
      return Init1Dd2dx2(Grid, dim, -1./(2*mass));
   }

   dVec* Kspace::Init1Dd2dx2(const GridSystem &Grid, const int dim, const double factor)
   {
      dVec *kspace;
      int Nx = Grid.DimSize(dim);

      kspace = new dVec(Nx);

      double dp = Dk(Grid, dim); // Setup dp for kspace

      dp *= dp; /* dp^2 */
      /* We include all the factors in the k-space function => Do it only once */
      /* The minus cancels with minus from -kx^2. */
      dp *= - factor;

      if (Nx % 2 == 0) { /* even + odd grid points */
         for (int i = 0; i < Nx / 2; i++) { //run from [-p..+p]
            (*kspace)[i] = (double(i) * double(i)) * dp;
            (*kspace)[Nx - i - 1] = (double(i + 1) * double(i + 1)) * dp;
         }
      } else {
         (*kspace)[0] = 0;
         for (int i = 1; i < (Nx + 1) / 2; i++) { //run from [-p..+p]
            (*kspace)[i] = (double(i) * double(i)) * dp;
            (*kspace)[Nx - i] = (double(i) * double(i)) * dp;
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
    * \param Grid    Grid system description. 
    * \param dim     Desired Dimension.
    * \param factor  Prefactor to include.
    */
   dVec* Kspace::Init1Dddx(const GridSystem &Grid, const int dim, const double factor)
   {
      dVec *kspace;
      int Nx = Grid.DimSize(dim);
   
      kspace = new dVec(Nx);
      
      double dp = Dk(Grid, dim);    // Setup dp for kspace
      dp *= factor;
     
       if (Nx % 2 == 0){ /* even + odd grid points */
	 for (int i=0; i < Nx / 2; i++){ //run from [-p..dp] and [0..p-dp]
	    (*kspace)[i] = double(i) * dp;
	    (*kspace)[Nx - i - 1] = -1 * double(i+1)* dp;
	 }
       } else {
	 (*kspace)[0] = 0;
	 for (int i=1; i < (Nx+1) / 2; i++){ //run from [-p..+p]
            (*kspace)[i] = double(i) * dp;
            (*kspace)[Nx - i] = -1 * double(i) * dp;
	 }
      }
      
      return kspace;
   }
}

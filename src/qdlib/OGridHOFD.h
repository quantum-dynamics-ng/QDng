/*
 * OGridHOFD.h
 *
 *  Created on: 08.05.2012
 *      Author: markus
 */

#ifndef OGRIDHOFD_H_
#define OGRIDHOFD_H_

#include "qdlib/OGridsystem.h"
#include "qdlib/HOFD.h"

#define HOFD_NDIFF 2      /* Max number of derivates */
#define HOFD_PERDIFF 4    /* number of orders per deriv. */
#define HOFD_NCOEFF 9     /* Max number of coeffs */

namespace QDLIB
{

   /**
    * Higher order finite differenciating scheme.
    *
    * First and second derivitives from 2. - 8. order
    * Parameters:
    *  \li deriv
    *  \li order
    *  \li dim   Dimension to act on
    *  \li mass  If given the prefactor is set to -1/(2m)
    *
    * \author Markus Kowalewksi
    */
   class OGridHOFD : public OGridSystem
   {
      private:
         string _name;
         int _deriv;     /* n-derivative */
         int _order;     /* accuracy */
         int _dim;       /* Dim. to act on. All if -1 */
         double _pfac;   /* contains a prefactor */
         cHOFD *_hofd;    /* diff. operator */
         double _mass[MAX_DIMS];
         WaveFunction *_buf;
      public:
         OGridHOFD();
         virtual ~OGridHOFD();

         /* Interface implementation, Operator */
         virtual Operator* NewInstance();

         virtual void Init(ParamContainer &params);

         virtual void Init(WaveFunction *Psi);

         virtual const string& Name();

         virtual void UpdateTime() {};

         virtual dcomplex Emax();

         virtual dcomplex Emin();

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

   };

} /* namespace QDLIB */
#endif /* OGRIDHOFD_H_ */

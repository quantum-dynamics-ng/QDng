/*
 * HOFD.h
 *
 *  Created on: 09.05.2012
 *      Author: markus
 */

#ifndef HOFD_H_
#define HOFD_H_

#include "qdlib/OGridsystem.h"

#define HOFD_NDIFF 2      /* derivatives: first and second */
#define HOFD_MAXORDER 50  /* Largest FD order */

namespace QDLIB
{

   /**
    * Higher Order Finite Difference Operator.
    */
   template<class T>
   class HOFD
   {
      private:
         int _deriv;
         int _order;
         bool _pbc;    /* periodic boudary conditions */

         /* coefficients for central diff. derivatives.
          * sorted by [first, second deriv., order][n-order/2..0]
          */
         double _cfc[HOFD_MAXORDER][HOFD_MAXORDER/2+1];

         double _pfac;   /* pre-factor */
         GridSystem _grid;
      public:
         HOFD(int diff, int order);
         ~HOFD();

         void SetFactor(double factor) { _pfac = factor; }

         void SetGrid(GridSystem &grid) { _grid = grid; }

         void SetPBC(bool pbc) { _pbc = pbc; }

         void Diff(Vector<T> *res, Vector<T> *in);

         void Diff(Vector<T> *res, Vector<T> *in, int dim);

         void DiffAdd(Vector<T> *res, Vector<T> *in, int dim);
   };

   typedef HOFD<dcomplex> cHOFD;
   typedef HOFD<double> dHOFD;

   /**
    * The derivative and the order of finite diference scheme must be given at construction.
    */
   template<class T>
   HOFD<T>::HOFD(int diff, int order) : _deriv(diff), _order(order), _pbc(true)
   {
      if (_order > HOFD_MAXORDER || _order % 2 != 0 || _order < 2)
         throw(EParamProblem("Invalid order of FD derivative", order));

      if (_deriv > 2 || _deriv < 1)
         throw(EParamProblem("Invalid FD derivative", diff));

      for (int i=0; i < HOFD_MAXORDER; i++)
         for (int j=0; j < HOFD_MAXORDER/2+1; j++)
            _cfc[i][j] = 0;

      /* fill the table with the coefficients - Moved to seperate files for readability of code */
      #include "cfd_table_1.h"
      #include "cfd_table_2.h"

   }

   template<class T>
   HOFD<T>::~HOFD()
   {
   }

   /**
    * Differenciate all dimensions.
    */
   template<class T>
   inline void HOFD<T>::Diff(Vector<T>* res, Vector<T>* in)
   {
      for (int dim=0; dim < _grid.Dim(); dim++){ /* Loop over all dims */
         Diff(res, in, dim);
      }

   }

   /**
    * Differenciate only the given dimension.
    */
   template<class T>
   void HOFD<T>::Diff(Vector<T>* res, Vector<T>* in, int dim)
   {
         double h = 1./pow(_grid.Dx(dim), _deriv) * _pfac;
         int order2 = _order/2;

         _grid.ActiveDim(dim); /* Use index mapping scheme */
         int Nx = _grid.NumActive();

         double df=1;
         if (_deriv % 2 != 0) df=-1;

         T* psi = in->begin(0);
         T* Dpsi = res->begin(0);

         for (int rep=0; rep < _grid.NumOthers(); rep++){ /* replica points in other dims */
            int bi = _grid.IndexBase(rep); /* base index of grid points */
            int lo = _grid.LowOthers();    /* stripe size in lower other dims */
            /* Edge of grid - periodic boundary cond. */
            double* cf = _cfc[(_deriv-1)*HOFD_MAXORDER/2+order2-1];
 
            for (int i = 0; i <= order2-1 ; i++){ /* grid points */
               int cfbb = bi+i*lo;         /* base index for left grid point */
               int cfbe = bi+(Nx-i-1)*lo;  /* base index for right grid point */
               int step = 0;
               T db = cf[order2] * psi[cfbb];
               T de = cf[order2] * psi[cfbe];

               /* Left (right) loop */
               step = lo;
               for (int j=order2-1; j >= order2-i; j--){
                  db += cf[j] * psi[cfbb-step];
                  de += df * cf[j] * psi[cfbe+step];
                  step += lo;
               }

               /* PBC loop */
               step = lo;
               for (int j=order2-i-1; j >= 0; j--){
                  db += cf[j] * psi[bi+Nx*lo-step];
                  de += df * cf[j] * psi[bi+step-lo];
                  step += lo;
               }

               /* Right (left) loop */
               step = lo;
               for (int j=order2-1; j >= 0; j--){
                  db += df * cf[j] * psi[cfbb+step];
                  de += cf[j] * psi[cfbe-step];
                  step += lo;
               }

               Dpsi[cfbb] = h * db;
               Dpsi[cfbe] = h * de;
            }

            /* Center of grid */
            for (int i =order2; i < Nx - order2; i++){ /* grid points */
               int cfb = bi+i*lo;  /* base index for diff. */
               int step = lo;
               T d = cf[order2] * psi[cfb];
               for (int j=order2-1; j >= 0 ; j--){ /* coeffs */
                  d += df * cf[j] * psi[cfb+step];
                  d += cf[j] * psi[cfb-step];
                  step += lo;
               }
               Dpsi[cfb] = d * h;
            }
         }
   }

   /**
    * Differenciate only the given dimension and add to res.
    */
   template<class T>
   void HOFD<T>::DiffAdd(Vector<T>* res, Vector<T>* in, int dim)
   {
         double h = 1./pow(_grid.Dx(dim), _deriv) * _pfac;
         int order2 = _order/2;

         _grid.ActiveDim(dim); /* Use index mapping scheme */
         int Nx = _grid.NumActive();

         double df=1;
         if (_deriv % 2 != 0) df=-1;

         T* psi = in->begin(0);
         T* Dpsi = res->begin(0);

         for (int rep=0; rep < _grid.NumOthers(); rep++){ /* replica points in other dims */
            int bi = _grid.IndexBase(rep); /* base index of grid points */
            int lo = _grid.LowOthers();    /* stripe size in lower other dims */

            double* cf = _cfc[(_deriv-1)*HOFD_MAXORDER/2+order2-1];

            /* Edge of grid */
            for (int i = 0; i <= order2-1 ; i++){ /* grid points */
               int cfbb = bi+i*lo;         /* base index for left grid point */
               int cfbe = bi+(Nx-i-1)*lo;  /* base index for right grid point */
               int step = lo;
               T db = cf[order2] * psi[cfbb];
               T de = cf[order2] * psi[cfbe];

               /* Left (right) loop */
               for (int j=order2-1; j >= order2-i; j--){
                  db += cf[j] * psi[cfbb-step];
                  de += df * cf[j] * psi[cfbe+step];
                  step += lo;
               }

               /* PBC loop */
               step = lo;
               for (int j=order2-i-1; j >= 0; j--){
                  db += cf[j] * psi[bi+Nx*lo-step];
                  de += df * cf[j] * psi[bi+step-lo];
                  step += lo;
               }

               /* Right (left) loop */
               step = lo;
               for (int j=order2-1; j >= 0; j--){
                  db += df * cf[j] * psi[cfbb+step];
                  de += cf[j] * psi[cfbe-step];
                  step += lo;
               }

               Dpsi[cfbb] += h * db;
               Dpsi[cfbe] += h * de;
            }

            /* Center of grid */
            for (int i =order2; i < Nx - order2; i++){ /* grid points */
               int cfb = bi+i*lo;  /* base index for diff. */
               int step = lo;
               T d = cf[order2] * psi[cfb];
               for (int j=order2-1; j >= 0 ; j--){ /* coeffs */
                  d += df * cf[j] * psi[cfb+step];
                  d += cf[j] * psi[cfb-step];
                  step += lo;
               }
               Dpsi[cfb] += d * h;
            }
         }
   }

} /* namespace QDLIB */
#endif /* HOFD_H_ */

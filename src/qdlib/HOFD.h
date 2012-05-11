/*
 * HOFD.h
 *
 *  Created on: 09.05.2012
 *      Author: markus
 */

#ifndef HOFD_H_
#define HOFD_H_

#include "qdlib/OGridsystem.h"

#define HOFD_NDIFF 2      /* Max number of derivates */
#define HOFD_PERDIFF 4    /* number of orders per deriv. */
#define HOFD_NCOEFF 9 /* Max number of coeffs */

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
         double* _cf;    /* coefficients */
         double _pfac;   /* pre-factor */
         GridSystem _grid;
      public:
         HOFD(int diff, int order);
         ~HOFD();

         void SetFactor(double factor) { _pfac = factor; }

         void SetGrid(GridSystem &grid) { _grid = grid; }

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
   HOFD<T>::HOFD(int diff, int order) : _deriv(diff), _order(order), _cf(NULL)
   {
      if (_order > 8 || _order % 2 != 0 || _order < 1)
         throw(EParamProblem("Invalid order of FD derivative", order));

      _cf = new double[order+1];
      switch (diff) {
         case 1: switch (order) {
            case 2:
               _cf[0] = -1./2; // Deriv. 1, Accu. 2
               _cf[1] = 0;
               _cf[2] =  1./2;
               break;
            case 4:
               _cf[0] =  1./12.; // Deriv. 1, Accu. 4
               _cf[1] = -2./3.;
               _cf[2] = 0;
               _cf[3] =  2./3.;
               _cf[4] = -1./12.;
               break;
            case 6:
               _cf[0] =  -1./60.; // Deriv. 1, Accu. 6
               _cf[1] =   3./20.;
               _cf[2] =  -3./4.;
               _cf[3] = 0;
               _cf[4] =   3./4.;
               _cf[5] =  -3./20.;
               _cf[6] =   1./60.;
               break;
            case 8:
               _cf[0] =  1./280.; // Deriv. 1, Accu. 8
               _cf[1] = -4./105.;
               _cf[2] =  1./5.;
               _cf[3] =  -4./5.;
               _cf[4] = 0;
               _cf[5] =  4./5.;
               _cf[6] = -1./5.;
               _cf[7] =  4./105.;
               _cf[8] =  -1./280.;
               break;
            default:
               throw(EParamProblem("Invalid order of FD derivative", order));
         }
         break;
         case 2: switch(order) {
            case 2:
               _cf[0] = _cf[2] = 1.; // Deriv. 2, Accu. 2
               _cf[1] =         -2;
               break;
            case 4:
               _cf[0] = _cf[4] = -1./12.; // Deriv. 2, Accu. 4
               _cf[1] = _cf[3] =  4./3.;
               _cf[2] =          -5./2.;
               break;
            case 6:
               _cf[0] = _cf[6] =  1./90.; // Deriv. 2, Accu. 6
               _cf[1] = _cf[5] = -3./20.;
               _cf[2] = _cf[4] =  3./2.;
               _cf[3] =        -49./18.;
               break;
            case 8:
               _cf[0] = _cf[8] = -1./560.; // Deriv. 2, Accu. 8
               _cf[1] = _cf[7] =  8./315.;
               _cf[2] = _cf[6] = -1./5.;
               _cf[3] = _cf[5] =  8./5.;
               _cf[4] =        -205./72.;
               break;
            default:
               throw(EParamProblem("Invalid order of FD derivative", order));
         }
         break;
      default:
         throw(EParamProblem("Invalid FD derivative", diff));
      }
   }

   template<class T>
   HOFD<T>::~HOFD()
   {
      if (_cf != NULL) delete[] _cf;
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
   inline void HOFD<T>::Diff(Vector<T>* res, Vector<T>* in, int dim)
   {
         double h = 1./pow(_grid.Dx(dim), _deriv) * _pfac;

         _grid.ActiveDim(dim); /* Use index mapping scheme */
         int Nx = _grid.NumActive();


         T* psi = in->begin(0);
         T* Dpsi = res->begin(0);

         for (int rep=0; rep < _grid.NumOthers(); rep++){ /* replica points in other dims */
            int bi = _grid.IndexBase(rep); /* base index of grid points */
            int lo = _grid.LowOthers();    /* stripe size in lower other dims */

            /* Edge of grid */
            for (int i = 0; i <= _order/2-1 ; i++){ /* grid points */
               T db = 0;
               T de = 0;
               int cfb = i - _order/2;  /* base index for diff. */
               for(int j=_order/2-i; j <= _order; j++){ /* coeffs */
                  db += _cf[j] * psi[bi+(cfb + j) * lo];
                  de += _cf[_order-j] * psi[bi+(Nx-1 - (cfb + j)) * lo];
               }
               Dpsi[bi+i * lo] = h * db;
               Dpsi[bi+(Nx-i-1) * lo] = h * de;
            }

            /* Center of grid */
            for (int i =_order/2; i < Nx - _order/2; i++){ /* grid points */
               T d = 0;
               int cfb = i - _order/2;  /* base index for diff. */
               for(int j=0; j <= _order; j++){ /* coeffs */
                  d += _cf[j] * psi[bi+(cfb  + j) * lo];
               }
               Dpsi[bi+i*lo] = d * h;
            }
         }
   }

   /**
    * Differenciate only the given dimension and add to res.
    */
   template<class T>
   inline void HOFD<T>::DiffAdd(Vector<T>* res, Vector<T>* in, int dim)
   {
         double h = 1./pow(_grid.Dx(dim), _deriv) * _pfac;
         int order2 = _order/2;

         _grid.ActiveDim(dim); /* Use index mapping scheme */
         int Nx = _grid.NumActive();

         T* psi = in->begin(0);
         T* Dpsi = res->begin(0);

         for (int rep=0; rep < _grid.NumOthers(); rep++){ /* replica points in other dims */
            int bi = _grid.IndexBase(rep); /* base index of grid points */
            int lo = _grid.LowOthers();    /* stripe size in lower other dims */

            /* Edge of grid */
            for (int i = 0; i <= order2-1 ; i++){ /* grid points */
               T db = 0;
               T de = 0;
               int cfb = i - order2;  /* base index for diff. */
               for(int j=order2-i; j <= _order; j++){ /* coeffs */
                  db += _cf[j] * psi[bi+(cfb + j) * lo];
                  de += _cf[_order-j] * psi[bi+(Nx-1 - (cfb + j)) * lo];
               }

               Dpsi[bi+i * lo] += db * h;
               Dpsi[bi+(Nx-i-1) * lo] += de * h;
            }

            /* Center of grid */
            for (int i =order2; i < Nx - order2; i++){ /* grid points */
               T d = 0;
               int cfb = i - order2;  /* base index for diff. */
               for(int j=0; j <= _order; j++){ /* coeffs */
                  d += _cf[j] * psi[bi+(cfb  + j) * lo];
               }
               Dpsi[bi+i * lo] += d * h;
            }
         }
   }

} /* namespace QDLIB */
#endif /* HOFD_H_ */

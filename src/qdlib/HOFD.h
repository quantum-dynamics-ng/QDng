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

           void _CheckSetup();

           void _DiffBlock2DDxDy(T *out, T *buf, T *in, int order2, int point,
                    int N, int lo1, int lo2,  int bs1, int bs2,
                    double* cf, double df, double h1, double h2 ,bool acc);

           void _DiffBlock2D(T *out, T *in, int order2, int point,
                    int N, int lo1, int lo2,  int bs1, int bs2,
                    double* cf, double df, double h ,bool acc);
      public:
         HOFD(int diff, int order);
         ~HOFD();

         void SetFactor(double factor) { _pfac = factor; }

         void SetGrid(GridSystem &grid) { _grid = grid; }

         void SetPBC(bool pbc) { _pbc = pbc; }

         void SetDeriv(int deriv) { _deriv=deriv; _CheckSetup(); }

         void SetOrder(int order) { _order=order; _CheckSetup(); }

         static int GetMaxOrder() { return HOFD_MAXORDER; }

         void Diff(Vector<T> *res, Vector<T> *in, bool acc = false);

         void Diff(Vector<T> *res, Vector<T> *in, int dim, bool acc = false);

         void Diff(Vector<T>* res, Vector<T>* buf, Vector<T>* in, int dim1, int dim2, bool acc = false);

   };

   typedef HOFD<dcomplex> cHOFD;
   typedef HOFD<double> dHOFD;

   template<class T>
   void HOFD<T>::_CheckSetup()
   {
      if (_order > HOFD_MAXORDER || _order % 2 != 0 || _order < 2)
         throw(EParamProblem("Invalid order of FD derivative", _order));

      if (_deriv > 2 || _deriv < 1)
         throw(EParamProblem("Invalid FD derivative", _deriv));
   }

   /**
    * The derivative and the order of finite diference scheme must be given at construction.
    */
   template<class T>
   HOFD<T>::HOFD(int diff, int order) : _deriv(diff), _order(order), _pbc(true), _pfac(1)
   {
      _CheckSetup();

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
   inline void HOFD<T>::Diff(Vector<T>* res, Vector<T>* in, bool acc)
   {
      for (int dim=0; dim < _grid.Dim(); dim++){ /* Loop over all dims */
         Diff(res, in, dim);
      }

   }

   /**
    * Differenciate only the given dimension.
    *
    * \param res
    * \param in
    * \param dim  Differenciate along dimension.
    * \param acc  Add result to output.
    */
   template<class T>
   void HOFD<T>::Diff(Vector<T>* res, Vector<T>* in, int dim, bool acc)
   {
         double h = 1./pow(_grid.Dx(dim), _deriv) * _pfac;
         int order2 = _order/2;

         _grid.ActiveDim(dim); /* Use index mapping scheme */
         int Nx = _grid.NumActive();

         double df=1;
         if (_deriv % 2 != 0) df=-1;

         T* psi = in->begin(0);
         T* Dpsi = res->begin(0);

#ifdef _OPENMP
#pragma  omp parallel for
#endif
         for (int rep=0; rep < _grid.NumOthers(); rep++){ /* replica points in other dims */
            int bi = _grid.IndexBase(rep); /* base index of grid points */
            int lo = _grid.LowOthers();    /* stripe size in lower other dims */

            double* cf = _cfc[(_deriv-1)*HOFD_MAXORDER/2+order2-1];
 
            /* Edge of grid - evtl. periodic boundary cond. */
            for (int i = 0; i <= order2-1 ; i++){ /* grid points */
               int cfbb = bi+i*lo;         /* base index for left grid point */
               int cfbe = bi+(Nx-i-1)*lo;  /* base index for right grid point */
               int step = 0;
               T db = cf[order2] * psi[cfbb];
               T de = cf[order2] * psi[cfbe];

               /* Left (right) loop */
               step = lo;
               for (int j=order2-1; j >= order2-i; j--){
                  db += cf[j] * (psi[cfbb-step] + df * psi[cfbb+step]);
                  de += cf[j] * (psi[cfbe-step] + df * psi[cfbe+step]);
                  step += lo;
               }

               /* Right (left) loop */
               for (int j=order2-1-i; j >= 0; j--){
                  db += df * cf[j] * psi[cfbb+step];
                  de += cf[j] * psi[cfbe-step];
                  step += lo;
               }

               /* PBC loop */
               if (_pbc){
                  step = lo;
                  for (int j=order2-i-1; j >= 0; j--){
                     db += cf[j] * psi[bi+Nx*lo-step];
                     de += df * cf[j] * psi[bi+step-lo];
                     step += lo;
                  }
               }

               if (acc){
                  Dpsi[cfbb] += h * db;
                  Dpsi[cfbe] += h * de;
               } else {
                  Dpsi[cfbb] = h * db;
                  Dpsi[cfbe] = h * de;
               }
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

               if (acc)
                  Dpsi[cfb] += d * h;
               else
                  Dpsi[cfb] = d * h;
            }
         }
   }

   /**
    * Mixed derivative.
    *
    * \param res
    * \param in
    * \param dim  Differenciate along dimension.
    * \param acc  Add result to output.
    */
   template<class T>
   void HOFD<T>::Diff(Vector<T>* res, Vector<T>* buf, Vector<T>* in, int dim1, int dim2, bool acc)
   {
      double h1 = 1./pow(_grid.Dx(dim1), _deriv) * _pfac;
      double h2 = 1./pow(_grid.Dx(dim2), _deriv) * _pfac;
      const int order2 = _order/2;

      _grid.ActiveDim(dim1); /* Use index mapping scheme */
      int lo1 = _grid.LowOthers();
      int Nx1 = _grid.DimSize(dim1);
      _grid.ActiveDim(dim2); /* Use index mapping scheme */
      int lo2 = _grid.LowOthers();
      int Nx2 = _grid.DimSize(dim2);

      /* choose block sizes */
      int bs2;
      int bs2beg;

      if (order2 < 16) bs2beg=16; /* bs2 must be at least order/2*/
      else bs2beg=order2;

      for (bs2=bs2beg; bs2 < Nx2; bs2++)
         if (Nx2%bs2 == 0) break;

      int bs1;
      for (bs1=16; bs1 < Nx1; bs1++)
         if (Nx1%bs1 == 0) break;

      double df=1;
      if (_deriv % 2 != 0) df=-1;

      double* cf = _cfc[(_deriv-1)*HOFD_MAXORDER/2+order2-1];

      T* psi = in->begin(0);
      T* Bpsi = buf->begin(0);
      T* Dpsi = res->begin(0);

      int N = _grid.Size();
      int Nrep = _grid.NumOthers(dim1, dim2);
      int lorep = _grid.LowOthers(dim1, dim2);
      int base=0;
      cout << "Size: " << Nx1 << "x" << Nx2 << endl;
      cout << "BS: " << bs1 << "x" << bs2 << endl;
      cout << "stripe: " << lo1 << "x" << lo2 << endl;
#ifdef _OPENMP
#pragma  omp parallel for
#endif
      for (int rep=0; rep < Nrep; rep++){ /* loop over all 2D slices */
#ifdef _OPENMP
#pragma  omp parallel for
#endif
         for (int blk1=0; blk1 < Nx1/bs1; blk1++){ /* Loop over lead. dim */
            int base1 = base + blk1*bs1*lo1;

            /* dx1 in block - end of grid to get PBC right */
            _DiffBlock2D(Bpsi, psi,  order2, (N+base1 - order2*lo2)%N,
                        N, lo1, lo2,  bs1, order2,
                        cf, df, h1 , acc);

            /* first block along dim1 */
            _DiffBlock2D(Bpsi, psi, order2, base1,
                     N, lo1, lo2, bs1, bs2,
                     cf, df, h1, acc);

            for (int blk2=0; blk2 < (Nx2-order2)/bs2-1; blk2++){ /* loop over sec. dim*/
               int base2 = base1 + blk2*bs2*lo2;

               cout << " blk1, blk2 " << blk1 << " " << blk2+1 <<" "<< blk2 <<" "<< base2+bs2*lo2 << " "<< base2 << endl;
               /* dx1 in block + 1*/
               _DiffBlock2D(Bpsi, psi, order2, base2+bs2*lo2,
                        N, lo1, lo2, bs1, bs2,
                        cf, df, h1, acc);
               /* dx2 in actual block */
               cout << "dx2\n";
               _DiffBlock2D(Dpsi, Bpsi, order2, base2,
                        N, lo2, lo1, bs2, bs1,
                        cf, df, h2, acc);
            }
            /* dx2 in last block */
            _DiffBlock2D(Dpsi, Bpsi, order2, base1+((Nx2-order2)/bs2-1) * bs2 * lo2,
                     N, lo2, lo1, bs2, bs1,
                     cf, df, h2, acc);

            /* dx2 in block - end of grid (left over from pbc condition)*/
            _DiffBlock2D(Dpsi, Bpsi,  order2, (N+base1-order2*lo2)%N,
                        N, lo2, lo1, order2, bs1,
                        cf, df, h2 , acc);


         }
         base += lorep;
      }
   }

   template<class T>
   void HOFD<T>::_DiffBlock2DDxDy(T *out, T *buf, T *in, int order2, int point,
            int N, int lo1, int lo2,  int bs1, int bs2,
            double* cf, double df, double h1, double h2 ,bool acc)
   {
         /* dx1 */
         _DiffBlock2D(buf, in, order2, point,
                  N, lo1, lo2, bs1, bs2,
                  cf, df, h1, acc);
         /* dx2 */
         _DiffBlock2D(out, buf, order2, point,
                  N, lo2, lo1, bs2, bs1,
                  cf, df, h2, acc);

   }

   template<class T>
   void HOFD<T>::_DiffBlock2D(T *out, T *in, int order2, int point,
            int N, int lo1, int lo2,  int bs1, int bs2,
            double* cf, double df, double h ,bool acc)
   {
      int base2=point;

      for (int x2=0; x2 < bs2; x2++){
         int base = base2;
         for (int x1=0; x1 < bs1; x1++){
            int step=lo1;
            T d = cf[order2] * in[base];
            for (int j=order2-1; j >= 0 ; j--){ /* coeffs */
               d += df * cf[j] * in[(base+step)%N]; /* wrap around indices for PBC */
               d += cf[j] * in[(N+base-step)%N];
               step += lo1;
            }
            if (acc){
               out[base] += h * d;
            } else {
               out[base] = h * d;
            }
            base += lo1;
         }
         base2 += lo2;
      }
   }
} /* namespace QDLIB */
#endif /* HOFD_H_ */

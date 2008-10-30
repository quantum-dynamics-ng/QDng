#ifndef WFGRID1D_H
#define WFGRID1D_H

#include "WFGridSystem.h"

namespace QDLIB {
   
   /**
    * Simple 1D cartesian grid system.
    * 
    * The k-space is initalized with \f$ dp = \frac{2\pi}{L}\f$ where
    * L is the length of the drid.
    * 
    * Takes the following params:
    * - xmin
    * - xmax
    * - Nx
    */
   class WFGrid1D : public WFGridSystem
   {
      private:
         ParamContainer _params;
         string _name;
         double _dx;
         double _l;
	 double _dp;
         int _Nx;
         double _xmin;
         double _xmax;
      public:
         
         WFGrid1D() : _name("WFGrid1D"), _dx(0), _dp(0), _Nx(0), _xmin(0), _xmax(0) {}
         
         WFGrid1D(ParamContainer &params);
         
         WFGrid1D& operator=(WFGrid1D &Psi);
         
         double Dx();
         void Dx(double dx);
         
	 double Dp();
	 void Dp(double dp);
	 
	 
         double xmin();
         void xmin(double xmin);
         
         double xmax();
         void xmax(double xmax);
         
         int Nx();
         void Nx(int nx);
         
         
         /* Interface implementation */
         virtual WaveFunction* NewInstance();

         virtual void Init(ParamContainer &params);
	 
         virtual const string& Name();
	 
         virtual int ID();
	    	 
         virtual double Norm();
         virtual void Normalize();

         virtual WaveFunction* operator=(WaveFunction* Psi);
	 
         virtual dcomplex operator*(WaveFunction* Psi);
   };
   
} /* namespace QDLIB */


#endif /* #ifndef WFGRID1D_H */

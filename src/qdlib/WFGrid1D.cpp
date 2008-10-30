#include "WFGrid1D.h"
#include "sys/Exception.h"

namespace QDLIB {
   /* Interface implementation */

   
   WaveFunction* WFGrid1D::NewInstance()
   {
      WFGrid1D *r =  new WFGrid1D();
      r->Init(_params);
      return r;
   }

   void WFGrid1D::Init(ParamContainer &params)
   {
      
      /* Set parameters of mother class */
      WFGridSystem::Dim(1);
      int *sizes = WFGridSystem::DimSizes();
      
      _params = params;
            
      _params.GetValue("xmin", _xmin);
      _params.GetValue("xmax", _xmax);
      _params.GetValue("Nx", _Nx);

      
      if (_Nx == 0) throw ( EParamProblem("Zero elements grid defined") );
      sizes[0] = _Nx;
      
      _dx = _xmax - _xmin;
      if (_dx == 0) throw ( EParamProblem("Zero length grid defined") );
      
      _dp = (2*PI) / _dx;     // K-Space -> dp =  2pi / L
      
      _dx = _dx / (double(_Nx) - 1) ;
     

      cVec::newsize(_Nx);
   }

   const string& WFGrid1D::Name()
   {
      return _name;
   }
	 
   int WFGrid1D::ID()
   {
      return 0;
   }
	    	 
   double WFGrid1D::Norm()
   {
      dcomplex d(0,0);
      
      for (int i=0; i < cVec::size(); i++){
         d += (*this)[i].conj() * (*this)[i];
      }
      
      if ( isKspace() ) /* k-Space - real space ? */
	 d *= _dp;
      else
         d *= _dx;
      
      return d.real();
   }
   
   void WFGrid1D::Normalize()
   {
      (*this) * Norm();
   }

   WaveFunction* WFGrid1D::operator=(WaveFunction* Psi)
   {
      
      _params =  Psi->Params();
            
      Init(_params);
      
      *(cVec*) this = (cVec) *Psi;
      
      return this;
   }
	 
   dcomplex WFGrid1D::operator*(WaveFunction* Psi)
   {
      dcomplex d(0,0);
      
      for (int i=0; i < cVec::size(); i++){
         d += ((*this)[i]).conj() * (*Psi)[i];
      }
      
      if ( isKspace() ) /* k-Space - real space ? */
	 d *= _dp;
      else
	 d *= _dx;
      
      
      return d;
   }
   
   /**
    * Grid spacing.
    */
   double WFGrid1D::Dx()
   {
      return _dx;
   }
   
   void WFGrid1D::Dx(double dx)
   {
      _dp = _dx / (2*PI);     // K-Space -> delta p
      _dx = dx;
      
   }
   
   double WFGrid1D::Dp()
   {
      return _dp;
   }
   
   /**
    * Set dp-Kspace length.
    * 
    * \todo think about. does it make sense? => make consistent.
    */
   void WFGrid1D::Dp(double dp)
   {
      _dp = dp;
   }
   
   
   double WFGrid1D::xmin()
   {
      return _xmin;
   }
   
   void WFGrid1D::xmin(double xmin)
   {
      _xmin = xmin;
   }
   
   double WFGrid1D::xmax()
   {
      return _xmax;
   }
   
   void WFGrid1D::xmax(double xmax)
   {
      _xmax = xmax;
   }
	 
   int WFGrid1D::Nx()
   {
      return cVec::size();
   }
   
   /**
    * Resize the grid.
    * 
    * Old data will be deleted.
    */
   void WFGrid1D::Nx(int nx)
   {
      cVec::newsize(nx);
   }
   
   /**
    * Native copy operator.
    */
   WFGrid1D& WFGrid1D::operator=(WFGrid1D &Psi)
   {
      _params = Psi._params;
      Init(_params);
      
      
      *(cVec*) this = (cVec) Psi;
      
      return *this;
   }
   
} /* namespace QDLIB */

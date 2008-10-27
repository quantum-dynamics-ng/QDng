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
      
      if (_Nx == 0) throw ( EParamProblem("Zero elemets grid defined") );
      sizes[1] = _Nx;
      
      _dx = _xmax - _xmin;
      if (_dx == 0) throw ( EParamProblem("Zero length grid defined") );
      
      _dx = (double(_Nx) - 1) / _dx;
      
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
      dcomplex d;
      
      for (int i=0; i < cVec::size(); i++){
         d += (*this)[i].conj() * (*this)[i];
      }
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
      dcomplex d;
      
      for (int i=0; i < cVec::size(); i++){
         d += (*this)[i].conj() * (*Psi)[i];
      }
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
      _dx = dx;
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
   
   void WFGrid1D::Nx(int nx)
   {
      cVec::newsize(nx);
   }
   
   /**
    * Native copy operator.
    */
   WFGrid1D& WFGrid1D::operator=(WFGrid1D &Psi)
   {
      _params = Psi.Params();
      Init(_params);
      
      (cVec) *this = (cVec) Psi;
      
      return *this;
   }
   
} /* namespace QDLIB */

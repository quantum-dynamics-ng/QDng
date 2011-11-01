#include "WFGridCartesian.h"
#include "tools/Exception.h"

namespace QDLIB {

   
   QDNG_WAVEFUNCTION_NEW_INSTANCE_FUNCTION(WFGridCartesian)
	 
   WFGridCartesian::WFGridCartesian()
      : WFGridSystem(), _name("WFGridCartesian")
   {
   }
   
   
   WFGridCartesian::~WFGridCartesian()
   {
   }

   void WFGridCartesian::Init(ParamContainer &params)
   {
      string s;
      int n;
      double d;
   
      _params = params;
      
      /* Get all the params from the ParamContainer */
      _params.GetValue( "dims", n );
      if ( n <= 0 ) throw ( EParamProblem("Zero number of dimension defined") );
      GridSystem::Dim(n);
  
      int i=0;
      char c[256];
      sprintf (c, "%d", i);
      s = string("N") + string(c);
      while ( _params.isPresent(s) && i < GridSystem::Dim()){
	 _params.GetValue( string("N") + string(c), n);
	 
	 if ( n <= 0) throw ( EParamProblem("Zero elements grid defined") );
	 GridSystem::DimSize(i, n);
	 _params.GetValue( string("xmin") + string(c), d);
	 GridSystem::Xmin(i, d);
	 _params.GetValue( string("xmax") + string(c), d);
	 GridSystem::Xmax(i, d);
	 if ( (GridSystem::Xmax(i) - GridSystem::Xmin(i)) <= 0 )
	    throw ( EParamProblem("Zero length grid defined") );

	 i++;
	 sprintf (c, "%d", i);
      }
      
      cVec::newsize(GridSystem::Size());
      
      WFGridSystem::Init(_params);
   }

   const string & WFGridCartesian::Name()
   {
      return _name;
   }


   double WFGridCartesian::Norm()
   {
      dcomplex c(0,0);
   
      c = *this * this;
   
      return sqrt(c.real());
   }

   void WFGridCartesian::Normalize()
   {
      MultElements( (cVec*) this, 1/Norm() );
   }

   WaveFunction * WFGridCartesian::operator =(WaveFunction * Psi)
   {
      /* WFGridSystem already knows what to copy */
      *((WFGridSystem*) this) = dynamic_cast<WFGridSystem*> (Psi);
      
      return this;
   }

   dcomplex WFGridCartesian::operator *(WaveFunction *Psi)
   {
      dcomplex c(0,0);
      dcomplex cglob(0,0);
      
      int s;
      lint size = lsize();
      dcomplex *a, *b;
      for (s=0; s < strides(); s++){
	 a = begin(s);
         b = Psi->begin(s);
	 int i;
#ifdef _OPENMP
#pragma omp parallel shared(cglob) private(i) firstprivate(c)
{   
#pragma omp  for nowait
#endif
	 for(i=0; i < size; i++){
	    c += a[i].conj() * b[i];
	 }
#ifdef _OPENMP
#pragma omp critical
	 {
	 cglob += c;
	 }
}
#else
         cglob = c;
#endif
      }
   
      /* k-space has different Norm! */
      if (IsKspace())
      {
	 for(int i=0; i < GridSystem::Dim(); i++){
	    cglob *= 2*M_PI / (GridSystem::Xmax(i) - GridSystem::Xmin(i)) ;
	 }

      } else {
	 for(int i=0; i < GridSystem::Dim(); i++){
	    cglob *= GridSystem::Dx(i);
	 }
	 
      }
      
      return cglob;
   }


   dcomplex QDLIB::WFGridCartesian::Integral()
   {
      dcomplex c(0,0);
      dcomplex cglob(0,0);
      
   
      lint size = lsize();
      dcomplex *a;

      for (int s = MPIrank(); s < strides(); s += MPIsize()) {
         a = begin(s);
         int i;
#ifdef _OPENMP
#pragma omp parallel shared(cglob) private(i) firstprivate(c)
         {   
#pragma omp  for nowait
#endif
            for(i=0; i < size; i++){
               c += a[i];
            }
#ifdef _OPENMP
#pragma omp critical
            {
               cglob += c;
            }
         }
#else
         cglob = c;
#endif
      }

#ifdef HAVE_MPI
      if (GetComm() != NULL){
         GetComm()->Allreduce(&cglob, &c, 1, MPIdcomplex::Instance(), MPI::SUM);
         cglob = c;
      }
#endif

      /* k-space has different Norm! */
      if (IsKspace())
      {
         for(int i=0; i < GridSystem::Dim(); i++){
            cglob *= 2*M_PI / (GridSystem::Xmax(i) - GridSystem::Xmin(i)) ;
         }

      } else {
         for(int i=0; i < GridSystem::Dim(); i++){
            cglob *= GridSystem::Dx(i);
         }
         
      }
      
      return cglob;
   }

}


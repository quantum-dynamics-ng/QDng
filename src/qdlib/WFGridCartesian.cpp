#include "WFGridCartesian.h"
#include "tools/Exception.h"

#include "google/protobuf/message.h"

using google::protobuf::Message;

namespace QDLIB {

   

   WaveFunction* WFGridCartesian::NewInstance()
   {
      WFGridCartesian* p = new WFGridCartesian(static_cast<GridSystem&>(*this));
      return p;
   }
	 
   WFGridCartesian::WFGridCartesian()
      : WFGridSystem(), _name("WFGridCartesian")
   {
   }
   
   WFGridCartesian::WFGridCartesian(GridSystem& grid)
      : WFGridSystem(grid), _name("WFGridCartesian")
   {
     grid_sys.DumpParams(_params);
   }
   
   WFGridCartesian::~WFGridCartesian()
   {
   }


   void WFGridCartesian::Init(ParamContainer &params)
   {
      _params = params;

      grid_sys.InitFromParamContainer(params);
      
      cVec::newsize(GridSystem::Size());
      
      WFGridSystem::Init(_params);
   }

   const string & WFGridCartesian::Name()
   {
      return _name;
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
      for (s = MPIrank(); s < strides(); s += MPIsize()) {
         a = begin(s);
         b = Psi->begin(s);
#ifdef HAVE_SSE2
            m128dc vc(c);
#ifdef _OPENMP
#pragma omp parallel shared(cglob) firstprivate(vc,c)
         {
#pragma omp  for nowait
#endif
            for (int i = 0; i < size; i++)
               vc += m128dc(&(a[i])).conj() * m128dc(&(b[i]));

            vc.Store(c);
#else
#ifdef _OPENMP
#pragma omp parallel shared(cglob) firstprivate(c)
         {
#pragma omp  for nowait
#endif
            for (int i = 0; i < size; i++)
               c += a[i].conj() * b[i];
#endif
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
         for (int i = 0; i < GridSystem::Dim(); i++) {
            cglob *= 2 * M_PI / (GridSystem::Xmax(i) - GridSystem::Xmin(i));
         }

      } else {
         for (int i = 0; i < GridSystem::Dim(); i++) {
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
         GetComm()->Allreduce(&cglob, &c, 2, MPI::DOUBLE, MPI::SUM);
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


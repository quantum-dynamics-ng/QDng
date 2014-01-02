#include "WFGridSystem.h"
#include "tools/Exception.h"
#include "fft/fft.h"
#include "math/math_functions.h"
#include "tools/ZCopyStream.h"

namespace QDLIB {
         
   WFGridSystem::WFGridSystem() :
	 _isKspace(false),  _fft(NULL)
   {}
	    
   WFGridSystem::~WFGridSystem()
   {
      if (_fft != NULL) delete _fft;
   }
   
   void WFGridSystem::CheckFFT()
   {
      /* Make sure the space buffer has the right size */
      GetSpaceBuffer();

      if ( _spacebuffer->size() != size() || _spacebuffer->strides() != strides()){
         _spacebuffer->newsize(size(), strides());
         delete _fft;
         _fft = NULL;
      }

      if (_fft == NULL){
         if (IsKspace())
            /* Take care - the buffers are switched in this constellation! */
            _fft = new FFT(*((GridSystem*) this), *GetSpaceBuffer(), *((cVec*) this));
         else
            _fft = new FFT(*((GridSystem*) this), *((cVec*) this), *GetSpaceBuffer());
      }
   }
   
   /**
    * Return a reference to internal FFT object.
    */
   FFT& WFGridSystem::fft()
   {
      CheckFFT();
      return *_fft;
   }

   void WFGridSystem::Init(ParamContainer &params)
   {
      if (GridSystem::Dim() == 0 || GridSystem::Dim() > MAX_DIMS)
         throw(EParamProblem("Dims not initialized or to large"));
   }

   
   /**
    * Copy own content.
    * 
    * Should be used by derived classes to transfers the information correctly.
    */
   void WFGridSystem::operator =(WFGridSystem *G)
   {
      IsKspace(G->IsKspace());
      *((GridSystem*) this) = *((GridSystem*) G);
      *((cVec*) this) = *((cVec*) G);
   }

   void WFGridSystem::Reaquire()
   {
      WaveFunction::Reaquire();

      /* Tell the fft about new buffers. */
      if (_fft != NULL){
         if (IsKspace())
            _fft->ReplaceBuffers(GetSpaceBuffer(), (cVec*) this);
         else
            _fft->ReplaceBuffers((cVec*) this, GetSpaceBuffer());
      }

   }

   void WFGridSystem::Reduce(double tolerance)
   {
      double norm;
      int size = cVec::size();
 
      CheckFFT();
      
      _fft->forward();
      IsKspace(true);
      
      norm = VecMax(*this) * tolerance; /* This is the cut-off criteria */
      
      int i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
      for (i=0; i <  size; i++){
         /* cut down real & imag seperately */
         if ( abs((*this)[i].real()) < norm )
            (*this)[i]._real = 0;
         
         if ( abs((*this)[i].imag()) < norm )
            (*this)[i]._imag = 0;
      }
      
      *this *= 1./sqrt(double(size)); /* Normalize */
   }

   void WFGridSystem::Restore()
   {
      CheckFFT();
      
      _fft->backward();
      IsKspace(false);
      
      *this *= 1./sqrt(double(cVec::size())); /* Normalize */
   }
   

   void WFGridSystem::Serialize (::google::protobuf::io::ZeroCopyOutputStream& os)
   {
      // Write header
      grid_sys.set_data_size(sizeBytes());

      uint32_t size = grid_sys.ByteSize();
      WriteToZeroCopyStream(os, reinterpret_cast<char*>(&size), sizeof(size));

      if (! grid_sys.SerializeToZeroCopyStream(&os) )
         throw(EIOError("Can't write WF to stream"));

      // Write data
      WriteToZeroCopyStream(os, reinterpret_cast<char*>(begin(0)), sizeBytes());
   }

   void WFGridSystem::DeSerialize (::google::protobuf::io::ZeroCopyInputStream& is)
   {
      // read header
      uint32_t size;

      ReadFromZeroCopyStream(is, reinterpret_cast<char*>(&size), sizeof(uint32_t));

      if (! grid_sys.ParseFromBoundedZeroCopyStream(&is, size) )
         throw(EIOError("Can't read WF from stream"));

      newsize(GridSystem::Size());
      grid_sys.set_data_size(GridSystem::Size() * sizeof(*begin(0)));

      ReadFromZeroCopyStream(is, reinterpret_cast<char*>(begin(0)), sizeBytes());
   }

} /* namespace QDLIB */




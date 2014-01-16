#include "WFGridSystem.h"
#include "tools/Exception.h"
#include "fft/fft.h"
#include "math/math_functions.h"
#include "tools/ZCopyStream.h"

namespace QDLIB {
         
   WFGridSystem::WFGridSystem() :
	 _isKspace(false),  _fft(NULL)
   {}
	    

   WFGridSystem::WFGridSystem(const GridSystem& grid) :
            WaveFunction(grid.Size()), GridSystem(grid), _isKspace(false),  _fft(NULL)
   {
   }

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

   WaveFunction* WFGridSystem::Reduce(double tolerance)
   {
      WaveFunction* res = NewInstance();
      int size = cVec::size();
 
      CheckFFT();
      
      _fft->forward();
      IsKspace(true);
      
      const double thr = VecMax(*this) * tolerance; /* This is the cut-off criteria */
      
      int i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
      for (i=0; i <  size; i++){
         /* cut down real & imag seperately */
         if ( abs((*this)[i].real()) < thr )
            (*res)[i]._real = 0;
         else
            (*res)[i]._real = (*this)[i].real() / double(size);
         
         if ( abs((*this)[i].imag()) < thr )
            (*res)[i]._imag = 0;
         else
            (*res)[i]._imag = (*this)[i].imag()  / double(size);
      }

      IsKspace(false);

      return res;
   }

   void WFGridSystem::Restore(WaveFunction* Psi)
   {
      WFGridSystem* psi = dynamic_cast<WFGridSystem*>(Psi);

      // Copy
      IsKspace(true);
      *((GridSystem*) this) = *((GridSystem*) psi);
      *((cVec*) this) = *((cVec*) psi);

      CheckFFT();
      _fft->backward();
      IsKspace(false);
      
   }
   

   void WFGridSystem::Serialize (std::ostream& os)
   {
      // Write header
      grid_sys.set_data_size(sizeBytes());

      uint32_t size = grid_sys.ByteSize();
      os.write(reinterpret_cast<char*>(&size), sizeof(size));

      if (! grid_sys.SerializeToOstream(&os) )
         throw(EIOError("Can't write WF to stream"));

      // Write data
      os.write(reinterpret_cast<char*>(begin(0)), sizeBytes());

      // dump gridsystem parameters, so the get written to the meta data
      grid_sys.DumpParams(_params);
   }

   void WFGridSystem::DeSerialize (std::istream& is)
   {
      // read header
      uint32_t size;

      is.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));

      char* buf = new char[size];
      is.read(buf, size);
      if (! grid_sys.ParseFromArray(buf, size) )
         throw(EIOError("Can't read WF from stream"));

      delete[] buf;

      // Read data
      newsize(GridSystem::Size());
      grid_sys.set_data_size(GridSystem::Size() * sizeof(*begin(0)));

      is.read(reinterpret_cast<char*>(begin(0)), sizeBytes());
   }

} /* namespace QDLIB */




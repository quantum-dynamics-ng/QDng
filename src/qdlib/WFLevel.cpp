#include "WFLevel.h"
#include "tools/Exception.h"
#include "tools/ZCopyStream.h"

#include <stdint.h>

namespace QDLIB {
   
   QDNG_WAVEFUNCTION_NEW_INSTANCE_FUNCTION(WFLevel)
   
   WFLevel::WFLevel(): _name("WFLevel")   {}
   
   WFLevel::~WFLevel() {}
   
   /**
    * \see constructor.
    * 
    */
   void WFLevel::Init(ParamContainer &params)
   {
      int size;
      
      _params = params;
      
      _params.GetValue("size", size);
      
      /* Zero vector doesn't make sense */
      if (size == 0) {
	 ParamContainer missing;
	 missing.SetValue("size", 0);
	 throw( EParamProblem(missing, "Missing size") );
      }
      
      cVec::newsize(size);
   }
   
   const string& WFLevel::Name()
   {
      return _name;
   }
   
   WaveFunction* WFLevel::operator=(WaveFunction *Psi)
   {
      WFLevel *psi = dynamic_cast<WFLevel*>(Psi);
      
      if (!psi) throw ( EIncompatible("WFLevel can't copy from", Psi->Name()) );
      
      (*(cVec*) this) = (cVec) *Psi;
      _params =  Psi->Params();
      
      return this;
   }
   
   WFLevel& WFLevel::operator=(WFLevel &Psi)
   {
      *(cVec*) this = (cVec) Psi;
      _params =  Psi.Params();
      
      return *this;
   }
   
   /** Scalar product. */
   dcomplex WFLevel::operator*(WaveFunction *Psi)
   {
      
      if (size() != Psi->size()) throw(EIncompatible("WFs differ in size"));
      
      /** \todo make mpi-save */
      int s;
      dcomplex *a, *b;
      dcomplex c(0,0);
      for (s=0; s < strides(); s++){
         a = begin(s);
         b = Psi->begin(s);
         for(int i=0; i < lsize(); i++){
            c += a[i].conj() * b[i];
         }
      }
      
      return c;
   }
   
   /**
    * Set new size of the vector.
    */
   void WFLevel::Size(int size)
   {
      _params.SetValue("size",  size);
      cVec::newsize(size);
   }
   
   void WFLevel::Reduce(double tolerance)
   {
      cVec *out = GetSpaceBuffer(); /* Result is written in spacebuffer */
      double norm = Norm();
      int size = cVec::size();
      
      norm = norm / double(size) * tolerance; /* This is the cut-off criteria */
      
      for (int i=0; i <  size; i++){
         /* treat real & imag seperately */
         if ( (*this)[i].real() < norm )
            (*out)[i]._real = 0;
         else
            (*out)[i]._real = (*this)[i].real();
         
         if ( (*this)[i].imag() < norm )
            (*out)[i]._imag = 0;
         else
            (*out)[i]._imag = (*this)[i].imag();
      }
      
      IsKspace(true);
   }

   void WFLevel::Restore()
   {
      /* Nothing to do here. Just switch the buffer to the right position */
      cVec::swap(*_spacebuffer);
      _IsKspace = false;
   }
   

   void WFLevel::Serialize (::google::protobuf::io::ZeroCopyOutputStream& os)
   {
      // Simple data format:
      uint64_t sizee=cVec::size();
      uint64_t sizeb=sizeBytes();

      WriteToZeroCopyStream(os, reinterpret_cast<char*>(&sizee), sizeof(sizee));
      WriteToZeroCopyStream(os, reinterpret_cast<char*>(&sizeb), sizeof(sizeb));
      WriteToZeroCopyStream(os, reinterpret_cast<char*>(begin(0)), sizeb);
   }

   void WFLevel::DeSerialize (::google::protobuf::io::ZeroCopyInputStream& is)
   {
      uint64_t sizee, sizeb;

      ReadFromZeroCopyStream(is, reinterpret_cast<char*>(&sizee), sizeof(sizee));
      ReadFromZeroCopyStream(is, reinterpret_cast<char*>(&sizeb), sizeof(sizeb));

      if (sizee * sizeof(dcomplex) != sizeb)
         throw( EIOError("Can't read WFLevel: wrong header") );

      newsize(sizee);

      ReadFromZeroCopyStream(is, reinterpret_cast<char*>(begin(0)), sizeb);
   }
} /* namespace QDLIB */


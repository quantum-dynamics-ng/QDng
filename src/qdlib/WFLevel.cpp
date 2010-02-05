#include "WFLevel.h"
#include "tools/Exception.h"


namespace QDLIB {
   
   WFLevel::WFLevel(): _name("WFLevel")   {}
   
   WFLevel::~WFLevel() {}
   
   
   WaveFunction* WFLevel::NewInstance()
   {
      WFLevel *r =  new WFLevel();
      r->Init(_params);
      return r;
   }
   
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
      
   /**
    * Calcualte the Norm of the vector.
    */
   double WFLevel::Norm()
   {
      dcomplex d;
      
      d = *this * *this;
      return sqrt(d.real());
   }
   
   /**
    * Normalize the vector.
    */
   void WFLevel::Normalize()
   {
      double  d = 1/Norm();
      for (int i=0; i < size(); i++){
	 (*this)[i] = (*this)[i] * d;
      }
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
   
   
} /* namespace QDLIB */


#include "WFLevel.h"
#include "tools/Exception.h"

using namespace TNT;
namespace QDLIB {
   
   WFLevel::WFLevel(): _name("WFLevel") {}
   
   WFLevel::~WFLevel() {}
   
   /**
    * Init with params.
    * 
    * Parameters needed:
    * -size
    * 
    * \bug if size is missing, what should happen?
    */
   WFLevel::WFLevel(ParamContainer &params): WaveFunction(params), _name("WFLevel")
   {
      int size;
      
      _params.GetValue(string("size"), size);
      cVec::newsize(size);
   }
   
   /**
    * Init with size of vector.
    */
   WFLevel::WFLevel(int size) : _name("WFLevel")
   {
      _params.SetValue("size",  size);
      cVec::newsize(size);
   }
   
   
   WaveFunction* WFLevel::NewInstance()
   {
      WFLevel *r =  new WFLevel(_params);
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
   
   int WFLevel::ID()
   {
      return 0;
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
      cout << "factor: " << d << endl;
      for (int i=0; i < size(); i++){
	 (*this)[i] = (*this)[i] * d;
      }
   }
   
   WaveFunction* WFLevel::operator=(WaveFunction *Psi)
   {
      *(cVec*) this = (cVec) *Psi;
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
      return (cVec) *Psi * (cVec) *Psi;
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


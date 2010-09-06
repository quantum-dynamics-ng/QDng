#include "OSum.h"


namespace QDLIB {
   

   OSum::~OSum()
   {
      for(int i=0; i < Size(); i++){
	 DELETE_WF(_WFbuf[i]);
      }
   }
   
   /**
    * Default constructor
    */
   OSum::OSum() : _name("OSum")
   {
      for(int i=0; i < MAX_OPS; i++){
	 _WFbuf[i] = NULL;
      }
   }
   
   
   /* Interface implementation, Operator */
   Operator* OSum::NewInstance()
   {
      Operator *r;
      r = new OSum();
      return r;
   }
   
   const string& OSum::Name()
   {
      return _name;
   }
		  
   dcomplex OSum::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      WaveFunction  *out;
      dcomplex d;
     
      out = PsiKet->NewInstance();
      Apply(out, PsiKet);
      d = *PsiBra * out;
      DELETE_WF(out);
      
      return d;
   }
   
   dcomplex OSum::Emax()
   {
      if (Size() == 0)
	 throw ( EParamProblem("Sum Operator is empty") );
      
      dcomplex d(0);
      for (int i=0; i < Size(); i++){
	 d += Get(i)->Emax();
      }
      
      return d;
   }
	 
   dcomplex OSum::Emin()
   {
      if (Size() == 0)
	 throw ( EParamProblem("Sum Operator is empty") );
      
      dcomplex d(0);
      for (int i=0; i < Size(); i++)
	 d += Get(i)->Emin();
      
      return d;
   }
   
   void OSum::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      if (_WFbuf[0] == NULL) _WFbuf[0] = sourcePsi->NewInstance();

      Get(0)->Apply(destPsi, sourcePsi);
      
      for (int i=1; i < Size(); i++)
      {
	 Get(i)->Apply(_WFbuf[0], sourcePsi);
	 AddElements(destPsi, _WFbuf[0]);
      }
   }
   
   void OSum::Apply(WaveFunction *Psi)
   {
      
      if (_WFbuf[0] == NULL) _WFbuf[0] = Psi->NewInstance();
      if (_WFbuf[1] == NULL) _WFbuf[1] = Psi->NewInstance();
      
      _WFbuf[0]->FastCopy(*Psi);
      Get(0)->Apply(Psi);
            
      for (int i=1; i < Size(); i++)
      {
	 Get(i)->Apply(_WFbuf[1], _WFbuf[0]);
	 AddElements(Psi, _WFbuf[1]);
      }
   }
   
   Operator * QDLIB::OSum::Copy(Operator * O)
   {
      OSum *r;
      
      r = dynamic_cast<OSum*> (O);
      if (r == NULL)
	 throw( EIncompatible ("Incompatible in Assignment", this->Name(), O->Name() ) );
      
      OList::Copy(O);
      
      return r;
   }
   
   
}
 /* namespace QDLIB */





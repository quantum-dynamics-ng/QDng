#include "OSum.h"


namespace QDLIB {
   

   OSum::~OSum()
   {
      for(int i=0; i < _size; i++){
	 delete _O[i];
	 delete _WFbuf[i];
      }
   }
   
   /**
    * Default constructor
    */
   OSum::OSum() : _name("OSum"), _size(0)
   {
      for(int i=0; i < MAX_OPS; i++){
	 _O[i] = NULL;
	 _WFbuf[i] = NULL;
      }
   }
   
   /**
    * Access elements.
    * 
    * Note this is read only. For assingning use Add()
    */
   Operator* OSum::operator[](int i)
   {
      if (i >= _size)
	 throw( EIncompatible("Access to empty element") );
      return _O[i];
   }
   
   /**
    * Number of operators.
    */
   int OSum::Size()
   {
      return _size;
   }
   
   /**
    * Add a new operator.
    */
   void OSum::Add(Operator *O)
   {
      if (_size >= MAX_OPS) {
	 throw ( EOverflow("Maximum numbers of operators reached") );
      }
      if (O == NULL){
	 throw ( EIncompatible("Operator not initialized") );
      }
      
      if (O->isTimeDep()) _isTimedependent = true;
      O->Clock(clock);
      
      _O[_size] = O;
      _size++;
      
      
   }
   
   /* Interface implementation, Operator */
   Operator* OSum::NewInstance()
   {
      Operator *r;
      r = new OSum();
      return r;
   }
	   
   void OSum::UpdateTime()
   {
      for(int i=0; i < _size; i++){
	 _O[i]->UpdateTime();
      }     
   }
   
   
   /**
    * We take can't take parameters.
    */
   void OSum::Init(ParamContainer &params)
   {
   }
  
   void OSum::Init( WaveFunction *Psi)
   {
      for(int i=0; i < _size; i++){
	 _O[i]->Init(Psi);
      }
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
      delete out;
      
      return d;
   }
   
   double OSum::Expec(WaveFunction *Psi)
   {
      dcomplex d;
      
      d = MatrixElement(Psi,Psi);
      
      return d.real();
   }
   
   double OSum::Emax()
   {
      if (_size == 0)
	 throw ( EParamProblem("Sum Operator is empty") );
      
      double d = 0;
      for (int i=0; i < _size; i++)
	 d += _O[i]->Emax();
      
      return d;
   }
	 
   double OSum::Emin()
   {
      if (_size == 0)
	 throw ( EParamProblem("Sum Operator is empty") );
      
      double d = 0;
      for (int i=0; i < _size; i++)
	 d += _O[i]->Emin();
      
      return d;
   }
   
   WaveFunction* OSum::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      if (_WFbuf[0] == NULL) _WFbuf[0] = sourcePsi->NewInstance();
      
      _O[0]->Apply(destPsi, sourcePsi);
      
      dcomplex *d = destPsi->begin(0), *b = _WFbuf[0]->begin(0);
      int size = destPsi->lsize();
      int strides = destPsi->strides();
      for (int i=1; i < _size; i++)
      {
	 _O[i]->Apply(_WFbuf[0], sourcePsi);	 
	 for (int s=0; s < strides; s++){
	    #pragma omp parallel for
	    for (int j=0; j < size; j++){
	       d[j] += b[j];
	    }
	 }
      }
   
      return destPsi;
   }
   
   WaveFunction* OSum::Apply(WaveFunction *Psi)
   {
      
      if (_WFbuf[0] == NULL) _WFbuf[0] = Psi->NewInstance();
      if (_WFbuf[1] == NULL) _WFbuf[1] = Psi->NewInstance();
      
      _WFbuf[0]->FastCopy(*Psi);
      
      _O[0]->Apply(Psi);
      
      dcomplex *d = Psi->begin(0), *b = _WFbuf[1]->begin(0);
      int size = Psi->lsize();
      int strides = Psi->strides();
      
      for (int i=1; i < _size; i++)
      {
	 _O[i]->Apply(_WFbuf[1], _WFbuf[0]);
	 for (int s=0; s < strides; s++){
	    #pragma omp parallel for
	    for (int j=0; j < size; j++){
	       d[j] += b[j];
	    }
	 }
      }
      
      return Psi;
   }
      
   
   Operator* OSum::operator=(Operator* O)
   {
      OSum *r;
      
      r = dynamic_cast<OSum*> (O);
      if (r == NULL)
	 throw( EIncompatible ("Incompatible in Assginment", this->Name(), O->Name() ) );
      
      _size = r->_size;
      for (int i=0; i < _size; i++){
	 _O[i] = r->_O[i]->NewInstance();
	 *(_O[i]) = r->_O[i];
      }
      return r;
   }
   
   Operator* OSum::operator*(Operator* O)
   {
      throw( EIncompatible("Not Implementet")  );
   }
	    
   
   Operator* QDLIB::OSum::Offset(const double d)
   {
      for (int i=0; i < _size; i++)
	 _O[i]->Offset(d);
      return this;
   }

   Operator* QDLIB::OSum::Scale(const double d)
   {
      for (int i=0; i < _size; i++){
	 _O[i]->Scale(d);
      }
      
      return this;
   }

   
} /* namespace QDLIB */



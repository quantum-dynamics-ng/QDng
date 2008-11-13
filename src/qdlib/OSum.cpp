#include "OSum.h"


namespace QDLIB {
   

   OSum::~OSum()
   {
   }
   
   /**
    * Default constructor
    */
   OSum::OSum() : _name("OSum"), _size(0)
   {
      for(int i=0; i < MAX_OPS; i++){
	 _O[i] = NULL;
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
  
	   
   const string& OSum::Name()
   {
      return _name;
   }
		  
   dcomplex OSum::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      WaveFunction  *out;
         
      out = PsiKet->NewInstance();
      *out =  (*this) * PsiKet;
      
      return *PsiBra * out;
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
   
   
   WaveFunction* OSum::operator*(WaveFunction *Psi)
   {
      WaveFunction *sum;
      
      
      sum = Psi->NewInstance();
      *((cVec*) sum) = dcomplex(0,0);
      
      for (int i=0; i < _size; i++)
      {
	*sum += *(_O[i]) * Psi;
      }
     
      return sum;
   }
   
   WaveFunction* OSum::operator*=(WaveFunction *Psi)
   {
      WaveFunction *ket;
      
      ket = Psi->NewInstance();
     
      *ket = Psi;    // Copy
      *((cVec*) Psi) = dcomplex(0,0); // Init with zeroes
      
      for (int i=0; i < _size; i++)
      {
	*Psi +=  *(_O[i]) * ket;
      }
      return Psi;
   }
   
   
   Operator* OSum::operator=(Operator* O)
   {
      OSum *r;
      
      r = dynamic_cast<OSum*> (O);
      
      r->_size = _size;
      for (int i=0; i < _size; i++){
	 r->_O[i] = _O[i]->NewInstance();
	 *(r->_O[i]) = _O[i];
      }
      return r;
   }
   
   Operator* OSum::operator*(Operator* O)
   {
      throw( EIncompatible("Not Implementet")  );
   }
	    
   
   
} /* namespace QDLIB */


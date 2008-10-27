#include "OSum.h"


namespace QDLIB {
   
   /**
    * All operators will be destructed.
    */
   OSum::~OSum()
   {
      for(int i=0; i < _size; i++){
	 delete _O[i];
      }
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
      
      _O[_size - 1] = O;
      _size++;
      
      
   }
   
   /* Interface implementation, Operator */
   Operator* OSum::NewInstance()
   {
      Operator *r;
      r = new OSum();
      return r;
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
      WaveFunction *ket;
      
      *ket = *PsiKet;
      
      for (int i; i < _size; i++)
      {
	*(_O[i]) *= ket; 
      }
      return *PsiBra * ket;
   }
   
   double OSum::Expec(WaveFunction *Psi)
   {
      dcomplex d;
      
      d = MatrixElement(Psi,Psi);
      
      return d.real();
   }
   
   WaveFunction* OSum::operator*(WaveFunction *Psi)
   {
      WaveFunction *ket, *sum;
      
      *ket = Psi;
      
      
      for (int i=0; i < _size; i++)
      {
	*sum += *(_O[i]) * ket; 
      }
      return sum;
   }
   
   WaveFunction* OSum::operator*=(WaveFunction *Psi)
   {
      
      for (int i=0; i < _size; i++)
      {
	*Psi += *(_O[i]) * Psi; 
      }
      return Psi;
   }
   
   
   Operator* OSum::operator=(Operator* O)
   {
      OSum *r;
      
      *r = dynamic_cast<OSum*> (O);
      
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


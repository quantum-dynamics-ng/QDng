#include "OGrid1Dd2dx.h"
#include "sys/Exception.h"
#include "WFGrid1D.h"



namespace QDLIB {
   
   
   double OGrid1Dd2dx::Mass()
   {
      return _mass;
   }
   
   void OGrid1Dd2dx::Mass(double mass)
   {
      _mass = mass;
   }
   
   /* Interface Implementation */
   
   Operator* OGrid1Dd2dx::NewInstance()
   {
      OGrid1Dd2dx *r = new OGrid1Dd2dx();
      r->Init(_params);
      return r;
   }
	 	 
   void OGrid1Dd2dx::Init(ParamContainer &params)
   {
      _params = params;
      
      _params.GetValue("mass", _mass);
      
      if (_mass <= 0) throw ( EParamProblem("Zero mass defined") );
      
   }
	 
   const string& OGrid1Dd2dx::Name()
   {
      return _name;
   }
 
   /**
    * Init the k-Space with \f$ -k^2 \f$.
    * 
    * 
    * \todo Optimize k-space init loop, its a bit ugly
    */
   void OGrid1Dd2dx::InitKspace()
   {
      if ( (_L_last) == 0 || _Nx_last == 0)
         throw ( EParamProblem("Can't init k-space: No parameters defined") );
      

      
      if (_kspace == NULL){
         _kspace = new dVec(_Nx_last);
      }
      else if ( _kspace->size() != _Nx_last){
         _kspace->newsize(_Nx_last);
      }
         
      double dp = (2*PI) / _L_last;    // Setup dp for kspace
     
      /* We include all the factors in the k-space function => Do it only once */
      /* The minus cancels with minus from -kx^2. */
      /* 1/Nx due to unormalized FFT  */
      double m = 0.5 / ( _mass  );
     
      
      int i,j;
	 
      for (i=(-(_Nx_last) / 2), j=0; j < _Nx_last / 2; i++, j++){ //run from [-p..+p]
	 (*_kspace)[j] = (double(j) * double(j)) * dp * dp * m;
	(*_kspace)[_Nx_last - j - 1] = (double(j+1)* double(j+1)) * dp * dp * m;
      }
    
   }
   
   dcomplex OGrid1Dd2dx::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      if (PsiBra->size() != PsiKet->size()) throw ( EParamProblem("Bra and Ket vector doesn't have same size") );
      
      WaveFunction *opKet;
      opKet =  (*this) * PsiKet;
      dcomplex d = (*PsiBra) * opKet;
      
      delete opKet;
      
      return d;
   }
   
   
   
   double OGrid1Dd2dx::Expec(WaveFunction *Psi)
   {
      dcomplex d = MatrixElement(Psi, Psi);
      return d.real();
   }
	 
   WaveFunction* OGrid1Dd2dx::operator*(WaveFunction *Psi)
   {
      
      WFGrid1D *opPsi, *ket;
      
      ket = dynamic_cast<WFGrid1D*> (Psi);
      
      if ( (ket->xmax() - ket->xmin()) != _L_last || ket->size() != _Nx_last )
      {
         _L_last = ket->xmax() - ket->xmin();
         _Nx_last = ket->size();
         InitKspace();
      }
      
      opPsi = new WFGrid1D();    // New instance
      *opPsi = *ket; // Copy
            
      opPsi->ToKspace();
    
      for (int i=0; i < _kspace->size(); i++)
      {
	 (*opPsi)[i] *= (*_kspace)[i] /double(_Nx_last);
      }
      
      opPsi->ToXspace();
    
      return opPsi;
   }
   
   
   WaveFunction* OGrid1Dd2dx::operator*=(WaveFunction *Psi)
   {
      WFGrid1D  *ket;
      
      ket = dynamic_cast<WFGrid1D*> (Psi);
      
      if ( (ket->xmax() - ket->xmin()) != _L_last || ket->size() != _Nx_last )
      {
         _L_last = ket->xmax() - ket->xmin();
         _Nx_last = ket->size();
         InitKspace();
      }
     
      ket->ToKspace();
      
      for (int i=0; i < _kspace->size(); i++)
      {
	 (*ket)[i] *= (*_kspace)[i]/double(_Nx_last);
      }
       
      ket->ToXspace();
      
      return ket;
   }
   
   
   Operator* OGrid1Dd2dx::operator=(Operator* O)
   {
      _params = O->Params();
      
      OGrid1Dd2dx* op;
      
      op = dynamic_cast<OGrid1Dd2dx*> (O);
      
      _L_last = op->_L_last;
      _Nx_last = op->_Nx_last;
      
      *_kspace = *(op->_kspace);
     
      _mass = op->_mass;
      
      return this;
   }
	 
   Operator* OGrid1Dd2dx::operator*(Operator* O)
   {
      throw ( EIncompatible("Sorry can't apply to another operator") );
   }
   
} /* namespace QDLIB */

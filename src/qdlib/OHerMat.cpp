#include "OHerMat.h"
#include "tools/Exception.h"



namespace QDLIB {
   
   OHerMat::~OHerMat()
   {
      if (_X == NULL) delete _X;
      if (_E == NULL) delete _X;
   }
   
   OHerMat::OHerMat() : _name("OHermitianRealMatrix"), _X(NULL), _E(NULL) {}
   
   /**
    * Initialieses a size x size matrix.
    */
   OHerMat::OHerMat(int size) : _name("OHermitianRealMatrix"), _X(NULL), _E(NULL) 
   {
      _params.SetValue("size", size);
      dMat::newsize(size,size);
   }
   
   OHerMat::OHerMat(ParamContainer &params) :Operator(params),  _name("OHermitianRealMatrix")
   {
      int size;
      
      _params.GetValue("size", size);
      
      dMat::newsize(size,size);
   }
   
   /**
    * Return a fresh instance.
    */
   Operator* OHerMat::NewInstance()
   {
      OHerMat *r =  new OHerMat();
      r->newsize(num_rows(), num_rows());
      return r;
   }
   
   void OHerMat::UpdateTime() {}
   
   /**
    * Initializer.
    */
   void OHerMat::Init(ParamContainer &params)
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
      
      dMat::newsize(size,size);
   }
   
   /**
    * Set size of Matrix.
    */
   void OHerMat::Size(int size)
   {  
      int s;
      
      _params.GetValue("size", s);
      
      if (s != size) {
         _params.SetValue("size", size);
	 dMat::newsize(size,size);
      }
      
   }
   
   int OHerMat::Size()
   {
      return num_rows();
   }
   
   /**
    * Tell your name.
    */
   const string& OHerMat::Name()
   {
      return _name;
   }
   
   /**
    * Calulate a "matrix" element.
    * 
    * This looks strange because we are a matrix at the moment.
    * But never the less \f$ \langle \psi_1 \vert \hat O \vert \psi_2 \rangle\f$ is calculated
    * and thus it is still straight forward.
    */
   dcomplex OHerMat::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      return *PsiBra * *((*this) * (PsiKet));
   }
   
   /**
    * Calulate an expectation value.
    */
   double OHerMat::Expec(WaveFunction *Psi)
   {
      return MatrixElement(Psi, Psi).real();
   }
   
   double OHerMat::Emax()
   {      
      double d = (*this)[0][0];
      
      for (int i=1; i < num_rows() ; i++)
	 if ((*this)[0][0] > d) d = (*this)[0][0];
	 
      return d;
   }
	 
   double OHerMat::Emin()
   {      
      double d = (*this)[0][0];
      
      for (int i=1; i < num_rows() ; i++)
	 if ((*this)[0][0] < d) d = (*this)[0][0];
      
      return d;
   }
   
   /**
    * Matrix-Vector multiplication.
    * 
    * Works with every type of WaveFunction (relies only on cVec).
    */
   WaveFunction* OHerMat::operator*(WaveFunction *Psi)
   {
      WaveFunction *result;
      
      result = Psi->NewInstance();
      MatrixVectorMult(result, *this, *Psi);
      return result;
   }
   
   /**
    * Matrix-Vector multiplication (in place).
    * 
    * Works with every type of WaveFunction (relies only on cVec).
    * 
    * \todo Optimize for real inplace.
    */   
   WaveFunction* OHerMat::operator*=(WaveFunction *Psi)
   {
      WaveFunction *result;
      
      result = Psi->NewInstance();
      MatrixVectorMult(result, *this, *Psi);
      delete Psi;
      return result;
   }
   
   /**
    * Copy operator.
    */
   Operator* OHerMat::operator=(Operator *O)
   {
      *((dMat*) this) = *((dMat*) O);
      _params =  O->Params();
      
      return this;
   }
   
   /**
    * Copy operator.
    */
   OHerMat& OHerMat::operator=(OHerMat &O)
   {
      *(dMat*) this = (dMat) O;
      _params =  O.Params();
      
      return *this;
   }
   
   /**
    * Diagonlize the matrix.
    * 
    * The eigenvalues & eigenvectors will be stored in the class.
    */
   void OHerMat::Diag()
   {
      throw ( Exception("Diag not implemented yet.") );
   }
   
   /**
    * Mulitiplies a matrix (dMat) with another matrix (of type dMat).
    * 
    * Matrix*Matrix has to be defined for the types used.
    */
   Operator* OHerMat::operator*(Operator *O)
   {
      Operator *result;
      
      result = this->NewInstance();
      
      MatrixMatrixMult( (dynamic_cast<dMat*>(result) ), this, ( dynamic_cast<dMat*>(O) ) );
      
      return result;
   }
   
   Operator * QDLIB::OHerMat::Offset(const double d)
   {
      for (int i=0; i < num_rows() ; i++)
	 (*this)[i][i] += d;
      return this;
   }


   Operator * QDLIB::OHerMat::Scale(const double d)
   {
      for (int i=0; i < num_rows() ; i++)
	 for (int j=0; j < num_cols() ; j++)
	  (*this)[i][j] *= d;
      return this;
   }


} /* namespace QDLIB */



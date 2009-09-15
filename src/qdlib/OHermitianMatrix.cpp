#include "OHermitianMatrix.h"
#include "tools/Exception.h"
#include "linalg/LapackDiag.h"

namespace QDLIB {
   
   OHermitianMatrix::~OHermitianMatrix()
   {
      if (_X == NULL) delete _X;
   }
   
   
   OHermitianMatrix::OHermitianMatrix() : dMat(), ODSpaceT<double>(), _X(NULL),  _valid(false), _init(false), _name("OHermitianMatrix") {}
   
   
   /**
    * Return a fresh instance.
    */
    Operator* OHermitianMatrix::NewInstance()
   {
      OHermitianMatrix *r =  new OHermitianMatrix();
      r->newsize(rows(), rows());
      return r;
   }
   
   /**
    * Initializer.
    *
    */
   void OHermitianMatrix::Init(ParamContainer &params)
   {
      int size;
      string name;
      
      Operator::_params = params;
      
      _params.GetValue("file", name);
      _params.GetValue("size", size);
      
      /* Init from file - Set recursion indicator _init */
      if (size <= 0 && name.size() != 0 && !_init){
         _init = true;
         File()->Suffix(BINARY_O_SUFFIX);
         File()->Name(name);
	 
         *File() >> (OHermitianMatrix*) this; /* Read Matrix */

         /* Adjust min. diag value to zero */
         bool set_zero;
         _params.GetValue("setzero", set_zero);
         if (set_zero){
            double min = Emin();
            for (lint i=0; i < Size(); i++){
               (*this)(i,i) -= min;
            }
         }

         _init = false;
         return;
      }
      
      /* Init with params */
      
      /* Zero size matrix doesn't make sense */
      if (size == 0) {
	 ParamContainer missing;
	 missing.SetValue("size", 0);
	 throw( EParamProblem(missing, "Missing size") );
      }
      
      dMat::newsize(size,size);
      
      /* Check if we diagonalize imidiately */
      bool initDiag;
      _params.GetValue("qdiag", initDiag, true);
      if (initDiag){
         if (_X == NULL) _X = new dMat(*this);
         LAPACK::FullDiagHermitian(_X, _dspace);
      }
      
      
   }
   
   /**
    * Set size of Matrix.
    */
   void OHermitianMatrix::Size(int size)
   {  
      int s;
      
      _params.GetValue("size", s);
      
      if (s != size) {
         _params.SetValue("size", size);
         dMat::newsize(size,size);
      }
      
   }
   
   /**
    * \return size of matrix
    */
   int OHermitianMatrix::Size()
   {
      return rows();
   }
   
   /**
    * Tell your name.
    */
   const string& OHermitianMatrix::Name()
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
   dcomplex OHermitianMatrix::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      WaveFunction *ket = PsiKet->NewInstance();
      
      Apply(ket, PsiKet);
      return *PsiBra * ket;
   }
   
   /**
    * Calulate an expectation value.
    */
   double OHermitianMatrix::Expec(WaveFunction *Psi)
   {
      return MatrixElement(Psi, Psi).real();
   }
   
   double OHermitianMatrix::Emax()
   {      
      double d = (*this)(0,0);
      
      for (int i=1; i < rows() ; i++)
         if ((*this)(i,i) > d) d = (*this)(i,i);
	 
      return d;
   }
	 
   double OHermitianMatrix::Emin()
   {      
      double d = (*this)(0,0);
      
      for (int i=1; i < rows() ; i++)
         if ((*this)(0,0) < d) d = (*this)(0,0);
      
      return d;
   }
   
   /**
    * Matrix-Vector multiplication.
    * 
    * Works with every type of WaveFunction (relies only on cVec).
    */
   WaveFunction* OHermitianMatrix::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      MatVecMult((cVec*) destPsi, (dMat*) this, (cVec*) sourcePsi);
      return destPsi;
   }
   
   /**
    * Matrix-Vector multiplication (in place).
    * 
    * Works with every type of WaveFunction (relies only on cVec).
    * 
    * \todo Optimize for real inplace.
    */   
   WaveFunction* OHermitianMatrix::Apply(WaveFunction *Psi)
   {
      WaveFunction *result;
      
      result = Psi->NewInstance();
      MatVecMult((cVec*) result, (dMat*) this, (cVec*) Psi);
      *Psi = result;
      delete result;
      return Psi;
   }
   
   /**
    * Copy operator.
    */
   Operator* OHermitianMatrix::operator=(Operator *O)
   {
      *((dMat*) this) = *((dMat*) O);
      _params =  O->Params();
      
      return this;
   }
   
   /**
    * Copy operator.
    */
   OHermitianMatrix& OHermitianMatrix::operator=(OHermitianMatrix &O)
   {
      *(dMat*) this = (dMat) O;
      _params =  O.Params();
      
      _valid = false;
      return *this;
   }
   
   /**
    * Diagonlize the matrix.
    * 
    * The eigenvalues & eigenvectors will be stored in the class.
    */
   void OHermitianMatrix::Diag()
   {
      if (_X == NULL || _dspace == NULL)
      {
	 _X;
      }
      FullDiagHermitian();
      _valid = true;
   }
   
   /**
    * Mulitiplies a matrix (dMat) with another matrix (of type dMat).
    * 
    * Matrix*Matrix has to be defined for the types used.
    */
   Operator* OHermitianMatrix::operator*(Operator *O)
   {
      Operator *result;
      
      result = this->NewInstance();
      
      MatrixMatrixMult( (dynamic_cast<dMat*>(result) ), this, ( dynamic_cast<dMat*>(O) ) );
      
      return result;
   }
   
   Operator * OHermitianMatrix::Offset(const double d)
   {
      for (int i=0; i < num_rows() ; i++)
	 (*this)[i][i] += d;
      return this;
   }


   Operator * OHermitianMatrix::Scale(const double d)
   {
      for (int i=0; i < num_rows() ; i++)
	 for (int j=0; j < num_cols() ; j++)
	  (*this)[i][j] *= d;
      
      scaling=d;
      return this;
   }

   void OHermitianMatrix::InitDspace()
   {
      if (_dspace == NULL) {
         _space = new dVec(rows)
      }
   }

} /* namespace QDLIB */



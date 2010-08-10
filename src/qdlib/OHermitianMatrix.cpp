#include "OHermitianMatrix.h"
#include "WFLevel.h"
#include "tools/Exception.h"
#include "linalg/LapackDiag.h"

namespace QDLIB {

   OHermitianMatrix::~OHermitianMatrix()
   {
      if (_X != NULL) delete _X;
      if (_file != NULL) delete _file;
      if (_dspace != NULL) delete _dspace;
   }


   OHermitianMatrix::OHermitianMatrix() : dMat(), ODSpace(), _X(NULL),  _valid(false),
                                      _file(NULL) ,_init(false), _name("OHermitianMatrix") {}


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

      Operator::_params = params;

      _params.GetValue("size", size);
      /* Init from file - Set recursion indicator _init */
      if (_params.isPresent("file") && !_init){

         string name;
         _params.GetValue("file", name);
         _init = true;

         File()->Suffix(BINARY_O_SUFFIX);
         File()->Name(name);

         *File() >> (OHermitianMatrix*) this; /* Read Matrix */

         /* Adjust min. diag value to zero */
         bool set_zero;
         _params.GetValue("setzero", set_zero);
         if (set_zero){
            double min = Emin().real();
            for (lint i=0; i < Size(); i++){
               (*this)(i,i) -= min;
            }
         }
         _init = false;
         _valid = false;
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
      _valid = false;

      /* Check if we diagonalize imidiately */
      bool initDiag;
      _params.GetValue("qdiag", initDiag, false);
      if (initDiag){
         if (_X == NULL) _X = new dMat(rows(),rows());
         InitDspace();
      }


   }

   void OHermitianMatrix::Init(WaveFunction * Psi)
   {
      WFLevel *psi=dynamic_cast<WFLevel*>(Psi);

      if (!psi)
         throw ( EIncompatible("Matrix operator not compatible with wavefunction ",Name(), Psi->Name()) );

      if (psi->size() != rows())
         throw ( EIncompatible("Wavefunction has wrong size") );
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
         _valid = false;
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
      dcomplex c;
      WaveFunction *ket = PsiKet->NewInstance();

      *ket = PsiKet;
      Apply(ket, PsiKet);
      c = *PsiBra * ket;
      
      delete ket;
      return c;
   }

   /**
    * Calulate an expectation value.
    */
   double OHermitianMatrix::Expec(WaveFunction *Psi)
   {
      return MatrixElement(Psi, Psi).real();
   }

   /** Return the largest eigenvalue */
   dcomplex OHermitianMatrix::Emax()
   {
      if (!_valid) InitDspace();
      /* Overestimate Emax to avoid conflicts due to incorrect diagonalization in sums */
      return dcomplex(3 * (*_dspace)[_dspace->size()-1]);
   }

   /** Return the smallest diagonal */
   dcomplex OHermitianMatrix::Emin()
   {
      if (!_valid) InitDspace();
      return dcomplex((*_dspace)[0]);
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
   Operator* OHermitianMatrix::Copy(Operator *O)
   {
      OHermitianMatrix* op = dynamic_cast<OHermitianMatrix*>(O);

      if (op==NULL)
         throw ( EIncompatible ("Copy, not of type OHermitianMatrix", O->Name() ) );

      *((dMat*) this) = *((dMat*) op);

      if (_X != NULL)
         *_X = *(op->_X);

      if (_dspace != NULL)
         *_dspace = *(op->_dspace);

      op->_XT.SetMatrix(op->_X);
      _params =  op->Params();

      _valid = op->_valid;
      return this;
   }

   /**
    * Copy operator.
    */
   Operator* OHermitianMatrix::operator=(Operator *O)
   {
     Copy(O);
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
      InitDspace();
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
      for (int i=0; i < rows() ; i++){
	 (*this)(i,i) += d;
         if(_valid)
            (*_dspace)[i] += d;
      }


      return this;
   }


   Operator * OHermitianMatrix::Scale(const double d)
   {
      *((dMat*) this) *= d;

      if (_valid)
         (*_dspace) *= d;

      scaling=d;
      return this;
   }

   bool QDLIB::OHermitianMatrix::Valid(WaveFunction * Psi)
   {
      WFLevel *PsiLv;

      PsiLv = dynamic_cast<WFLevel*>(Psi);
      if ( PsiLv == NULL) return false;

      if ( Psi->size() != rows() ) return false;

      return true;
   }

   void OHermitianMatrix::InitDspace()
   {
      if (_dspace == NULL)
         _dspace = new dVec(rows());

      if (_X == NULL){
         _X = new dMat(rows(),rows());
         _XT.SetMatrix(_X);
      }

      *_X = *((dMat*) this);
      _dspace->newsize(rows());

      LAPACK::FullDiagHermitian(_X, _dspace);
      _valid = true;
   }

} /* namespace QDLIB */







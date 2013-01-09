#include "OGridNabla.h"
#include "WFGridSystem.h"
#include "Kspace.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridNabla)
   
   OGridNabla::OGridNabla() : ODSpaceCplx(), GridSystem(), _name("OGridNabla"), _fac(1), _kspace(NULL),  _momentum(false), _dim(-1)
   {
   }

   OGridNabla::OGridNabla(int dim) : ODSpaceCplx(), GridSystem(), _name("OGridNabla"), _fac(1), _kspace(NULL),  _momentum(false), _dim(dim)
   {
   }


   OGridNabla::OGridNabla(bool momentum) :
         ODSpaceCplx(), GridSystem(),_name("OGridNabla"), _fac(1), _kspace(NULL), _momentum(momentum), _dim(-1)
   {
   }

   OGridNabla::OGridNabla(bool momentum, int dim) :
         ODSpaceCplx(), GridSystem(),_name("OGridNabla"), _fac(1), _kspace(NULL), _momentum(momentum), _dim(dim)
   {
   }


   OGridNabla::~OGridNabla()
   {
      if (_kspace != NULL) delete _kspace;
      if (_dspace != NULL) delete _dspace;
   }

   void OGridNabla::Init(ParamContainer & params)
   {
      _params = params;

      if(_params.isPresent("factor"))
         _params.GetValue( "factor", _fac);

      if (_params.isPresent("dim")){
         _params.GetValue("dim", _dim);

         if (_dim < 0)
            throw ( EParamProblem("Invalid dimension chosen for momentum operator: ", _dim)  );
      }
   }

   void OGridNabla::Init(WaveFunction * Psi)
   {
      WFGridSystem *psi;

      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
         throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

      *((GridSystem*) this) = *((GridSystem*) psi);

      if (GridSystem::Dim() < _dim + 1)
         throw (EParamProblem ("Dimension for momentum operator exceeds dimensions of Wavefunction", _dim) );

      _InitDspaceReal(); /* Setup internal kspace */
   }

   /** \return largest k-value*/
   dcomplex OGridNabla::Emax()
   {
      return dcomplex( (*_kspace)[GridSystem::Size()/2] );
   }

   void OGridNabla::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      WFGridSystem *ket, *opPsi;

      ket = dynamic_cast<WFGridSystem*>(sourcePsi);
      opPsi = dynamic_cast<WFGridSystem*>(destPsi);

      double sgn = 1;
      if (_conj) sgn = -1; /* Switch sign for complex conjugate */

      _FFT.Forward(ket);
      opPsi->IsKspace(true);
      if (_momentum)
         MultElementsCopy((cVec*) opPsi, (cVec*) ket, _kspace, sgn * _fac/double(GridSystem::Size()));
      else
         MultElementsComplexEq((cVec*) opPsi, (cVec*) ket, _kspace, sgn * _fac/double(GridSystem::Size()));
      
      ket->IsKspace(false);   /* switch back to X-space -> we don't change sourcePsi*/
      _FFT.Backward(opPsi);
   }


   void OGridNabla::Apply(WaveFunction * Psi)
   {
      WFGridSystem *opPsi;

      opPsi = dynamic_cast<WFGridSystem*>(Psi);

      double sgn = 1;
      if (_conj) sgn = -1; /* Switch sign for complex conjugate */

      _FFT.Forward(opPsi);
      if (_momentum)
         MultElements((cVec*) opPsi, _kspace, sgn * _fac/double(GridSystem::Size()));
      else
         MultElementsComplex((cVec*) opPsi, _kspace, sgn * _fac/double(GridSystem::Size()));

      _FFT.Backward(opPsi);
   }

   Operator * OGridNabla::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator * OGridNabla::Copy(Operator * O)
   {
      OGridNabla* o = dynamic_cast<OGridNabla*>(O);

      if (o == NULL)
         throw ( EIncompatible("Operator not of type GridNabla", O->Name()) );

      _params = o->_params;
      _fac = o->_fac;
      _momentum = o->_momentum;

      *((GridSystem*) this) = *((GridSystem*) o);

      if (_kspace == NULL)
         _kspace = new dVec(GridSystem::Size(), true);

      *_kspace = *(o->_kspace);

      return this;
   }

   bool OGridNabla::Valid(WaveFunction * Psi)
   {
      WFGridSystem *PsiGS;

      PsiGS = dynamic_cast<WFGridSystem*>(Psi);
      if ( PsiGS == NULL) return false;

      if ( *((GridSystem*) this) != *((GridSystem*) PsiGS) ) return false;

      return true;
   }

   void OGridNabla::InitDspace()
   {
      if (_kspace == NULL) _InitDspaceReal();

      if (_dspace == NULL ) _dspace = new cVec(GridSystem::Size(), true);

      /** \todo make mpi-save */
      /* Copy to "official" dspace represention of mother class */
      for (int i=0; i < GridSystem::Size(); i++){
         (*_dspace)[i]._imag = -(*_kspace)[i];
      }

   }

   /**
    * Setup k-space for specified dimension.
    */
   void QDLIB::OGridNabla::_InitDim(dVecView & view, const int dim)
   {
      dVec *kspace1; /* Single dimension k-space */

      kspace1 = Kspace::Init1Dddx(static_cast<GridSystem>(*this), dim);;

      view.ActiveDim(dim);
      view += *kspace1;
      delete kspace1;
   }


   /** Init internal k-space only.
    *
    * This saves some effort of copying the real valued vector to the general
    * complex valued vector if not needed
    */
   void OGridNabla::_InitDspaceReal()
   {
      if (_kspace == NULL)
         _kspace = new dVec(GridSystem::Size(), true);
         
      *_kspace = double(0); /* Init with zeros */

      dVecView view(*_kspace, GridSystem::Dim(), GridSystem::DimSizes());

      if (_dim < 0 ) {
         /* Init k-space for every dimension */
         for (int i=0; i < GridSystem::Dim(); i++)
            _InitDim(view, i);
         } else {
            _InitDim(view, _dim);
         }
   }
   
   void OGridNabla::InitExponential (cVec *exp, dcomplex c)
   {
      if (_dspace == NULL) InitDspace();
      
      ExpElements(exp, _dspace, c * _fac);
      *exp *= 1/double(GridSystem::Size());
   }

} /* namespace QDLIB */























#include "OGridNabla.h"
#include "WFGridSystem.h"
#include "Kspace.h"
#include "Butterworth.h"

namespace QDLIB {

   OGridNabla::OGridNabla() : _name("OGridNabla"), _fac(1), _kspace(NULL),  _momentum(false)
   {
   }
   
   OGridNabla::OGridNabla(bool momentum) : _name("OGridNabla"), _fac(1), _kspace(NULL), _momentum(momentum)
   {
   }

   
   OGridNabla::~OGridNabla()
   {
      if (_kspace != NULL) delete _kspace;
   }

   Operator * OGridNabla::NewInstance()
   {
      OGridNabla *r;
      
      r = new OGridNabla();
      
      return r;
   }
   
   void OGridNabla::Init(ParamContainer & params)
   {
      _params = params;
            
      if(_params.isPresent("factor"))
         _params.GetValue( "factor", _fac);
   }
   
   void OGridNabla::Init(WaveFunction * Psi)
   {
      WFGridSystem *psi;
      
      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
         throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );
      
      *((GridSystem*) this) = *((GridSystem*) psi);
      
      _kspace = new dVec(GridSystem::Size(), true);
      
      _InitDspaceReal(); /* Setup internal kspace */
   }

   const string& OGridNabla::Name()
   {
      return _name;
   }
   
   
   dcomplex OGridNabla::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      dcomplex c;
      WaveFunction *opPsi;
      
      opPsi = PsiBra->NewInstance();
      Apply(opPsi, PsiKet);
      c = *PsiBra * opPsi;
      delete opPsi;
      
      return c;
   }
   
   double QDLIB::OGridNabla::Expec(WaveFunction * Psi)
   {
      dcomplex c;
        
      c = MatrixElement(Psi, Psi);
      
      return c.real();
   }
   
   /** \todo Refine function 
    * \return 0
    */
   double OGridNabla::Emax()
   {
      return 0;
   }
   
   WaveFunction * OGridNabla::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      WFGridSystem *ket, *opPsi;
      
      ket = dynamic_cast<WFGridSystem*>(sourcePsi);
      opPsi = dynamic_cast<WFGridSystem*>(destPsi);
      
      ket->ToKspace();
      opPsi->isKspace(true);
      if (_momentum)
         MultElementsCopy((cVec*) opPsi, (cVec*) ket, _kspace, _fac/double(GridSystem::Size()));
      else
         MultElementsComplexEq((cVec*) opPsi, (cVec*) ket, _kspace, _fac/double(GridSystem::Size()));
      ket->isKspace(false);   /* switch back to X-space -> we don't change sourcePsi*/
      opPsi->ToXspace();
         
      return destPsi;
   }
   
   
   WaveFunction * OGridNabla::Apply(WaveFunction * Psi)
   {
      WFGridSystem *opPsi;
      
      opPsi = dynamic_cast<WFGridSystem*>(Psi);

      opPsi->ToKspace();
      if (_momentum)
         MultElements((cVec*) opPsi, _kspace, _fac/double(GridSystem::Size()));
      else 
         MultElementsComplex((cVec*) opPsi, _kspace, _fac/double(GridSystem::Size()));
      opPsi->ToXspace();
      
      return opPsi;
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

   Operator * QDLIB::OGridNabla::operator *(Operator * O)
   {
      throw (EIncompatible("Operator apply not supported", Name(),O->Name()) );
      return this;
   }

   
   Operator * OGridNabla::Scale(const double d)
   {
      if (_kspace == NULL)
         throw ( EParamProblem("k-space not initialized: ", Name()) );
      
      MultElements(_kspace, d);
     
      /* Redirect to parent */
      ODSpaceCplx::Scale(d);
      
      return this;
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
   
   /** Init internal k-space only.
    * 
    * This saves some effort of copying the real valued vector to the general
    * complex valued vector if not needed
    */
   void OGridNabla::_InitDspaceReal()
   {
      dVec *kspace1; /* Single dimension k-space */
      
      *_kspace = 0; /* Init with zeros */
      
      dVecView view(*_kspace, GridSystem::Dim(), GridSystem::DimSizes());
      
      /* Init k-space for every dimension */
      for (int i=0; i < GridSystem::Dim(); i++){ 
         kspace1 = Kspace::Init1Dddx(GridSystem::Xmax(i) - GridSystem::Xmin(i), GridSystem::DimSizes(i));;
         
         view.ActiveDim(i);
         view += *kspace1;
         delete kspace1;
      }
   }
   
   
} /* namespace QDLIB */




















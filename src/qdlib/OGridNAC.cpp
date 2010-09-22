#include "OGridNAC.h"
#include "math/math_functions.h"
#include "qdlib/WFGridSystem.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridNAC)
   
   OGridNAC::OGridNAC():  _name("GridNAC"), _sign(1), _mass(1), _buf(NULL)
   {
            
      for (int i=0; i < MAX_DIMS; i++){
         _pNACME[i] = (dVec*) &_NACME[i];
      }
   }
   
   
   OGridNAC::~OGridNAC()
   {
      DELETE_WF(_buf);
   }

   void OGridNAC::Init(ParamContainer & params)
   {
      
      /* Check dimensions */
      if (_params.isPresent("dim")){
         int dim = 0;
         _params.GetValue("dim", dim);
         
         if (dim < 1)
            throw (EParamProblem("NAC got nonsense dims"));
         
         if (dim > MAX_DIMS)
            throw (EParamProblem("Dims for NAC exceedes MAX_DIMS"));
      } else
         Dim(1); /* Default */
      
      if (!_params.isPresent("file"))
         throw (EParamProblem("NACME file is missing"));
      
      string s;
      _params.GetValue("file",s);
      
      /* Init Grid with NAC file(s) */
      for (int i=0; i < Dim(); i++ ){
         ParamContainer pNAC;
         ParamContainer pNabla;
         char nd[3];
         
         snprintf(nd, 3, "%d", i);
         
         pNAC.SetValue("name", "OGridPotential");
         pNAC.SetValue("file", s + string(nd));
         
         _NACME[i].Init(pNAC);
         
         pNabla.SetValue("dim", i);
         
         _Nabla[i].Init(pNabla);
      }
      
      if (_params.isPresent("sign"))
         _params.GetValue("sign",_sign);
      
      if (Dim() == 1) /* We leave this here for compat reasons */
         if (_params.isPresent("mass"))
            _params.GetValue("mass",_mass);
   }

   
   void OGridNAC::Init(WaveFunction * Psi)
   {
      WFGridSystem *psi;

      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
         throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );
      
      if (Dim() != psi->Dim())
         throw ( EIncompatible("NAC: Psi has different num of dims", Psi->Name()) );
      
      *((GridSystem*) this) = *((GridSystem*) psi);
      
      for (int i=0; i < Dim(); i++ ){
         _Nabla[i].Init(Psi);
         _NACME[i].Init(Psi);
      }
      
      _buf = Psi->NewInstance();
   }

   /** \todo extend to N-D */
   dcomplex OGridNAC::Emax()
   {
      double max = 0;
      
      for (int i=0; i < Dim(); i++ )
         max += VecMax(_NACME[i]);
      
      max *= M_PI/ _mass / GridSystem::Dx(0);
      
      return dcomplex(max);
   }
   
   /** \todo extend to N-D */
   dcomplex OGridNAC::Emin()
   {
      double min = 0;
      
      for (int i=0; i < Dim(); i++ )
         VecMin(_NACME[i]);
      
      min *= M_PI/ _mass / GridSystem::Dx(0);
      
      return dcomplex(min);
   }
   
   dcomplex OGridNAC::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      dcomplex c(0,0);
      
      WaveFunction *opPsi;
      
      opPsi = PsiKet->NewInstance();
      Apply(opPsi, PsiKet);
      
      c = *PsiBra * opPsi;
      
      DELETE_WF(opPsi);
      return c;
   }

   double OGridNAC::Expec(WaveFunction * Psi)
   {
      dcomplex c;
      
      c = MatrixElement(Psi, Psi);
     
      return c.real();
   }

   void OGridNAC::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      for (int i=0; i < Dim(); i++ ){
         /* 1/2 f del psi*/
         _Nabla[i].PreFactor(0.5 * _sign / _mass);
         _Nabla[i].Apply(_buf, sourcePsi);
         _NACME[i].Apply(_buf);
         
         if (i == 0)
            *destPsi = _buf;
         else
            *destPsi += _buf;
         
         /* 1/2 del f psi*/
         _Nabla[i].PreFactor(0.5 * _sign / _mass);
         _NACME[i].Apply(_buf, sourcePsi);
         _Nabla[i].Apply(_buf);
   
         *destPsi += _buf;
      }
   }

   void OGridNAC::Apply(WaveFunction * Psi)
   {
      WaveFunction *opPsi = Psi->NewInstance();
      
      Apply(opPsi, Psi);
      *Psi = opPsi;
      
      delete opPsi;
   }

   Operator * OGridNAC::operator =(Operator * O)
   {
      return Copy(O);
   }

   Operator * OGridNAC::Copy(Operator * O)
   {
      OGridNAC* o = dynamic_cast<OGridNAC*>(O);
      
      if(o == NULL)
         throw (EIncompatible("Operator not of type OGridNAC", O->Name()) );
      
      *((GridSystem*) this) = *((GridSystem*) o);
      
      for (int i=0; i < Dim(); i++ ){
         _NACME[i] = &o->_NACME[i];
         _pNACME[i] = (dVec*) &_NACME[i];
         _Nabla[i] = &o->_Nabla[i];
      }
      _sign = o->_sign;
      _mass = o->_mass;
      
      _buf = o->_buf->NewInstance();
      return this;
   }
   
   bool OGridNAC::Valid(WaveFunction * Psi)
   {
      bool valid = true;
     
      /* Wf ist valid if it is valid with the sub-ordinate operators */
      valid &= _Nabla[0].Valid(Psi);
      valid &= _NACME[0].Valid(Psi);
      
      return valid;
   }
   
} /* namespace QDLIB */









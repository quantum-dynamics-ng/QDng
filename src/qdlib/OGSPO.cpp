#include "OGSPO.h"

namespace QDLIB
{

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGSPO)
   
   OGSPO::OGSPO() :
      _name("OGSPO"), _spoLen(0), _laststep(-1), _lastexp(0)
   {
      _needs.SetValue("A", "");
      _needs.SetValue("B", "opt");
      _needs.SetValue("C", "opt");
      _needs.SetValue("D", "opt");

      for (int i = 0; i < GSPO_MAX_LEN; i++) {
         _ops[i] = NULL;
      }
   }

   OGSPO::~OGSPO()
   {
      DELETE_OP(_H);
//      for (int i = 0; i < GSPO_MAX_LEN; i++) {
//         if (_ops[i] != NULL) {
//            delete _ops[i];
//         }
//      }
   }

   /**
    * Initialize the exponentials.
    *
    * Checks if an update is really needed (time dependent operators).
    */
   void OGSPO::InitExp()
   {
      double sf = 1; /* First Element is middle => factor 1 */

      for (int i = 0; i < _spoLen; i++) {
         if ((_ops[i]->isTimeDep() && _laststep != clock->TimeStep()) || _laststep == -1
                  || _lastexp.real() != Exponent().real() || _lastexp.imag() != Exponent().imag()) {
            _ops[i]->InitExponential(&(_exp[i]), Exponent() * sf);
            /* Include transform normalization here */
            if (_ops[i]->Transformation() != NULL){
               _exp[i] *= _ops[i]->Transformation()->Normalization();
            }
         }
         sf = 0.5; /* All other elements are outer ones => factor 1/2 */
      }
      _lastexp = Exponent();
      _laststep = clock->TimeStep();
   }

   void OGSPO::UpdateTime()
   {
      for (int i = 0; i < _spoLen; i++) {
         _ops[i]->UpdateTime();
      }
   }

   void OGSPO::Init(ParamContainer &params)
   {
      _params = params;
   }

   void OGSPO::Init(WaveFunction *Psi)
   {
      if (clock == NULL)
         throw(EParamProblem("Propagator has no clock!"));

      if (_spoLen < 1)
         throw(EParamProblem("GSPO is empty!"));

      for (int i = 0; i < _spoLen; i++) {
         if (_ops[i] == NULL)
            throw(EParamProblem("GSPO misses an operator. Sequence must be A, B, C, ..."));

//         _ops[i]->Init(Psi); /* Operators init round */
         _exp[i].newsize(Psi->size(), Psi->strides());
      }

   }

   dcomplex OGSPO::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      WaveFunction *opKet;
      opKet = PsiKet->NewInstance();
      dcomplex c;

      Apply(opKet, PsiKet);
      c = *PsiBra * PsiKet;
      DELETE_WF(opKet);
      return c;
   }

   double OGSPO::Expec(WaveFunction *Psi)
   {
      return MatrixElement(Psi, Psi).real();
   }

   void OGSPO::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      *destPsi = sourcePsi;

      Apply(destPsi);
   }

   void OGSPO::Apply(WaveFunction *Psi)
   {
      InitExp();

      /* e^1/2B e^1/2C ..., apply from right */
      for (int i = _spoLen - 1; i > 0; i--) {
         if (_ops[i]->Transformation() != NULL)
            _ops[i]->Transformation()->Forward(Psi);

         MultElements((cVec*) Psi, (cVec*) &(_exp[i]));

         if (_ops[i]->Transformation() != NULL)
            _ops[i]->Transformation()->Backward(Psi);
      }

      /* ... e^1/2C e^1/2B e^A , apply from right */
      for (int i = 0; i < _spoLen; i++) {
         if (_ops[i]->Transformation() != NULL)
            _ops[i]->Transformation()->Forward(Psi);

         MultElements((cVec*) Psi, (cVec*) &(_exp[i]));

         if (_ops[i]->Transformation() != NULL)
            _ops[i]->Transformation()->Backward(Psi);
      }
   }

   Operator* OGSPO::operator=(Operator* O)
   {
      return Copy(O);
   }

   Operator* OGSPO::Copy(Operator* O)
   {
      OGSPO *o = dynamic_cast<OGSPO*> (O);

      if (o == NULL)
         throw(EIncompatible("GSPO: wrong type for copy", O->Name()));

      OPropagator::Copy(O); /* Copy parent */

      _spoLen = o->_spoLen;

      for (int i = 0; i < _spoLen; i++) {
         _ops[i] = dynamic_cast<ODSpace*>(o->_ops[i]->NewInstance());
         _ops[i]->Copy(o->_ops[i]);
         _exp[i].newsize(o->_exp[i].size(), o->_exp[i].strides());
      }
      _laststep = -1;

      return this;
   }

   bool OGSPO::Valid(WaveFunction *Psi)
   {
      bool valid = false;

      for (int i = 0; i < _spoLen; i++) {
         if (_ops[i]->Valid(Psi) == false)
            return false;
      }

      return valid;
   }

   void OGSPO::AddNeeds(string &Key, Operator *O)
   {
      char base = 'A';
      const char *key = Key.c_str();

      char i = key[0] - base;
      	 
      if (i < GSPO_MAX_LEN && i > -1) {
	 if (int(i) < _spoLen)
	    throw(EParamProblem("GSPO: SPO-key already given ", key)); 

         ODSpace *op = dynamic_cast<ODSpace*> (O);

         if (op == NULL)
            throw(EIncompatible("GSPO: Operator has no diagonal representation", O->Name()));

         _ops[int(i)] = op;
         _spoLen++;
      } else {
         throw(EParamProblem("GSPO: Invalid SPO - key given ", key));
      }
      
      if (H == NULL) {
         _H = new OSum();
         CollectorOp::Instance()->Register(_H);
         H = _H;
      }   
      _H->Add(O);
   }

}


// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "OGSPO.h"

namespace QDLIB
{

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGSPO)
   
   OGSPO::OGSPO() :
      _name("OGSPO"), _spoLen(0), _laststep(-1), _lastexp(0)
   {
      for (int i = 0; i < MAX_OPS; i++) {
         _ops[i] = NULL;
      }
   }

   OGSPO::~OGSPO()
   {
      DELETE_OP(H);
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
      if (_exp[0].size() > 0) return; // Avoid init twice


      /* Initalize with all elements hamiltonian */
      if (H == NULL) {
         H = new OSum();
      }

      /* Grab and check operators from list */
      for (int i=0; i < OList::Size(); i++) {
         char base[2] = "A";
         base[0] += i;

         Operator* O = Get(string(base));

         ODSpace *op = dynamic_cast<ODSpace*> (O);
         if (op == NULL)
            throw(EIncompatible("GSPO: Operator has no diagonal representation", O->Name()));

         _ops[int(i)] = op;
         _spoLen++;
         dynamic_cast<OSum*>(H)->Add(string(base), op);
      }

      _spoLen = OList::Size();

      if (clock == NULL)
         throw(EParamProblem("Propagator has no clock!"));

      if (_spoLen < 1)
         throw(EParamProblem("GSPO is empty!"));
      
      for (int i = 0; i < _spoLen; i++) {
         if (_ops[i] == NULL)
            throw(EParamProblem("GSPO misses an operator. Sequence must be A, B, C, ..."));

         _exp[i].newsize(Psi->size(), Psi->strides());
      }

      OPropagator::Init(Psi);
      H->Init(Psi);
      H->Clock(Operator::Clock());
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

}


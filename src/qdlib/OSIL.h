// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOSIL_H
#define QDLIBOSIL_H

#include "qdlib/OPropagator.h"
#include "qdlib/WFBuffer.h"

#define SIL_DEF_ORDER 20
#define SIL_DEF_ERR 1e-12

namespace QDLIB {

    /**
     * Short iterative Lanczos Propagator.
     *
     * \param order   Recursion depth/Lanczos basis size. the default is 20.
     * \param err     Approximated series abortion error. [1e-20]
     *
     *     @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
     */

    class OSIL : public OPropagator
    {
        private:
            string _name;
            int _order;       /* Max. Recursion depth, size of basis */
            int _convorder;    /* Converged order of */
            double _err;       /* Error bound to use */
            WFBuffer _Lzb;  /* Buffer for Lanczos basis */
            WaveFunction *buf0, *buf1, *buf2;   /* Buffers */ 
            dVec _alpha;      /* Store alpha coeffs */
            dVec _beta;       /* Store beta coeffs */
            cVec _vect;       /* time vector in Lanczos basis */
            cVec _vec0;       /* time vector in Lanczos basis */
            cVec _expHD;       /* Exponential vector */
            dMat _Z;          /* Lanczos eigenvectors */
            
            void InitBuffers();
            void BuildLZB(WaveFunction* Psi);
            void DiagLZB(WaveFunction* Psi);
        public:
            OSIL();

            ~OSIL();

            virtual Operator* NewInstance();
            virtual void Init(ParamContainer &params);
            virtual void Init(WaveFunction *Psi);
            virtual const string& Name();
            virtual void UpdateTime() {if (H != NULL) H->UpdateTime();}
            virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
            virtual void Apply(WaveFunction *Psi);
            virtual Operator* operator=(Operator* O);
            virtual Operator* Copy(Operator* O);
            virtual bool Valid(WaveFunction *Psi);
    };

}

#endif

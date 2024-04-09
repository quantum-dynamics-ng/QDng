// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOARNOLDI_H
#define QDLIBOARNOLDI_H

#include "qdlib/OPropagator.h"
#include "qdlib/WFBuffer.h"

#define ARNOLDI_DEF_ORDER 20
#define ARNOLDI_UNDERRUN 1e-11
#define ARNOLDI_CONV 1e-12

namespace QDLIB {

    /**
     * Arnoldi Propagator.
     *
     * Generalisation of SIL. Can also handle non-hermitian hamiltonians.
     *
     * \param order   Maximum recursion depth/Ritz basis size. the default is 20.
     * \param conv    Approiximate error bound for time-stepping [1e-8].
     *
     *     @author Markus Kowalewski <markus.kowalewski@it.uu.se>
     */

    class OArnoldi : public OPropagator
    {
        private:
            string _name;
            int _maxorder;     /* Max. Recursion depth, size of basis */
            int _order;        /* Converged order of */
            double _conv;      /* Convergence criteria */
            WFBuffer _Lzb;     /* Buffer for Lanczos basis */
            WaveFunction *buf0, *buf1;   /* Buffers */
            cVec _evals;      /* Eigenvalues */
            cMat _HA;         /* Arnoldi hamiltonian */
            cMat _tHA;         /* temp.  Arnoldi hamiltonian */
            cVec _vect;       /* time vector in Lanczos basis */
            cVec _vec0;       /* time vector in Lanczos basis */
            cVec _expHD;       /* Exponential vector */
            cMat _ZL;          /* Ritz eigenvectors */
            cMat _ZR;          /* Ritz eigenvectors */

            void InitBuffers();
            void Propagate(WaveFunction* Psi);
        public:
            OArnoldi();
            ~OArnoldi();

            /* Provide Interface to internal data */

            /**
             * Iterate to build the Arnoldi basis vectors.
             * \f$AQ = Q* H\f$
             *
             * The approximate hamiltonian is stored internally
             * and can be diaognalized with DiagLZB().
             */
            void BuildLZB(WaveFunction* Psi);

            /**
             * Provides the Basisvectors created by BuildLZB.
             */
            WFBuffer* Basis() { return &_Lzb; }

            /**
             * Diagonalize the Arnoldi Hamiltonian.
             **/
            void DiagLZB();

            cVec* Evals() { return &_evals; }
            cMat* Evecs() { return &_ZR; }

            /**
             * Number of converged eigenvectors.
             */
            int Size() { return _order; }

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

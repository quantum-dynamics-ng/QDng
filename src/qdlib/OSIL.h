#ifndef QDLIBOSIL_H
#define QDLIBOSIL_H

#include "OPropagator.h"

#define SIL_MAX_ORDER 32
#define SIL_CUTOFF_BETA 1e4
#define SIL_CUTOFF_ALPHA 1e-8

namespace QDLIB {

    /**
     * Short iterative Lanczos Propagator.
     *
     * \param order   Recursion depth/Lanczos basis size. Values <= 0 mean autodetect.
     *
     *     @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
     */

    class OSIL : public OPropagator
    {
        private:
            string _name;
            ParamContainer _needs;
            int _order;       /* Recursion depth, size of basis */
            Operator* _hamilton;      /* The hamiltonian */
            WaveFunction* _Lzb[SIL_MAX_ORDER];  /* Buffer for Lanczos basis */
            WaveFunction *buf0, *buf1, *buf2;   /* Buffers */ 
            dVec _alpha;      /* Store alpha coeffs */
            dVec _beta;       /* Store beta coeffs */
            cVec _vect;       /* time vector in Lanczos basis */
            cVec _vec0;       /* time vector in Lanczos basis */
            dVec _HD;         /* diagonal Hamiltonian */
            cVec _expHD;       /* Exponential vector */
            dMat _Z;          /* Lanczos eigenvectors */
            
            void PropaLCB(WaveFunction* Psi, int basisLength);
        public:
            OSIL();

            ~OSIL();

            virtual Operator* NewInstance();
            virtual void Init(ParamContainer &params);
            virtual void Init(WaveFunction *Psi);
            virtual const string& Name() {return _name;}
            virtual void UpdateTime() {if (_hamilton != NULL) _hamilton->UpdateTime();}
            virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
            virtual double Expec(WaveFunction *Psi);
            virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
            virtual WaveFunction* Apply(WaveFunction *Psi);
            virtual Operator* operator=(Operator* O);
            virtual Operator* Copy(Operator* O);
            virtual Operator* operator*(Operator* O);
            virtual bool Valid(WaveFunction *Psi);

            /* Interface implementation, Propagator */
            virtual ParamContainer& TellNeeds();

            virtual void AddNeeds(string &Key, Operator *O);
    };

}

#endif

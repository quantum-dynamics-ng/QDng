#ifndef QDLIBOGRIDKICK_H
#define QDLIBOGRIDKICK_H

#include <Operator.h>
#include <GridSystem.h>

namespace QDLIB {

    /**
     *  This class is intendend to add distinct momentum to the WF
     * 
     *      <p>
     * Takes the parameters:
     * - dims
     * - kn  Momentum for coordinate \f$ x_n \f$. A missing
     * value for a dimension means zero.
     * 
     * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
     */
    class OGridKick : public Operator, public GridSystem
    {
       private:
          string _name;
          double _kn[MAX_DIMS];
          cVec *_exp;
          
        public:
            OGridKick();

            ~OGridKick();
            virtual Operator* NewInstance();

            virtual void Init(ParamContainer &params);

            virtual void Init(WaveFunction *Psi);

            virtual const string& Name();

            virtual void UpdateTime(){}

            virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);

            virtual double Expec(WaveFunction *Psi);

            virtual double Emax(){return 0;}

            virtual double Emin(){return 0;}

            virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

            virtual WaveFunction* Apply(WaveFunction *Psi);

            virtual Operator* operator=(Operator* O);

            virtual Operator* Copy(Operator* O);

            virtual Operator* operator*(Operator* O);

            virtual bool Valid(WaveFunction *Psi);
            
            virtual Operator* Offset(const double d){ return this;}

            virtual Operator* Scale(const double d){scaling = d; return this;};

    };

}

#endif
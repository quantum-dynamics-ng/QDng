#ifndef QDLIBOGRIDKICK_H
#define QDLIBOGRIDKICK_H

#include "qdlib/Operator.h"
#include "qdlib/GridSystem.h"

namespace QDLIB {

    /**
     *  This class is intendend to add distinct momentum to the WF
     * 
     *      <p>
     * Takes the parameters:
     * \li dims
     * \li kn  Momentum for coordinate \f$ x_n \f$. A missing
     *   value for a dimension means zero.
     * \li scale Scale factor for momentum vector.
     * 
     * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
     */
    class OGridKick : public Operator, public GridSystem
    {
       private:
          string _name;
          dVec _kn;        /* k-value for every dim */
          cVec *_exp;
          
        public:
            OGridKick();

            ~OGridKick();
            virtual Operator* NewInstance();

            virtual void Init(ParamContainer &params);

            virtual void Init(WaveFunction *Psi);

            virtual const string& Name();

            virtual void UpdateTime(){}

	    virtual dcomplex Emax() {return dcomplex(0);}

	    virtual dcomplex Emin() {return dcomplex(0);}

            virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

            virtual void Apply(WaveFunction *Psi);

            virtual Operator* operator=(Operator* O);

            virtual Operator* Copy(Operator* O);

            virtual bool Valid(WaveFunction *Psi);
    };

}

#endif

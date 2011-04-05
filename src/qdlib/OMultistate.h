#ifndef QDLIBOMULTISTATE_H
#define QDLIBOMULTISTATE_H

#include "qdlib/Operator.h"
#include "qdlib/WFMultistate.h"

namespace QDLIB
{

   /**
    * Multistate operator.
    * 
    * Takes several operators as matrix elementsments and acts as a hermitian
    * operator with real matrix elements.
    * 
    * parameters:
    * \li nonhermitian   Explicitly define a non-hermitian operator 
    * \li unity          Treat empty diagonals as 1 (default: false)
    * 
    *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OMultistate : public Operator
   {
      private:
	 string _name;
	 
	 bool _hermitian;  /* Assume hermiticity for missing elements */
	 int _nstates;
         bool _unity;      /* treat empty diagonal as unity */
         bool _subinit;    /* Initial content ops */
	 
	 Operator* _matrix[QD_MAX_STATES][QD_MAX_STATES];
	 
	 WFMultistate *_buf1, *_buf2;
	 
      public:
         OMultistate();

         ~OMultistate();
	 
	 /** Set hermitian/symmetric behavior.
          * 
	  * The default is to act as symmetric operator if only one diag. element is used.
	  */
	 void Hermitian(bool hermit) {_hermitian = hermit;}
	 
	 bool Hermitian() {return _hermitian;}
	 
         /**
          * Set behavior for missing diagonal elements.
          * 
          * The default is true.
          * \param unity If true, assume 1 for missing elements
          */
         void Unity(bool unity) {_unity= unity;}
	 
         bool Unity() {return _hermitian;}
         
         /**
          * Initialize content operators with Init(Wavefunction*).
          * 
          * This feature is on per default.
          */
         void SubInit(bool init) { _subinit = init; }
         
	 void Add(Operator *O, int row, int col);
	 
	 /** Number of states */
	 int States() {return _nstates;}
	 
	 
	 /** Explictly set the number of states. 
	  *  Usually this is done automaticly in the add method.
	  */
	 void States(int n) { _nstates = n; }
	 
	 Operator* State(int i, int j);
	 
	 /* Overide Clock setter */
	 virtual void Clock (QDClock *cl);
	 
	 /* Interface implementation */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual void Init(WaveFunction *Psi);
	 
	 virtual const string& Name();
	                
	 virtual void UpdateTime();
	 
	 virtual dcomplex Emax();
	 
	 virtual dcomplex Emin();
	 
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual void Apply(WaveFunction *Psi);
         
         virtual void ApplyParent(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	 
         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif

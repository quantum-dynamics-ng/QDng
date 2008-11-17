#ifndef QDLIBREPORTER_H
#define QDLIBREPORTER_H

#include "qdlib/WaveFunction.h"

namespace QDLIB
{

   /**
    * Report some analysis of the propagation.
    *
    *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class Reporter
   {
      private:
	 bool _norm;
	 bool _energy;
	 bool _proj0;
	 bool _spectrum;
	 
	 WaveFunction *_psi0;
	 string _specname;
	 ofstream _rfile;
	 cVec _specbuf;
	 lint _step;
	 
	 
      public:
         Reporter();
         ~Reporter();
	 
	 void PsiInitial(WaveFunction* Psi);
	 void Analyze(WaveFunction* Psi);
	 
	 bool Norm() const { return _norm; }
	 void Norm(bool on) { _norm = on; }
	 
	 bool Energy() const { return _energy; }
	 void Energy(bool on){ _energy = on; };
	 
	 bool Proj0() const { return _proj0; }
	 void Proj0(bool on) { _proj0 = on; }
	 
	 bool Spectrum() const { return _spectrum; }
	 void Spectrum(const string &name);
   };

}

#endif

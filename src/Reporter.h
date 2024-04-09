// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBREPORTER_H
#define QDLIBREPORTER_H

#include "qdlib/WFMultistate.h"
#include "qdlib/Operator.h"

#include <iostream>
#include <fstream>

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
	 lint _wcycle;
	 bool _norm;
         bool _scinorm;    /* print norm in scientific format */
	 bool _energy;
	 bool _proj0;
	 bool _proj0Sq;
	 bool _spectrum;
         bool _window;
	 bool _multistate;
	 
	 
	 WaveFunction *_psi0;
	 WFMultistate *_psims0;
	 
	 Operator *_H;
	 string _specname;
	 ofstream _rfile;
	 cVec _specbuf;
	 lint _step;
	 
	 
      public:
         Reporter();
         ~Reporter();
	 
         static void WriteSpectrum(cVec &correllation, double dt, const string& name);
         
	 void PsiInitial(WaveFunction* Psi);
	 void Hamilton(Operator *H);
	       
	 void Analyze(WaveFunction* Psi);
	 void Finalize();
	 
	 /** Write every n-steps*/
	 void WriteCycle(lint wcycle){ _wcycle = wcycle; }
	 
	 /** Report norm */
	 bool Norm() const { return _norm; }
         
	 /** Report norm */
	 void Norm(bool on) { _norm = on; }
	 
         /** Norm in Scientific format */
         void SciNorm (bool on) { _scinorm = on; }
         
         /** Norm in Scientific format */
         bool SciNorm () { return _scinorm; }
         
	 /** Report energy */
	 bool Energy() const { return _energy; }
	 /** Report energy */
	 void Energy(bool on){ _energy = on; };
	 
	 /** Autocorellation */
	 bool Proj0() const { return _proj0; }
	 
	 /** Autocorellation */
	 void Proj0(bool on) { _proj0 = on; }
	 
	 /** Absolute square off the Autocorrelation **/
	 void Proj0Square(bool on) {_proj0Sq = on; _proj0 = on | _proj0; }
	 
	 /** Absolute square off the Autocorrelation **/
	 bool Proj0Square() { return _proj0Sq; }
	 
	 /** Auto correllation spectrum on?*/
	 bool Spectrum() const { return _spectrum; }
	 /** File name of auto correllation spectrum. Turns it on automatically */
	 void Spectrum(const string &name);
         
         /** Apply a window function to autocorrelation */
         bool Window() const { return _window; }
         /** Apply a window function to autocorrelation */
         void Window(const bool on) { _window = on; }
   };

}

#endif

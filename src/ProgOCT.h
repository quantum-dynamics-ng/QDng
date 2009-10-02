#ifndef QDLIBPROGOCT_H
#define QDLIBPROGOCT_H

#include "tools/Exception.h"
#include "tools/XmlNode.h"
#include "tools/QDGlobalClock.h"
#include "qdlib/OPropagator.h"
#include "qdlib/OGridDipole.h"

#define DEFAULT_BASENAME_LASER "laser"
#define DEFAULT_ITERATIONS 50
#define DEFAULT_CONVERGENCE 1e-6
#define MAX_TARGETS 16
#define MAX_LASERS 4

namespace QDLIB {

   /**
   * 
   * Main module for optimal control theory.
   * 
   * Multi-target mode is supported.
   * 
   * XmlNode Attributes:
   * \li dt       time step
   * \li steps    number of time steps
   * \li dir      Output directory
   * \li method   OCT method to use (krotov, rabiz)
   * \li coup     The type of coupling to optimize (dipole)
   * \li ttype    target type (operator,overlap)
   * \li phase    phase sensitive objective (true|false, default false)
   * \li ntargets Number of targets
   * \li alpha    The Field restriction value
   * \li iterations Maximum number of iterations
   * \li conv     Target convergence. Stop iterating if below convergence
   * \li shape    Name of shapefile
   * \li writel   Write laser in every iteration
   * \li membuf   Buffer back propagated targets in memory (default true)
   * 
   * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class ProgOCT{
      private:
	 typedef enum {krotov, rabitz} _method_t;
	 typedef enum {dipole} _coupling_t;
	 typedef enum {op, ov} _ttype_t;
	 
	 XmlNode &_octNode;
	 XmlNode *_ContentNodes; 
	 	 
	 string _fname;
	 string _dir;
	 
	 int _iterations;
	 double _convergence;
	 bool _writel;
         bool _membuf;
	 
	 _method_t _method;
	 _coupling_t _coupling;
	 _ttype_t _ttype;
	 bool _phase;       /* Phase sensitive OCT */
         
	 int _ntargets;
	 double _alpha;
	 
	 OPropagator *_Uf;  /* Forward Propagation */
         OPropagator *_Ub;  /* Backward Propagation */
	 Operator *_H;      /* Hamiltonian */
	 Operator *_Coup;   /* The coupling to optimize */
	 
	 Laser* _laserf[MAX_LASERS];
	 Laser* _laserb[MAX_LASERS];
	 Laser _shape[MAX_LASERS];
	 
         Operator* _Otarget;     /* Target operator for OPOC */
         
	 WaveFunction* PsiI[MAX_TARGETS];
	 WaveFunction* PsiT[MAX_TARGETS];
         WaveFunction* _opwf;             /* Buffer for mu*psi */
         WaveFunction*** _memwfbuf;       /* memory buffer for backpropagation */
	 bool _membuf_init;               /* indicate initailized membuf */
         
	 void _InitParams();
	 
      protected:
	 double CalcLaserField(WaveFunction** wfi, WaveFunction** wft);
         double Report(WaveFunction **wfi, WaveFunction** wft, int iteration);
      public:
	 ProgOCT(XmlNode &OCTNode);
	 ~ProgOCT();

	 void SetDirectory(string &directory) {_dir = directory;}
	    
	 void Run();

   };

}

#endif

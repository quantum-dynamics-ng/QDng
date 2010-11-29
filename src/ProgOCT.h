#ifndef QDLIBPROGOCT_H
#define QDLIBPROGOCT_H

#include "tools/Exception.h"
#include "tools/XmlNode.h"
#include "tools/QDGlobalClock.h"
#include "qdlib/OPropagator.h"
#include "qdlib/OGridDipole.h"
#include "qdlib/WFBuffer.h"

#define DEFAULT_BASENAME_LASER "laser"
#define DEFAULT_ITERATIONS 50
#define DEFAULT_CONVERGENCE 1e-5
#define MAX_TARGETS 16
#define MAX_LASERS 16

namespace QDLIB
{

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
    * \li method   OCT method to use (krotov, rabiz, rabitzfb)
    * \li coup     The type of coupling to optimize (dipole)
    * \li ttype    target type (operator,overlap)
    * \li phase    phase sensitive objective (true|false, default false)
    * \li ntargets Number of targets
    * \li mv       Moving target (true|false, default false)
    * \li alpha    The Field restriction value
    * \li iterations Maximum number of iterations
    * \li conv     Target convergence. Stop iterating if below convergence
    * \li shape    Name of shapefile
    * \li writel   Write laser in every iteration
    * \li writey   Write Norm and Yild to seperate file
    * \li yfile    Name of the Yield file
    *
    * rabitzfb = Rabitz feedback iterative scheme (W. Zhu and H. Rabitz, J. Chem. Phys 109, 385 (1998))
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class ProgOCT
   {
      private:
         typedef enum
         {
            krotov, rabitz, rabitzfb, freq
         } _method_t;
         typedef enum
         {
            dipole
         } _coupling_t;
         typedef enum
         {
            op, ov
         } _ttype_t;

         XmlNode &_octNode;
         XmlNode *_ContentNodes;

         string _fname;
         string _dir;
         string _yfile;
         bool _writeyield;

         int _iterations; /* Maximum number of iterations */
         double _convergence; /* Convergence ctriteria */
         bool _writel; /* Write laser in every iteration */

         _method_t _method;
         _coupling_t _coupling;
         _ttype_t _ttype;
         bool _phase; /* Phase sensitive OCT */

         int _ntargets;
         bool _mv; /* Moving target */
         double _alpha;

         OPropagator *_U; /* Forward Propagation */
         Operator *_Coup; /* The coupling to optimize */

         Operator *_Gobbler; /* Gobbler operator */

         double _laserobj[MAX_LASERS]; /* Buffer for the Laser fluence contraints */
         Laser* _laserf[MAX_LASERS]; /* Forward propagation lasers */
         Laser* _laserb[MAX_LASERS]; /* Backward propagation lasers */
         int _nlaser; /* Number of (identical) lasers */
         Laser _shape[MAX_LASERS]; /* temporal shape function */
         Laser _frqmask[MAX_LASERS]; /* freq. mask for freq. OCT*/
         Laser _gamma[MAX_LASERS]; /* Correction field for freq. OCT*/

         Operator* _Otarget[MAX_TARGETS]; /* Target operator for OPOC */

         WaveFunction* PsiI[MAX_TARGETS];
         WaveFunction* PsiT[MAX_TARGETS];
         WaveFunction* _opwf; /* Buffer for mu*psi */
         WFBuffer _memwfbuf[MAX_TARGETS]; /* memory buffer for backpropagation */
         bool _membuf_init; /* indicate initailized membuf */

         void _InitParams();

         inline void _CopyWFs(WaveFunction **wfa, WaveFunction **wfb)
         {
            for (int t = 0; t < _ntargets; t++)
               *(wfa[t]) = wfb[t];
         }
      protected:
         double CalcCorr(WaveFunction** wfi, WaveFunction** wft);
         double CalcLaserField(WaveFunction** wfi, WaveFunction** wft);
         double Report(WaveFunction **wfi, WaveFunction** wft, int iteration);
         void PropagateForward(WaveFunction **wf);
         void PropagateBackward(WaveFunction **wf);
         void SyncTargetOverlap(WaveFunction** phii, WaveFunction** phit, int step);
         void SyncTargetOperator(WaveFunction** phii, WaveFunction** phit, int step);
         void Iterate(WaveFunction** phii, WaveFunction** phit, int step);
         void IterateRabitzFB(WaveFunction** phii, WaveFunction** phit, int step);
         void IterateFreq(WaveFunction** phii, WaveFunction** phit, int step);
      public:
         ProgOCT(XmlNode &OCTNode);
         ~ProgOCT();

         void SetDirectory(string &directory)
         {
            _dir = directory;
         }

         void Run();

   };

}

#endif

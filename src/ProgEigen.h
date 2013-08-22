#ifndef QDLIBPROGEIGEN_H
#define QDLIBPROGEIGEN_H

#include "tools/XmlNode.h"

#include "qdlib/OProjection.h"
#include "qdlib/OPropagator.h"

#include "qdlib/FileWF.h"

#define DEFAULT_NUMBER_EFS      20
#define DEFAULT_CONVERGENCE_EF_RAW  1e-8
#define DEFAULT_CONVERGENCE_EF_DIAG  1e-5
#define DEFAULT_MAXSTEPS        512
#define DEFAULT_EF_BASE_NAME    "ef"
#define DEFAULT_EF_ENERGY_NAME  "energy.dat"
#define DEFAULT_EF_SPECTRUM_NAME  "spectrum.dat"



namespace QDLIB
{

   /**
    *  Programm part for calculating eigenfunctions of bound potentials.
    * 
    *  The calculation is done via imaginary time propagation.
    * 
    * Options:
    *  \li method [imag,ac] Eigenfunction either via imaginary time propgation or auto correlation.
    *  \li dt
    *  \li steps  Maximum number of steps
    *  \li dir    Output directory
    *  \li fname  Base name for ef output
    *  \li ename  Name for energy.dat
    *  \li diag   true/false diagonalize the Hamiltonian in the basis of the calc. eigenfunctions
    * Only for method imag:
    *  \li Nef    Number of eigenfunctions
    *  \li conv   Convergence
    *  \li start  Do not start with zeroeth EF but read already calculated
    * Only for method ac:
    *  \li tol    Tolerance factor for peak finder (in units of sigma)
    *  \li spectrum Write eigenvalue spectrum to given file name.
    *  \li read   Read propagation from the given location.
    *  \li win    Apply a window function.
    *  S. MAHAPATRA , N. CHAKRABARTI and N. SATHYAMURTHY, Int. Rev. Phys. Chem. 18, 235 (1999).
    *
    * 
    *  @author Markus Kowalewski
    */
   class ProgEigen
   {

      private:
         typedef enum
         {
            imag, ac, lanczos
         } _method_t;
         /* Input */
         XmlNode &_EigenNode;
         XmlNode *_ContentNodes;

         /* Operators */
         OPropagator *_U;
         Operator *_H;
         OProjection *_P;

         /* Parameters */
         _method_t _method;
         string _dir;
         lint _Nef;
         double _convergence;
         int _MaxSteps;
         string _fname;
         string _ename;
         string _spectrum;
         string _read;
         double _dt;
         bool _diag; /* Diagonalize Basis */
         int _start;
         double _tol;
         int _width;
         bool _win;

         WaveFunction *_PsiInitial;

         FileWF _efile;

         dVec _Energies_raw;
         dVec _Energies_diag;

         void
         _InitParams();
         void
         _CreateInitalWF();

      protected:
         void WriteEnergyFile();
         void ImagTimeEF();
         void AutoCorrEF();
         void LanczosEF();
         void DiagBasis();
      public:
         ProgEigen(XmlNode &EigenNode);

         void
         SetDirectory(string &directory)
         {
            _dir = directory;
         }

         void
         Run();

         ~ProgEigen();

   };

}

#endif

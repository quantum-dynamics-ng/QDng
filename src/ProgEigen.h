#ifndef QDLIBPROGEIGEN_H
#define QDLIBPROGEIGEN_H

#include "tools/XmlNode.h"

#include "qdlib/OProjection.h"
#include "qdlib/OPropagator.h"

#include "qdlib/FileWF.h"

#define DEFAULT_NUMBER_EFS      20
#define DEFAULT_CONVERGENCE_EF_RAW  1e-8
#define DEFAULT_CONVERGENCE_EF_DIAG  1e-5
#define DEFAULT_MAXSTEPS        10000
#define DEFAULT_EF_BASE_NAME    "ef"
#define DEFAULT_EF_ENERGY_NAME  "energy.dat"



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
    * 
    *  @author Markus Kowalewski
    */
   class ProgEigen
   {

      private:
         typedef enum {imag, ac} _method_t;
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
	 double _dt;
	 bool _diag; /* Diagonalize Basis */
         int _start;
         double _tol;
         
         WaveFunction *_PsiInitial;
         
         FileWF _efile;
         
	 dVec _Energies_raw;
	 dVec _Energies_diag;
	 
	 void _InitParams();
	 void _CreateInitalWF();
	 
      protected:
	 void WriteEnergyFile();
         void ImagTimeEF();
         void AutoCorrEF();
      public:
	 ProgEigen(XmlNode &EigenNode);
	 
	 void SetDirectory(string &directory) {_dir = directory;}
	       
	 void Run();
         
	 ~ProgEigen();

   };

}

#endif

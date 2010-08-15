#ifndef QDLIBPROGEIGEN_H
#define QDLIBPROGEIGEN_H

#include "tools/XmlNode.h"

#include "qdlib/OProjection.h"
#include "qdlib/OPropagator.h"

#include "tools/XmlNode.h"


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
    *  \li dt
    *  \li conv   Convergence
    *  \li steps  Maximum number of steps
    *  \li Nef    Number of eigenfunctions
    *  \li dir    Output directory
    *  \li fname  Base name for ef output
    *  \li ename  Name for energy.dat
    *  \li diag   true/false diagonalize the eigen basis
    *  \li start  Do not start with zeroeth EF but read already calculated
    * 
    *  \todo diagonalization
    *  @author Markus Kowalewski
    */
   class ProgEigen
   {

      private:
	 /* Input */
	 XmlNode &_EigenNode;
	 XmlNode *_ContentNodes;
	 
	 /* Operators */
	 OPropagator *_U;
	 Operator *_H;
	 OProjection _P;
	 
	 /* Parameters */
	 string _dir;
	 lint _Nef;
	 double _convergence;
	 int _MaxSteps;
	 string _fname;
	 string _ename;
	 double _dt;
	 bool _diag; /* Diagonalize Basis */
         int _start;
         
	 dVec _Energies_raw;
	 dVec _Energies_diag;
	 
	 void _InitParams();
	 void _CreateInitalWF(); 
	 
      protected:
	 void WriteEnergyFile();
      public:
	 ProgEigen(XmlNode &EigenNode);
	 
	 void SetDirectory(string &directory) {_dir = directory;}
	       
	 void Run();
         
	 ~ProgEigen();

   };

}

#endif

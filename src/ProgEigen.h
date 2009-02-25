#ifndef QDLIBPROGEIGEN_H
#define QDLIBPROGEIGEN_H

#include "qdlib/Operator.h"
#include "qdlib/OProjection.h"


#define DEFAULT_NUMBER_EFS      20
#define DEFAULT_CONVERGENCE_EF  1e-5
#define DEFAULT_NCYCLE          10
#define DEFAULT_MAXSTEPS        10000
#define DEFAULT_EF_BASE_NAME    "ef"



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
    *  \li ncycle Normalize & check for convergence every ncycle steps
    *  \li Nef    Number of eigenfunctions
    *  \li dir    Output directory
    *  \li fname  Base name for ef output
    * 
    *  \todo diagonalization
    *  @author Markus Kowalewski
    */
   class ProgEigen
   {

      private:
	 XmlNode &_EigenNode;
	 XmlNode *_ContentNodes;
	 
	 OPropagator *_U;
	 Operator *_H;
	 OProjection _P;
	 
	 string _dir;
	 lint _Nef;
	 double _convergence;
	 double _MaxSteps;
	 int _ncycle;
	 string _fname;
	 double _dt;
	 
	 void _InitParams();
	 void _CreateInitalWF(); 
      public:
	 ProgEigen(XmlNode &EigenNode);
	 
	 void Run(string &directory);
         
	 ~ProgEigen();

   };

}

#endif

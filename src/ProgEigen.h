#ifndef QDLIBPROGEIGEN_H
#define QDLIBPROGEIGEN_H

#define DEFAULT_NUMBER_EFS
#define DEFAULT_CONVERGENCE_EF

namespace QDLIB
{

   /**
    *  Programm part for calculating eigenfunctions of bound potentials.
    * 
    * Options:
    *  \li dt
    *  \li conv   Convergence
    *  \li N      Number of eigenfunctions
    * 
    *  @author Markus Kowalewski
    */
   class ProgEigen
   {

      private:
	 XmlNode &_EigenNode;
	 XmlNode *_ContentNodes;
	 
	 OPropagator *_U;
	 Operator *_H;
	 
	 string _dir;
	 lint _Nef;
	 double _convergence;
	 
	 WaveFunction** _efbuf;
	 
	 void _InitParams();
	 void _CreateInitalWF();
	 
      public:
	 ProgEigen(XmlNode &EigenNode);
	 
	 void Run();
         
	 ~ProgEigen();

   };

}

#endif

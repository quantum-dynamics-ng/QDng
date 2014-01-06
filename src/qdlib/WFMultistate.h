#ifndef QDLIBWFMULTISTATE_H
#define QDLIBWFMULTISTATE_H

#include "qdlib/WaveFunction.h"

#define QD_MAX_STATES 16

namespace QDLIB
{

   /**
    * Multistate wave function acts as a container for WaveFunction.
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class WFMultistate : public WaveFunction
   {
      private:
	 string _name;
	 lint _nstates;
	 WaveFunction* _states[QD_MAX_STATES];
	 
	 void _destroy();
      public:
         WFMultistate();
         ~WFMultistate();

	 void Add(WaveFunction* Psi, lint index);
	 
	 lint States();
	 WaveFunction* State(lint index);
	 
	 void Init(int reqstates);

	 /* Interface implementation WaveFunction */
	 virtual WaveFunction* NewInstance();
	 
	 virtual void Init(ParamContainer &params);
	 
	 virtual const string& Name();
	 
	 virtual double Norm();
	 	 
	 virtual void Normalize();
      
	 virtual WaveFunction* operator=(WaveFunction* Psi);
	 
	 virtual dcomplex operator*(WaveFunction* Psi);
         
         virtual void IsKspace(bool is);
         
         virtual void Reduce(double tolerance);
         
         virtual void Restore();

         virtual void Retire();

         virtual void Reaquire();

         virtual void Serialize (std::ostream& os);

         virtual void DeSerialize (std::istream& is);
   };

}

#endif

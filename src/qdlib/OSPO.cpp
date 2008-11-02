#include "OSPO.h"

namespace QDLIB {

   OSPO::OSPO()
   : OPropagator(), _name("OSPO"), Tkin(NULL), Vpot(NULL), expT(NULL), expV(NULL)
   {
   }
   
   
   OSPO::~OSPO()
   {
   }
   
   
   void QDLIB::OSPO::InitExp( )
   {
      if ( imaginary ) {}
      if ( forward ) {}
      
      /* Init expV */
      for (int i=0; i < size(); i++)
      {
	 
      }
   }


   
   Operator * QDLIB::OSPO::NewInstance( )
   {
      OSPO *O = new OSPO();
      
      O->Init(_params);
      
      return O;
   }

   void QDLIB::OSPO::Init( ParamContainer & params )
   {
   }
 
   
   const string & QDLIB::OSPO::Name( )
   {
      return _name;
   }

   WaveFunction * QDLIB::OSPO::operator *( WaveFunction * Psi )
   {
      WaveFunction *ket;
      
      if (expT == NULL || expV == NULL) InitExp();
      
      ket = Psi->NewInstance();
      *ket = Psi;       // Copy
      
      
   }

   
}





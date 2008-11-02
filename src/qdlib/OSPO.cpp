#include "OSPO.h"
#include "Exception.h"

namespace QDLIB {

   OSPO::OSPO()
   : OPropagator(), _name("OSPO"), Tkin(NULL), Vpot(NULL), expT(NULL), expV(NULL),
		 cV(0,0), cT(0,0), _last_time(0)
   {
   }
   
   
   OSPO::~OSPO()
   {
      if (expT != NULL) delete expT;
      if (expV != NULL) delete expV;
   }
   
   /** Init expT */
   void OSPO::InitT( )
   {
      
      for (int i=0; i < Vpot->size(); i++)
      {
	 expT[i] = cexp(Tkin[i] * cT);
      }      
   }
   
   /** Init expV */
   void OSPO::InitV( )
   {
      
      for (int i=0; i < Vpot->size(); i++)
      {
	 expV[i] = cexp(Vpot[i] * cV);
      }
            
   }
      
   void OSPO::InitExp( )
   {
      complex cV = I * clock->Dt()/2;
      dcomplex cT = I * clock->Dt();
      
      if (Tkin == NULL || Vpot == NULL)
	 throw ( EParamProblem("Tkin or Vpot not initialized") );
      
      if (Tkin == NULL || Vpot == NULL)
	 throw ( EParamProblem("Tkin or Vpot not initialized") );

      if (Vpot->size() != Tkin->size())
	 throw ( EParamProblem("Tkin and Vpot differ in size") );
      
//       /* Distingiush between forward & backward in time */
      if ( forward ) {
	 cV = I * clock->Dt()/2;
	 cT = I * clock->Dt();
      } else {
	 cV = -I * clock->Dt()/2;
	 cT = -I * clock->Dt();
      }
      
      
      if ( imaginary ) {   /* imaginary time propagation */
	 cV = I * cV;
	 cT = I * cT;
      }
      
      /* Init storage for exponetials */
      if (expV == NULL) {
	 expV = new cVec(Vpot->size());
      }
      if (expT == NULL) {
	 expT = new cVec(Tkin->size());
      }
      
      InitT();
      InitV();


      
   }


   
   Operator * OSPO::NewInstance( )
   {
      OSPO *O = new OSPO();
      
      O->Init(_params);
      
      return O;
   }

   void QDLIB::OSPO::Init( ParamContainer & params )
   {
   }
 
   
   const string& OSPO::Name( )
   {
      return _name;
   }
   
   dcomplex OSPO::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      throw ( EIncompatible("Sorry the SPO can't calculate a matrix element") );
   }

   double OSPO::Expec(WaveFunction *Psi)
   {
      throw ( EIncompatible("Sorry the SPO can't calculate an expectation value") );
   }
   
   WaveFunction * OSPO::operator *( WaveFunction * Psi )
   {
      WaveFunction *ket;
      WFGridSystem *psi;
      
      if (expT == NULL || expV == NULL) InitExp();
      if (_last_time != clock->TimeStep() && Vpot->isTimeDependent()) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep;
	 InitV();
      }
      
      ket = Psi->NewInstance();
      *ket = Psi;       // Copy
      
      psi = dynamic_cast<WFGridSystem*>(ket);
      
      MultElements(psi, expV);
      
      psi->ToKspace();
      MultElements(psi, expT);
      psi->ToXspace();
      
      MultElements(psi, expV);
      
      return psi;
   }

   WaveFunction * OSPO::operator *=(WaveFunction * Psi)
   {
      WFGridSystem *psi;
      
      if (expT == NULL || expV == NULL) InitExp();
      if (_last_time != clock->TimeStep() && Vpot->isTimeDependent()) /* Re-init Vpot if is time dep.*/
      {  
	 _last_time = clock->TimeStep;
	 InitV();
      }
      
      psi = dynamic_cast<WFGridSystem*>(Psi);
            
      MultElements(psi, expV);
      
      psi->ToKspace();
      MultElements(psi, expT);
      psi->ToXspace();
      
      MultElements(psi, expV);
      
      return psi;
   }
   
   Operator * OSPO::operator =(Operator * O)
   {
      OSPO *org;
      
      org = dynamic_cast<OSPO*>(O);
	    
      *Tkin = org->Tkin;
      *Vpot = org->Vpot;

      *expT = org->expT;
      *expV = org->expV;
      
      *clock = org->clock;
      forward = org->forward;
      imaginary = org->imaginary;
   }
   
   Operator * OSPO::operator *(Operator * O)
   {
      throw ( EIncompatible("Can't apply the SPO to another operator") );
   }

}









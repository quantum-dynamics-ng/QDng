#ifndef QDLIBOSPO_H
#define QDLIBOSPO_H

#include "ODSpace.h"
#include "OPropagator.h"
#include "OGridSum.h"
#include "tools/Exception.h"


namespace QDLIB {

   /**
    * SPO propagator designed for GridSystems.
    * 
    * Relies on a ODSpace operator and a OGridSystem.
    * It can handle a single state or a Multistate System with two levels.
    * 
    * The needs are Tkin, Vpot and optional an off-diagnonal Element Vcoup.
    * 
    * Tkin must be given as single state operator.
    * Vpot is either a single state operator or a two level multistate
    * operator when combined with two level coupling.
    * The state coupling operator must be given as single state variant.
    * Only hermitian Hamiltonians are possible.
    * 
    * Without coupling the SPO is realized as:
    * \f$ e^{-\frac{i}{2} \hat V dt} e^{-i \hat T dt} e^{-\frac{i}{2} \hat V dt}\f$
    * 
    * With coupling it is:
    * \f$ e^{-\frac{i}{2} \hat T dt} e^{-i \hat V dt} X e^{-i \hat V_{coup} dt} X^{-1} e^{-\frac{i}{2} \hat T dt} \f$
    * 
    */
   class OSPO : public OPropagator
   {
      private:
	 string _name;
	 ParamContainer *_needs;
	 
	 ODSpace *_Tkin;
         Transform* _X;             /* Transformation into diags space of Tkin */
	 dVec    *_Tkin_kspace;
	 OGridSystem* _Vpot[2];
	 OGridSystem* _Vcoup;
         OGridSystem* _VcoupI;
	 cVec* _expT;
	 WaveFunction *_expV, *_V1;     /* Dirty hack to get time dependence */
	 WaveFunction *_expVcoup, *_Vcoup1; /* Dirty hack to get time dependence */
         WaveFunction *_expVcoupI;
         
	 WaveFunction *_buf, *_buf2;
	 
         bool _coupling;        /* coupling */
         bool _coupdiag;        /* Coupling on diagonal in Vpot*/
	 
	 dcomplex _cT, _cV;
	 int _last_time;
	 
	 void _InitV();
	 void _InitT();
	 
      public:
	 OSPO();
      
	 ~OSPO();

	 /* Interface implementation, Operator */
	 virtual Operator* NewInstance();
	       
	 virtual void Init(ParamContainer &params);
	 
	 virtual void Init(WaveFunction *Psi);
	 
	 virtual const string& Name();
			
	 virtual void UpdateTime();
	 
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
		  
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual void Apply(WaveFunction *Psi);

	 virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	 
	 /* Interface implementation, Propagator */
	 virtual ParamContainer& TellNeeds();
      
	 virtual void AddNeeds(string &Key, Operator *O);
         
         virtual bool Valid(WaveFunction *Psi);
      };

}

#endif


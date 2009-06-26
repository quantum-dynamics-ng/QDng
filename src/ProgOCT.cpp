#include "ProgOCT.h"

#include "tools/FileSingleDefs.h"
#include "tools/Logger.h"
#include "ChainLoader.h"

namespace QDLIB {

   ProgOCT::ProgOCT(XmlNode &OCTNode) : _octNode(OCTNode), _ContentNodes(NULL),
   _fname(DEFAULT_BASENAME_LASER), _dir(""), _iterations(DEFAULT_ITERATIONS), _convergence(DEFAULT_CONVERGENCE),
   _method(krotov), _coupling(dipole), _ttype(ov), _phase(false),  _ntargets(1), _alpha(1)
   {
      for(int i=0; i < MAX_TARGETS; i++){
	 PsiI[i] = NULL;
	 PsiT[i] = NULL;
      }
   }
   
   
   ProgOCT::~ProgOCT()
   {
      for(int i=0; i < _ntargets; i++){
	 if (PsiI[i] != NULL) delete PsiI[i];
	 if (PsiT[i] != NULL) delete PsiT[i];
      }
   }

   /**
    * Initalize the global parameters.
    */
   void QDLIB::ProgOCT::_InitParams( )
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      ParamContainer attr;
       Logger& log = Logger::InstanceRef();
      
      attr = _octNode.Attributes();
      double dt;
      int steps;
      string s;
      
      /* Init the clock */
      attr.GetValue("dt", dt);
      attr.GetValue("steps", steps);
   
      if (dt == 0)
	 throw ( EParamProblem("Zero time step defined") );
      if (steps == 0)
	 throw ( EParamProblem("Zero number of time step defined") );
   
      clock->Dt(dt);
      clock->Steps(steps);
      
      /* Init OCT output dir */
      if ( attr.isPresent("dir") && _dir.empty() ) {
	 attr.GetValue("dir", _dir);
	 if (_dir[_dir.length()-1] != '/' && ! _dir.empty())
	    _dir += "/";
      }
      
      /* Init propagation file basename */
      if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", _fname);
      }
      
      /* Optimization method to use */
      if (attr.isPresent("method")){
	 attr.GetValue("method", s);
	 if (s == "krotov")
	    _method = krotov;
	 else if (s == "rabitz")
	    _method = rabitz;
	 else
	    throw(EParamProblem("Unknown OCT method", s));

      }
      
      /* Read the shape file */
      if (attr.isPresent("shape")){
	 string shape;
	 attr.GetValue("shape", shape);
	 Laser::FileLaser file = _shape[0].File();
	 
	 file.Suffix(BINARY_O_SUFFIX);
	 file.Name(shape);
	 file >> &(_shape[0]);
      } else
	 throw (EParamProblem("No shape file given"));
      
	 
      /* Number of iterations */
      if (attr.isPresent("iterations")){
	 attr.GetValue("iterations", _iterations);
	 if (_iterations < 1)
	    throw(EParamProblem("Invalid number of iterations", _iterations));
      }
      
      /* Number of iterations */
      if (attr.isPresent("conv")){
	 attr.GetValue("conv", _convergence);
	 if (_iterations < 0)
	    throw(EParamProblem("Invalid convergency criteria", _convergence));
      }

      
      /* Type of coupling to use */
      if (attr.isPresent("coup")){
	 attr.GetValue("coup", s);
	 if (s == "dipole")
	    _coupling = dipole;
	 else
	    throw(EParamProblem("Unsupported coupling specified", s));	 
      }

      /* Target type to use */
      if (attr.isPresent("ttype")){
	 attr.GetValue("ttype", s);
	 if (s == "operator")
	    _ttype = op;
	 else if (s == "overlap")
	    _ttype = ov;
	 else
	    throw(EParamProblem("Unknown OCT target type", s));
      }
      
      /* Check for phase sensitive oct*/
      attr.GetValue("phase", _phase);
       
      /* Target type to use */
      if (attr.isPresent("ntargets")){
	 attr.GetValue("ntargets", _ntargets);
	 if (_ntargets < 1)
	    throw (EParamProblem("Number of targets to small", _ntargets));
	 if (_ntargets > MAX_TARGETS)
	    throw (EParamProblem("Number of targets exceeding MAX_TARGETS", _ntargets));	 
      }

      if (attr.isPresent("alpha")){
	 attr.GetValue("alpha", _alpha);
      }
      
      /* Get alpha */
      if (abs(_alpha) == 0)
	 throw (EParamProblem("Invalid alpha parameter", _alpha));
      
      
      /* Print parameter summary */
      log.Header("OCT parameters", Logger::Section);
    
      log.IndentInc();
      log.cout() << "Number of steps: " <<  clock->Steps() << endl;
      log.cout().precision(2);
      log.cout() << "Time step: " << fixed << clock->Dt() << endl;
      log.cout() << "Overall time: " << fixed << clock->Steps() * clock->Dt() << endl;
      log.cout() << "Basename for wave function output: " << _fname << endl;
      log.cout() << endl;
      
      log.cout() << "OCT - Style: ";
      switch(_method){
	 case krotov:
	    log.cout() << "Krotov";
	 break;
	 case rabitz:
	    log.cout() << "Rabitz";
	 break;
      }
      
      switch(_ttype){
         case ov:
	    log.cout() << " - Overlapp";
	    break;
	 case op:
	    log.cout() << " - Excpectation values";
	    break;
      }
      
      if (_phase)
	 log.cout() << " - phase sensitive" << endl;
      else 
	 log.cout() << endl;
      
      log.cout().precision(3);
      log.cout() << "alpha : " << _alpha << endl;
      log.cout() << "Number of targets: " << _ntargets << endl;
      
      switch (_coupling){
	 case dipole:
	    log.cout() << "Optimize laserfield for dipole coupling" << endl;
      }
      
      log.cout() << "Maximum number of iterations : " << _iterations << endl;
      log.cout().precision(1);
      log.cout() << "Convergence : " << scientific << _convergence << endl;
      
      log.flush();
   }
  
   

   /**
    * Calculate Laserfield for the next time step.
    */
   double ProgOCT::CalcLaserField( WaveFunction** wfi, WaveFunction** wft )
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      
      double res=0;
      dcomplex im(0,0);
      WaveFunction *opwf;
      
      
      
      opwf = wfi[0]->NewInstance();
      
      for (int t=0; t < _ntargets; t++){
	 
	 /* Imaginary term */
	 switch(_coupling){
	    case dipole:
               dynamic_cast<OGridPotential*>(_Coup)->Apply( opwf, wfi[t]);
	       break;
	 }
	 
	 /* Phase sensitive */
	 if (_phase)
	    im = *wft[t] * opwf;
	 else
	    im = (*wfi[t] * wft[t] ) * (*wft[t] * opwf );
		 
	 res += im.imag();
      }
      
      /* Method */
      switch(_method){
	 case krotov:
		  res = _laser[0][clock->TimeStep()] -
			_shape[0][clock->TimeStep()] / (_alpha * double(_ntargets)) * res;
	    break;
	 case rabitz:
		  res = _shape[0][clock->TimeStep()] / (_alpha * double(_ntargets)) * res;
	    break;
      }
      
      return res;
   }
   
   /**
    * Run the OCT-Programm
    */
   void ProgOCT::Run()
   {
      Logger& log = Logger::InstanceRef();
      
      XmlNode *section;
      Operator *_h;
      
      _InitParams( );
      
      /* fetch the ChildNodes */
      _ContentNodes = _octNode.NextChild();
      
      log.Header( "QM Initialization", Logger::Section);
      log.Header( "Propagator: ", Logger::SubSection);
      
      
      /* Load & Init the propagator */
      section = _ContentNodes->FindNode( "propagator" );
      if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );
      
      _U = ChainLoader::LoadPropagator( section, &_h );
      delete section;
      
      
      /* Load Initial Wavefunctions */
      log.Header( "Initial wave functions", Logger::SubSection);
      char num[3];
      for (int i=0; i< _ntargets; i++){
	 snprintf(num, 3, "%d", i);
	 section = _ContentNodes->FindNode( "wfi"+string(num) );
	 if (section == NULL)
	    throw ( EParamProblem ("Missing initial wavefunction", i) );
	 PsiI[i] = ChainLoader::LoadWaveFunctionChain( section );
	 delete section;
      }
      log.cout() << endl;
      
      /* Load Target Wavefunctions */
      log.Header( "Target wave functions", Logger::SubSection);
      for (int i=0; i< _ntargets; i++){
	 snprintf(num, 3, "%d", i);
	 section = _ContentNodes->FindNode( "wft"+string(num) );
	 if (section == NULL)
	    throw ( EParamProblem ("Missing target wavefunction", i) );
	 PsiT[i] = ChainLoader::LoadWaveFunctionChain( section );
	 delete section;
      }
      
      log.cout() << endl;
      
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
           
      /* Make sure our hamiltonian is initalized */
      _h->Clock( clock );
      _h->Init(PsiI[0]);
      log.coutdbg() << "Initial engergy: " << _h->Expec(PsiI[0]) << endl;
      
      /* Copy, since the propagator will propably scale it/modify etc. */
      _H = _h->NewInstance();
      *_H = _h; 
      _H->Clock( clock );
            
      /* Let the Propagator do it's initalisation */
      _U->Clock( clock );
      _H->UpdateTime();
      _U->Init(PsiI[0]);

      /* Check & reference the coupling operator */
      bool coupling_ok = false;
      switch(_coupling){
	 case dipole:
	    OGridDipole *CoupOGridDipole;
	    CoupOGridDipole = dynamic_cast<OGridDipole*>(FindOperatorType<OGridDipole>(_h));
	    if (CoupOGridDipole != NULL){
	       _laser[0] = CoupOGridDipole->GetLaser();
	       _Coup = (Operator*) CoupOGridDipole;
	       coupling_ok=true;
	    }
	    
	    break;
      }
      
      if (!coupling_ok)
	 throw ( EParamProblem ("No suitable coupling operator found in hamiltonian") );
      
      /* Objects for propagation */
      WaveFunction* phii[MAX_TARGETS];
      WaveFunction* phit[MAX_TARGETS];
      
      for (int i=0; i < _ntargets; i++){
	 phii[i] = PsiI[i]->NewInstance();
	 phit[i] = PsiT[i]->NewInstance();
      }

      
      /* Iteration loop */
      int i=0;
      double deltaTarget=1;
            
      while (i < _iterations && deltaTarget > _convergence){
	 /* Init with fresh wfs */
	 for (int t=0; t < _ntargets; t++){
	    *(phii[t]) = PsiI[t];
	    *(phit[t]) = PsiT[t];
	 }

	 /* Backpropagation of targets */
	 /** \todo prepare operator for fast forward/backward switching */
	 clock->End();
	 for (int s=0; s < clock->Steps(); s++){
	    for (int t=0; t < _ntargets; t++){
	       _U->Apply(phit[t]);
	    }
	    *(clock)--;
	 }

	 /* Forward Propagation */
	 clock->Begin();
	 for (int s=0; s < clock->Steps(); s++){
	    _laser[0][clock->TimeStep()] = CalcLaserField(phii,phit);
	    for (int t=0; t < _ntargets; t++){
	       _U->Apply(phit[t]);
	       _U->Apply(phii[t]);
	    }
	    *(clock)++;
	 }
	 
	 /* Write Laserfield */
	 i++; 
      }

      /* Remove tmp WFs */
      for (int i=0; i < _ntargets; i++){
	 delete phii[i];
	 delete phit[i];
      }

   }



} /* namespace QDLIB */

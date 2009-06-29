#include "ProgOCT.h"

#include "tools/FileSingleDefs.h"
#include "tools/Logger.h"
#include "ChainLoader.h"

namespace QDLIB {

   ProgOCT::ProgOCT(XmlNode &OCTNode) : _octNode(OCTNode), _ContentNodes(NULL),
   _fname(DEFAULT_BASENAME_LASER), _dir(""), _iterations(DEFAULT_ITERATIONS), _convergence(DEFAULT_CONVERGENCE),
   _writel(false), _method(krotov), _coupling(dipole), _ttype(ov), _phase(false),  _ntargets(1), _alpha(1)
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

      /* Write laserfield in every step */
      if(attr.isPresent("writel"))
	 attr.GetValue("writel", _writel);
      
      
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
      
      double scaling;
      
      opwf = wfi[0]->NewInstance();
      
      for (int t=0; t < _ntargets; t++){
	 
	 /* Imaginary term */
	 switch(_coupling){
	    case dipole:
	       scaling = _Coup->Scaling();
	       _Coup->Scale( 1/scaling );
	       _Coup->Apply( opwf, wfi[t]);
	       _Coup->Scale( scaling );
	       break;
	 }
	 
	 /* Phase sensitive */
	 if (_phase)
	    im = *wft[t] * opwf;
	 else
	    im = (*(wfi[t]) * wft[t] ) * (*(wft[t]) * opwf );
		 
	 res += im.imag();
      }
      
      /* Method */
      switch(_method){
	 case krotov:
		  res = _laserf[0][clock->TimeStep()] -
			_shape[0][clock->TimeStep()] / (_alpha * double(_ntargets)) * res;
	    break;
	 case rabitz:
		  res = _shape[0][clock->TimeStep()] / (_alpha * double(_ntargets)) * res;
	    break;
      }
      
      return res;
   }
   

   /**
    * Write Norm and Yield.
    * 
    * \return Target overlap
    */
   double ProgOCT::Report(WaveFunction **wfi, int iteration)
   {
      Logger& log = Logger::InstanceRef();
      dcomplex overlap;
      dcomplex ov_sum(0,0);
      
      log.cout() << iteration << "\t";
      for (int t=0; t < _ntargets; t++){
	 log.cout().precision(6);
	 log.cout() << wfi[t]->Norm() << "\t";
	 overlap = *wfi[t] * PsiT[t];
	 ov_sum += cabs(overlap);
	 log.cout() << cabs(overlap) << "\t";
	 if (_phase){
	    log.cout() << overlap << "\t";
	    ov_sum += overlap;
	 } else
	    ov_sum._real += cabs(overlap);
      }
      log.cout() << _laserf[0].PulseEnergy() << endl;
      
      return cabs(ov_sum);
   }
   
   
   /**
    * Run the OCT-Programm.
    */
   void ProgOCT::Run()
   {
      Logger& log = Logger::InstanceRef();
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      XmlNode *section;
      Operator *_hf, *_hb;
      
      _InitParams( );
      
      /* fetch the ChildNodes */
      _ContentNodes = _octNode.NextChild();
      
      log.Header( "QM Initialization", Logger::Section);
      log.Header( "Propagator: ", Logger::SubSection);
      
      
      /* Load & Init the propagator */
      section = _ContentNodes->FindNode( "propagator" );
      if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );
      
      _Uf = ChainLoader::LoadPropagator( section, &_hf );
      _Ub = ChainLoader::LoadPropagator( section, &_hb );
      delete section;
      
      
      /* Load Initial Wavefunctions */
      log.Header( "Initial wave functions", Logger::SubSection);
      log.IndentInc();
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
      log.IndentDec();
      
      /* Load Target Wavefunctions */
      log.Header( "Target wave functions", Logger::SubSection);
      log.IndentInc();
      for (int i=0; i< _ntargets; i++){
	 snprintf(num, 3, "%d", i);
	 section = _ContentNodes->FindNode( "wft"+string(num) );
	 if (section == NULL)
	    throw ( EParamProblem ("Missing target wavefunction", i) );
	 PsiT[i] = ChainLoader::LoadWaveFunctionChain( section );
	 delete section;
      }
      log.cout() << endl;
      log.IndentDec();
      
           
      /* Make sure our hamiltonian is initalized */
      _hf->Clock( clock );
      _hf->Init(PsiI[0]);
      _hb->Clock( clock );
      _hb->Init(PsiI[0]);      
      log.coutdbg() << "Initial engergy: " << _hf->Expec(PsiI[0]) << endl;
      
      /* Copy, since the propagator will propably scale it/modify etc. */
      _H = _hf->NewInstance();
      *_H = _hf; 
      _H->Clock( clock );
            

      /* Check & reference the coupling operator */
      bool coupling_ok = false;
      switch(_coupling){
	 case dipole:
	    OGridDipole *CoupOGridDipole;
	    _Coup = FindOperatorType<OGridDipole>(_hf, &CoupOGridDipole);
	    if (CoupOGridDipole != NULL){
	       _laserf[0] = CoupOGridDipole->GetLaser();
	       /* Get the backward laser */
	       FindOperatorType<OGridDipole>(_hb, &CoupOGridDipole);
	       _laserb[0] = CoupOGridDipole->GetLaser();
	       coupling_ok=true;
	    }
	    break;
      }
      
      if (!coupling_ok)
	 throw ( EParamProblem ("No suitable coupling operator found in hamiltonian") );
      
      _Coup->Init( PsiI[0] );
      
      /* Let the Propagator do it's initalisation */
      _Uf->Clock( clock );
      _Ub->Clock( clock );
      _H->UpdateTime();
      _Uf->Init(PsiI[0]);
      
      _Ub->Backward();
      _Ub->Init(PsiI[0]);
      
      /* Objects for propagation */
      WaveFunction* phii[MAX_TARGETS];
      WaveFunction* phit[MAX_TARGETS];
      
      for (int i=0; i < _ntargets; i++){
	 phii[i] = PsiI[i]->NewInstance();
	 phit[i] = PsiT[i]->NewInstance();
      }

      /* Write Header for iteration table */
      log.SetIndent(0);
      log.cout() << "OCT Iterations:" << endl;
      log.cout() << "iteration\t";
      for (int t=0; t < _ntargets; t++){
	 log.cout() << "Norm " << t << "\t";
	 if (_phase)
	    log.cout() << "Overlapp " << t << "\t";
      }
      log.cout() << "Pulse Energy" << endl;
      log.flush();

      /* Prepare laserfile writer */
      Laser::FileLaser file = _laserf[0].File();
      file.Suffix(BINARY_O_SUFFIX);
      
      /* Iteration loop */
      int i=0;
      double deltaTargetOld=1;
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
	       _Ub->Apply(phit[t]);
	    }
	    --(*clock);
	 }

	 /* Forward Propagation */
	 clock->Begin();
	 for (int s=0; s < clock->Steps(); s++){
	    _laserf[0][s] = CalcLaserField(phii,phit);
	    for (int t=0; t < _ntargets; t++){
	       _Uf->Apply(phit[t]);
	       _Uf->Apply(phii[t]);
	    }
	    ++(*clock);
	 }
	 
	 /* Write Report & Calculate change */
	 deltaTarget = abs(deltaTargetOld - Report(phii, i));
	 
	 /* Write laserfields */
	 if(_writel){
	    char num[1024];
	    snprintf(num, 1024,"%s/%s%d", _dir.c_str(), _fname.c_str(), i);
	    file.Name(string(num));
	    file << &(_laserf[0]);
	 }
	 
	 /* Exchange laserfields */
	 _laserf[0].swap(_laserb[0]);
	 
	 log.flush();
	 i++; 
      } /* while (i < _iterations && deltaTarget > _convergence) */

      /* Write the final laser */
      file.Name(_dir+"/"+_fname);
      file << &(_laserb[0]);
      
      /* Remove tmp WFs */
      for (int i=0; i < _ntargets; i++){
	 delete phii[i];
	 delete phit[i];
      }

   }



} /* namespace QDLIB */


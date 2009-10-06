#include "ProgOCT.h"

#include "tools/FileSingleDefs.h"
#include "tools/Logger.h"
#include "ChainLoader.h"
#include "qdlib/Butterworth.h"

namespace QDLIB {

   ProgOCT::ProgOCT(XmlNode &OCTNode) : _octNode(OCTNode), _ContentNodes(NULL),
   _fname(DEFAULT_BASENAME_LASER), _dir(""), _iterations(DEFAULT_ITERATIONS), _convergence(DEFAULT_CONVERGENCE),
          _writel(false),_membuf(true), _method(krotov), _coupling(dipole), _ttype(ov), _phase(false),
                  _ntargets(1), _alpha(1), _Otarget(NULL), _opwf(NULL), _membuf_init(false)
   {
      for(int i=0; i < MAX_TARGETS; i++){
	 PsiI[i] = NULL;
	 PsiT[i] = NULL;
      }
   }
   
   
   ProgOCT::~ProgOCT()
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      for(int i=0; i < _ntargets; i++){
	 if (PsiI[i] != NULL) delete PsiI[i];
	 if (PsiT[i] != NULL) delete PsiT[i];
      }
      if (_Uf != NULL) delete _Uf;
      if (_Ub != NULL) delete _Ub;
      if (_opwf != NULL) delete _opwf;
      
      if (_membuf && _membuf_init){
         for (int s=0; s < clock->Steps(); s++)
            for (int t=0; t < _ntargets; t++)
               delete _memwfbuf[s][t];
      }
      if (_Otarget != NULL) delete _Otarget;
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
         else if (s == "rabitzfb")
            _method = rabitzfb;
         else if (s == "freq")
            _method = freq;
	 else
	    throw(EParamProblem("Unknown OCT method ", s));

      }
      
      /* Read the shape file */
      if (attr.isPresent("shape")){
	 string shape;
	 attr.GetValue("shape", shape);
	 Laser::FileLaser file = _shape[0].File();
	 
	 file.Suffix(BINARY_O_SUFFIX);
	 file.Name(shape);
	 file >> &(_shape[0]);
	 _shape[0].Clock(clock);
      } else
	 throw (EParamProblem("No shape file given"));
      
      /* Look for freq. shape params */
      if (_method == freq){
         if ( attr.isPresent("freq") ){ /* Load mask from file */
            string frqmask;
            attr.GetValue("freq", frqmask);
            Laser::FileLaser file = _gamma[0].File();
            
            file.Suffix(BINARY_O_SUFFIX);
            file.Name(frqmask);
            file >> &(_frqmask[0]);
            if (_frqmask[0].Dt() != dt || _frqmask[0].Nt() != steps/2+1)
               throw ( EIncompatible("Frequency mask doesn't match dt/steps") );
         } else if ( attr.isPresent("freql") || attr.isPresent("freqh") ) { /* Construct mask from params */
            /* Set params */
            _frqmask[0].Nt(steps/2+1);
            _frqmask[0].Dt(dt);
            ((dVec&) (_frqmask[0])) = 1.0; /* initialize with ones */
            
            double cutoff;
            dVec mask(steps/2+1);
            /* Lower cuttoff => Highpass */
            if  (attr.isPresent("freql")) {
               attr.GetValue("freql", cutoff);
               cout << "Lower " << cutoff << endl;
               cutoff = abs(cutoff) * (steps-1)*dt/(2*M_PI); /* convert freq. => grid point */
               cout << cutoff << endl;
               if (cutoff > steps/2+1)
                  throw ( EParamProblem("Lower frequency bound larger than Nyquist frequency") );
               Butterworth<dVec>::Highpass(mask, cutoff, 100);
               for (int i=0; i < steps/2+1; i++){
                  //cout << (_frqmask[0])[i] << endl;
                  (_frqmask[0])[i] *= mask[i] ;
                  //cout << (_frqmask[0])[i] << endl;
                  //cout << mask[i] << endl;
               }
            }
            /* Higher cuttoff => Lowpass */
            if  (attr.isPresent("freqh")) {
               attr.GetValue("freqh", cutoff);
               cout << "Higher " << cutoff << endl;
               cutoff = abs(cutoff) * (steps-1)*dt/(2*M_PI); /* convert freq. => grid point */
               cout << cutoff << endl;
               if (cutoff > steps/2+1)
                  throw ( EParamProblem("Upper frequency bound larger than Nyquist frequency") );
               Butterworth<dVec>::Lowpass(mask, cutoff, 100);
               for (int i=0; i < steps/2+1; i++){
                  (_frqmask[0])[i] *= mask[i];
                  cout << (_frqmask[0])[i] << endl;
               }
            }            
         } else
            throw( EParamProblem("Missing frequency shaping parameters (freq, freql or freqh)") );
           
      } /* if (_method == freq) */
	 
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
      
      /* Use memory buffer for backpropagation */
      attr.GetValue("membuf", _membuf, true);
      
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
         case freq:
            log.cout() << "Krotov - frequency shaping";
            break;
	 case krotov:
	    log.cout() << "Krotov";
	 break;
	 case rabitz:
	    log.cout() << "Rabitz";
	 break;
         case rabitzfb:
            log.cout() << "Rabitz Feedback";
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
    * Calc. \f$ \Im ( A * \langle \phi_i | \mu | \phi_t \rangle) \f$
    */
   double ProgOCT::CalcCorr(WaveFunction ** wfi, WaveFunction ** wft)
   {
      double res=0;
      dcomplex im(0,0);
      
      if( _opwf == NULL)
         _opwf = wfi[0]->NewInstance();
      
      for (int t=0; t < _ntargets; t++){
	 
         /* Imaginary term */
         switch(_coupling){
            case dipole:
               _Coup->Apply( _opwf, wfi[t]);
               break;
         }
	 
         /* Phase sensitive */
         if (_phase)
            im = *wft[t] * _opwf;
         else /** \todo clarify why it only works this way! should be wfi * wft !!! */
            im = (*(wft[t]) * wfi[t] ) * (*(wft[t]) * _opwf );

         res += im.imag();
      }
      return res;
   }

   
   /**
    * Calculate Laserfield for the next time step.
    */
   double ProgOCT::CalcLaserField( WaveFunction** wfi, WaveFunction** wft )
   {
      double res;
      
      res = CalcCorr(wfi, wft);

      /* Method */
      switch(_method){
	 case krotov: /* Add New Laser to previous field */
		  res = _laserb[0]->Get() +
			_shape[0].Get() / (_alpha * double(_ntargets)) * res;
	    break;
         case freq:  /* like krotov - but subtract gamma */
            res = _laserb[0]->Get() +
                  _shape[0].Get() / (_alpha * double(_ntargets)) * (res-_gamma[0].Get());
            break;
	 case rabitz:
         case rabitzfb:
		  res = -1* _shape[0].Get() / (_alpha * double(_ntargets)) * res;
	    break;
      }
      
      return res;
   }
   

   /**
    * Write Norm and Yield.
    * 
    * \return Target overlap
    */
   double ProgOCT::Report(WaveFunction **wfi, WaveFunction **wft, int iteration)
   {
      Logger& log = Logger::InstanceRef();
      dcomplex overlap;
      dcomplex opval;
      dcomplex ov_sum(0,0);
      
      /* Write Header for iteration table */
      if (iteration == 0){
         log.SetIndent(0);
         log.Header("OCT Iterations:", Logger::SubSection);
         
         log.cout() << "iteration\t";
         for (int t=0; t < _ntargets; t++){
            log.cout() << "Norm_" << t << "\t\t";
            if (_ttype == ov) log.cout() << "Overlapp_" << t << "\t\t";
            else log.cout() << "Operator_" << t << "\t";
            if (_phase && _ttype == ov)
               log.cout() << "Phase_" << t << "\t\t";
         }
         log.cout() << "Pulse Energy" << endl;
      }
      
      if( _opwf == NULL)
         _opwf = wfi[0]->NewInstance();
      
      log.cout() << iteration << "\t\t";
      for (int t=0; t < _ntargets; t++){
         log.cout() << fixed;
	 log.cout().precision(8);
	 log.cout() << wfi[t]->Norm() << "\t";
	 
         if (_ttype == ov) { /* Print overlapp */
            overlap = *wfi[t] * wft[t];
            ov_sum += cabs(overlap);
            log.cout() << cabs(overlap) << "\t";
            if (_phase){
               log.cout() << overlap << "\t";
               ov_sum += overlap;
            } else
               ov_sum._real += cabs(overlap);
        } else { /* Print operator */
            _Otarget->Apply(_opwf,wft[t]);
            opval = *wft[t] * _opwf;
            ov_sum += cabs(opval);
            log.cout() << opval.real() << "\t";
         }
         
      }
      log.cout() << _laserb[0]->PulseEnergy() << endl;
      log.flush();
      return cabs(ov_sum);
   }
   
   
   /**
    * Propagate Forward from t= 0 => T all targets.
    * 
    * Using the _Uf and the corresponding Laserfield _laserf
    * \param membuf Save the propagation in the memory buffer
    */
   void ProgOCT::PropagateForward(WaveFunction ** wf, bool membuf)
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      
      clock->Begin();
      for (int s=0; s < clock->Steps(); s++){
         for (int t=0; t < _ntargets; t++){
            _Uf->Apply(wf[t]);
            if (membuf)
               *(_memwfbuf[clock->Steps()-s-1][t]) = wf[t];
         }
         ++(*clock);
      }
   }

   /**
    * Propagate Backward from t= T => 0 all targets.
    * 
    * 
    * Using the _Uf and the corresponding Laserfield _laserb
    * \param membuf Save the propagation in the memory buffer
    */
   void ProgOCT::PropagateBackward(WaveFunction **wf, bool membuf)
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      
      clock->End();
      for (int s=0; s < clock->Steps(); s++){
         for (int t=0; t < _ntargets; t++){
            _Ub->Apply(wf[t]);
            if (membuf)
               *(_memwfbuf[clock->Steps()-s-1][t]) = wf[t];
         }
         --(*clock);
      }
   }
   
   
   /**
    *Synchronize overlap targets.
    * 
    */
   void QDLIB::ProgOCT::SyncTargetOverlap(WaveFunction ** phii, WaveFunction ** phit, int step)
   {
      
      /* Backpropagation of targets */
      PropagateBackward(phit, _membuf);
            
      /* Write Report & Calculate change */
      Report(phii,phit, step-1);
      
   }

   /**
    * Synchronize operator targets.
    *
    */   
   void ProgOCT::SyncTargetOperator(WaveFunction ** phii, WaveFunction ** phit, int step)
   {
      
      /* Exchange laserfields */
      _laserb[0]->swap(*(_laserf[0]));
      /* Propagate forward */
      PropagateForward(phit, false);
      /* Write Report & Calculate change */
      Report(phii,phit, step-1);
      
      /* Exchange laserfields */
      _laserb[0]->swap(*(_laserf[0]));
      
      /* Apply */
      for (int t=0; t < _ntargets; t++)
         _Otarget->Apply(phit[t]);
      
      /* Propagate Backward */
      PropagateBackward(phit, _membuf);
   }

   /**
    * Iterate Krotov, Rabitz
    */
   void ProgOCT::Iterate(WaveFunction ** phii, WaveFunction ** phit, int step)
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      
      if (_ttype == ov)
         SyncTargetOverlap(phii, phit, step);
      else
         SyncTargetOperator(phii, phit, step);
      
      clock->Begin();
      for (int s=0; s < clock->Steps(); s++){
            
         /* Target with old field */
         if (! _membuf){
            for (int t=0; t < _ntargets; t++){
               _Uf->Apply(phit[t]);
            }
         }
         
         /* Get new field */
         if (_membuf)
            _laserf[0]->Set(CalcLaserField(phii,_memwfbuf[s]));
         else
            _laserf[0]->Set(CalcLaserField(phii,phit));
            
         /* Propagate initial with new field */
         for (int t=0; t < _ntargets; t++){
            _Uf->Apply(phii[t]);
         }
         ++(*clock);
      }
   }
   
   /**
    * Iteration Step for the Rabitz feedback method.
    */
   void QDLIB::ProgOCT::IterateRabitzFB(WaveFunction ** phii, WaveFunction ** phit, int step)
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      
      
      if (step==1){
         PropagateForward(phii, _membuf);
      }
      
      /* Create target */
      if (_ttype == op)
         for (int t=0; t < _ntargets; t++)
            _Otarget->Apply(phit[t], phii[t]);
      
      Report(phii, phit, step);
      
      /* Propagate Target Backward with new field */
      clock->End();
      for (int s=0; s < clock->Steps(); s++){
         /* Calc new laser field */
         if (_membuf)
            _laserb[0]->Set(CalcLaserField(_memwfbuf[s], phit));
         else {
            for (int t=0; t < _ntargets; t++)
               _Ub->Apply(phii[t]);
            _laserb[0]->Set(CalcLaserField(phii,phit));
         }
         
         /* Do one step back */
         for (int t=0; t < _ntargets; t++)
            _Ub->Apply(phit[t]);
         
         /* Save back steps */
         if (_membuf)
            _memwfbuf[clock->TimeStep()] = phit;
         
         --(*clock);
      }
      
      /* Propagate forward initial with new field */
      clock->Begin();
      for (int s=0; s < clock->Steps(); s++){
         /* Calc new laser field */
         if (_membuf)
            _laserf[0]->Set(CalcLaserField(phii,_memwfbuf[s]));
         else {
            for (int t=0; t < _ntargets; t++)
               _Uf->Apply(phit[t]);
            _laserf[0]->Set(CalcLaserField(phii,phit));
         }
         
         /* Do one step forward */
         for (int t=0; t < _ntargets; t++)
            _Uf->Apply(phii[t]);
         
         /* Save forward steps */
         if (_membuf)
            _memwfbuf[clock->TimeStep()] = phii;
         
         ++(*clock);
      }
      

      
   }

   /**
    * Iteration Step for Krotov frequency shaping method.
    */
   void ProgOCT::IterateFreq(WaveFunction ** phii, WaveFunction ** phit, int step)
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      
      cVec *freqbuf;
      
      /* Sync targets */
      if (_ttype == ov)
         SyncTargetOverlap(phii, phit, step);
      else
         SyncTargetOperator(phii, phit, step);
      
      /* Shape old Laser in the frequency domain & write result to new laser */
      _laserf[0]->FastCopy(*(_laserb[0]));
      freqbuf = _laserf[0]->Spectrum();
      
      for (int s=0; s < clock->Steps()/2+1; s++){ /* apply mask */ 
         //cout << (*freqbuf)[s] << "\n";
         //cout << (_frqmask[0])[s] << "\n";
         (*freqbuf)[s] *= (_frqmask[0])[s] / clock->Steps(); /** \todo Replace with low level method */
         //cout << (*freqbuf)[s] << "\n";
      }
      
      _laserf[0]->ToTimeDomain();

      //cout << *(_laserf[0]);
      /* Determine correction field gamma (propagation with shaped lasers)*/
      clock->Begin();
      for (int s=0; s < clock->Steps(); s++){
         for (int t=0; t < _ntargets; t++){
            _Uf->Apply(phit[t]);
            _Uf->Apply(phii[t]);
         }
         _gamma[0].Set( CalcCorr(phii,phit) );
         //cout << _gamma[0].Get() << "\n";
         ++(*clock);
      }

      /* Apply Bandstop filter */
      freqbuf = _gamma[0].Spectrum();
      for (int s=0; s < clock->Steps()/2+1; s++) /* apply mask */ 
         (*freqbuf)[s] *= (1-(_frqmask[0])[s])/ clock->Steps();
      
      _gamma[0].ToTimeDomain();
      
      /** \todo targets need resync without membuf */
      if (! _membuf){ /* refresh targets */
         for (int t=0; t < _ntargets; t++){
            *(phit[t]) = PsiT[t];
         }
      }
      
      /* Refresh intial */
      for (int t=0; t < _ntargets; t++){
         *(phii[t]) = PsiI[t];
      }

      /* Replace forward to old field */
      //_laserb[0]->swap(*(_laserf[0]));
      
      clock->Begin();
      for (int s=0; s < clock->Steps(); s++){
            
         /* Target with old field */
         if (! _membuf){
            for (int t=0; t < _ntargets; t++){
               _Uf->Apply(phit[t]);
            }
         }
         
         /* Get new field */
         if (_membuf)
            _laserf[0]->Set(CalcLaserField(phii,_memwfbuf[s]));
         else
            _laserf[0]->Set(CalcLaserField(phii,phit));
            
         /* Propagate initial with new field */
         for (int t=0; t < _ntargets; t++){
            _Uf->Apply(phii[t]);
         }
         ++(*clock);
      }
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
      
      if(_ttype == ov){
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
      } else if (_ttype == op) {
         log.Header( "Target operator", Logger::SubSection);
         log.IndentInc();
         section = _ContentNodes->FindNode( "target");
         if (section == NULL)
            throw ( EParamProblem ("Missing target operator") );
         _Otarget = ChainLoader::LoadOperatorChain( section );
         _Otarget->Clock(clock);
         _Otarget->Init(PsiI[0]);
      }
      
      log.IndentDec();
      
           
      /* Make sure our hamiltonian is initalized */
      _hf->Clock( clock );
      _hf->Init(PsiI[0]);
      _hb->Clock( clock );
      _hb->Init(PsiI[0]);
      log.cout() << "Initial engergy: " << _hf->Expec(PsiI[0]) << endl;
      if (_ttype == ov){
         log.cout() << "Initial engergy: " << _hb->Expec(PsiT[0]) << endl;
         log.cout() << "Initial Overlapp: " << *(PsiT[0]) * (PsiI[0])<< endl;
      }
      log.flush();
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
         default:
            throw(EIncompatible("Unknown coupling type") );
      }
      
      if (!coupling_ok)
         throw ( EParamProblem ("Given type of coupling operator not found in hamiltonian"));
      
      section = _ContentNodes->FindNode( "coupling" );
      if (section == NULL)
         throw( EParamProblem ("No labeled coupling operator found in hamiltonian") );
      
      
      log.Header( "Coupling operator", Logger::SubSection);
      log.IndentInc();
      _Coup = ChainLoader::LoadOperatorChain(section);
      if (_Coup == NULL)
         throw( EParamProblem ("No coupling operator") );
      
      log.IndentDec();
      
      switch(_coupling){
         case dipole:
            OGridPotential* test;
            FindOperatorType<OGridPotential>(_Coup, &test);
            if (test == NULL)
               throw( EParamProblem ("Invalid coupling operator") );
            break;
      }
      
      /* Unset unity in Multistate => Otherwise Operator will be wrong */
      OMultistate* ms;
      ms = dynamic_cast<OMultistate*>(_Coup);
      if (ms != NULL){
         log.coutdbg() << "Switching of unity\n";
         ms->Unity(false);
      }
      
      _Coup->Clock(clock);
      delete section;

       _Coup->Init( PsiI[0] );
      
      /* Let the Propagator do it's initalisation */
      _Uf->Clock( clock );
      _Ub->Clock( clock );
      _H->UpdateTime();
      _Uf->Init(PsiI[0]);
      
      _Ub->Backward();
      _Ub->Init(PsiI[0]);
      
      /* Report what the propagator has chosen */
      ParamContainer Upm;
    
      Upm = _Uf->Params();
      log.cout() << "Forward Propagators init parameters:\n\n" << Upm << endl;
      Upm = _Ub->Params();
      log.cout() << "Backward Propagators init parameters:\n\n" << Upm << endl;

      
      /* Objects for propagation */
      WaveFunction* phii[MAX_TARGETS];
      WaveFunction* phit[MAX_TARGETS];
      
      /* Init propa buffers */
      for (int i=0; i < _ntargets; i++){
	 phii[i] = PsiI[i]->NewInstance();
         if (_ttype == ov) 
            phit[i] = PsiT[i]->NewInstance();
         else
            phit[i] = PsiI[i]->NewInstance();
      }


      /* Prepare laserfile writer */
      Laser::FileLaser file = _laserf[0]->File();
      file.Suffix(BINARY_O_SUFFIX);
      file.Name(_dir+_fname);
      if (_writel){
	 file.ActivateSequence();
	 file << _laserf[0];
      }
      
      /* debuging */
      FileWF wfile;
      wfile.Name(_dir+string("wfb"));
      wfile.ActivateSequence();
      


      /* Initialize Buffer for backpropagation */
      if (_membuf){
         _memwfbuf = new WaveFunction**[clock->Steps()];
         for (int s=0; s < clock->Steps(); s++){
            _memwfbuf[s] = new WaveFunction*[_ntargets];
            for (int t=0; t < _ntargets; t++)
               _memwfbuf[s][t] = PsiI[t]->NewInstance();
         }
      }
      
      /* Initialize gamma for freq. shape */
      if (_method == freq){
         _gamma[0].Nt(clock->Steps());
         _gamma[0].Dt(clock->Dt());
         _gamma[0].Clock(clock);
      }
      
      /* Iteration loop */
      int i=1;
      while (i <= _iterations){
	 /* Init with fresh wfs */
         if ((_method == rabitzfb && i==1) || _method != rabitzfb)
	 for (int t=0; t < _ntargets; t++){
	    *(phii[t]) = PsiI[t];
            if (_ttype == ov)
               *(phit[t]) = PsiT[t];
            else
               *(phit[t]) = PsiI[t];
	 }

         /* The iteration step itself depends on the method */
	 if (_method == rabitzfb)
            IterateRabitzFB(phii, phit, i);
         else if (_method == freq)
            IterateFreq(phii, phit, i);
         else {
            Iterate(phii, phit,i);
         }


	 
	 /* Write laserfields */
	 if(_writel){
	    //file << _laserb[0];
	    file << _laserf[0];
	 }
	 
	 /* Exchange laserfields (New<->Old) */
	 _laserb[0]->swap(*(_laserf[0]));
	 
	 log.flush();
	 i++;
//          if (deltaTarget <= _convergence){
//             log.cout()  << "\nConvergence reached\n";
//             break;
//          }
      } /* while (i < _iterations && deltaTarget > _convergence) */

      /* Write the final laser */
      file.StopSequence();
      file << _laserb[0];
      
      /* Remove tmp WFs */
      for (int i=0; i < _ntargets; i++){
         
	 delete phii[i];
	 delete phit[i];
      }

   }



} /* namespace QDLIB */








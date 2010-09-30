#include "ProgEigen.h"

#include "tools/Logger.h"

#include "tools/QDGlobalClock.h"
#include "tools/helpers.h"
#include "tools/fstools.h"
#include "ChainLoader.h"

#include "tools/FileSingleDefs.h"
#include "tools/QDGlobalClock.h"

#include "ChainLoader.h"
#include "Reporter.h"
#include "qdlib/Conversion.h"
#include "qdlib/WFBuffer.h"
#include "tools/PeakFinder.h"
#include "qdlib/WindowFuncs.h"
#include "fft/fft.h"

#include "linalg/LapackDiag.h"

namespace QDLIB
{

   ProgEigen::ProgEigen(XmlNode &EigenNode) : _EigenNode(EigenNode),
                            _U(NULL), _H(NULL), _method(imag), _Nef(DEFAULT_NUMBER_EFS),
			    _convergence(DEFAULT_CONVERGENCE_EF_RAW),
			    _MaxSteps(DEFAULT_MAXSTEPS),
                                      _fname(DEFAULT_EF_BASE_NAME), _ename(DEFAULT_EF_ENERGY_NAME), _spectrum(DEFAULT_EF_SPECTRUM_NAME),
                                      _diag(true), _start(0), _tol(1), _win(true)
   {
      _P = new OProjection();
      CollectorOp::Instance()->Register(_P);
   }


   ProgEigen::~ProgEigen()
   {
      /* remove the clock */
      QDGlobalClock::Destroy();
      DELETE_OP(_P);
      GlobalOpList::Instance().Clear();
   }

   /**
    * Write the energies to a file
    */
   void ProgEigen::WriteEnergyFile( )
   {
      Logger& log = Logger::InstanceRef();
      /* Write energy dat */
      ofstream enfile;
      string sn;
      sn = _dir + _ename;
      try {
	 enfile.open(sn.c_str());
	 if (!enfile.is_open()) throw;
	
	 for (int i=0; i < _Nef; i++){

            if (_diag) { /* Use diagonalized energies */
               enfile.precision(8);
               enfile << i << "\t" << fixed << _Energies_diag[i];
               enfile.precision(2);
               enfile << "\t" << _Energies_diag[i]*AU2WAVENUMBERS;
               enfile << "\t" << (_Energies_diag[i]-_Energies_diag[0])*AU2WAVENUMBERS;
               if (i>0)
                  enfile << "\t" << (_Energies_diag[i]-_Energies_diag[i-1])*AU2WAVENUMBERS;
            } else { /* Raw energies  */
               enfile.precision(8);
               enfile << i << "\t" << fixed << _Energies_raw[i];
               enfile.precision(2);
               enfile << "\t" << _Energies_raw[i]*AU2WAVENUMBERS;
               enfile << "\t" << (_Energies_raw[i]-_Energies_raw[0])*AU2WAVENUMBERS;
               if (i>0)
                  enfile << "\t" << (_Energies_raw[i]-_Energies_raw[i-1])*AU2WAVENUMBERS;
            }
	    enfile << endl;
	 }
	 enfile.close();
      } catch (...){
	 log.cout() << "!!! Can't write energy file\n\n";
      }
   }

   
   /**
    * Init the propramm parameters.
    * 
    */
   void ProgEigen::_InitParams()
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      Logger& log = Logger::InstanceRef();
      
      ParamContainer attr;
   
      attr = _EigenNode.Attributes();
      
      /* check method */
      if ( attr.isPresent("method")) {
         string s;
         attr.GetValue("method", s);
         if (s == "imag") _method = imag;
         else if (s == "ac") _method = ac;
         else throw (EParamProblem("Unknown method for EF calculation given: ", s));
      }
      
      /* Init the clock */
      if ( !attr.isPresent("dt") ) 
	 throw ( EParamProblem ("No Delta t given") );
      
      attr.GetValue("dt", _dt);
               
      /* Filter diagonalization */
      if ( attr.isPresent("diag"))
         attr.GetValue("diag", _diag);
      
      /* Specific parameters for imag solver */
      if (_method == imag) {
         /* Number of eigenfunctions */
         if ( attr.isPresent("Nef") ) {
            attr.GetValue("Nef", _Nef);
            if ( _Nef < 1)
               throw ( EParamProblem ("Less than one eigenfunction given requested") );
         }
         
         if ( attr.isPresent("start") ){
            attr.GetValue("start", _start);
            if (_start < 0)
               throw ( EParamProblem ("Can't start with negative ef") );
            if (_start > _Nef) 
               throw ( EParamProblem ("Can't start above number of desired EFs") );
         }
   
         
         /* Default convergence */
         if (_diag)
            _convergence = DEFAULT_CONVERGENCE_EF_DIAG;
         else
            _convergence = DEFAULT_CONVERGENCE_EF_RAW;
            
         
         /* Convergence */
         if ( attr.isPresent("conv") ) {
            attr.GetValue("conv", _convergence);
            if ( _convergence > 1)
               throw ( EParamProblem ("Convergence critera larger than one doesn't make sense") );
         }
      } else { /* specific for ac */
         if ( attr.isPresent("tol")){
            attr.GetValue("tol", _tol);
            if (_tol < 0)
               throw (EParamProblem("Tolerance factor for peak finder is non-sense. Must >= 0"));
         }
	 if ( attr.isPresent("read"))
	    attr.GetValue("read", _read);
         
         /* window function */
         attr.GetValue("win", _win, true);
      }
      
      /* Maximum number of steps */
      if ( attr.isPresent("steps") ) {
	 attr.GetValue("steps", _MaxSteps);
	 if ( _MaxSteps < 1)
	    throw ( EParamProblem ("Maximum number of time steps smaller than one requested") );
      }

      /* Init the clock */
      clock->Dt(_dt);
      clock->Steps(_MaxSteps);
      
      /* ef base name */
      if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", _fname);
	 if ( _fname.length() == 0)
	    throw ( EParamProblem ("Empty base name for file output defined") );
      }

      /* ef base name */
      if ( attr.isPresent("ename") ) {
	 attr.GetValue("ename", _ename);
	 if ( _ename.length() == 0)
	    throw ( EParamProblem ("Empty name for energy.dat") );
      }
      
      /* Init propagation output dir */
      if ( attr.isPresent("dir") && _dir.empty()) {
	 attr.GetValue("dir", _dir);
	 if (_dir[_dir.length()-1] != '/' && ! _dir.empty())
	    _dir += "/";
      }
      FS::CreateDir(_dir);
           
      
      clock->Dt(_dt);
      clock->Steps(_MaxSteps);
      
      log.Header("Eigenfunction calculation parameters", Logger::Section);
      
      log.cout() << "Method: ";
      if (_method == imag)
         log.cout() << "Relaxation in imaginary time\n";
      else
         log.cout() << "EF extraction from auto correlation\n";
      
      log.cout() << "Number of steps: " <<  clock->Steps() << endl;
      log.cout().precision(2);
      log.cout() << "Time step: " << fixed << clock->Dt() << endl;
      /* This is specific */
      if (_method == imag) {
         if (_start > 0)
            log.cout() << "Restart Calculation at EF " << _start << endl;

         log.cout() << "Number of Eigenfunctions: " << _Nef << endl;
         log.cout() <<  "Requested convergence: " << scientific << _convergence << endl;
      } else {
	 if (!_read.empty()){
	    log.cout() << "Read Propagation from files: " << _read << endl;
	 }
      }
      log.cout() << "Maximum propagation  time: " << fixed << _MaxSteps * clock->Dt() << endl;
      if (_diag)
         log.cout() <<  "Diagonalize basis" << endl;
      log.cout() << "Name for eigenenergy file: " << _ename << endl;
      log.cout() << "Basename for wave function output: " << _fname << endl;
      log.cout().precision(6); log.cout() << scientific;
      log.flush();
   }

   
   /**
    * Calculate eigenfunctions in imaginary time.
    */
   void ProgEigen::ImagTimeEF()
   {
      Logger& log = Logger::InstanceRef();
      QDClock *clock = QDGlobalClock::Instance();
      
      WaveFunction *Psi_old, *Psi, *buf;
      
      /* Force backward imaginary time */
      _U->ImaginaryTime();
      _U->Backward();
      GlobalOpList::Instance().Init(_U, _PsiInitial);
      
      /* Report what the propagator has chosen */
      ParamContainer Upm;
    
      Upm = _U->Params();
      
      log.cout() << "Propagators init parameters:\n\n" << Upm << endl;
            
      _Energies_raw.newsize(_Nef);
      
      Psi_old = _PsiInitial->NewInstance();
      Psi = _PsiInitial->NewInstance();
      buf = _PsiInitial->NewInstance();
            
      /* Prepare Restart */
      if (_start > 0) {
         ParamContainer pres;
         pres.SetValue("start", 0);
         pres.SetValue("num", _start);
         pres.SetValue("step", 1);
         pres.SetValue("files", _dir+_fname);
         _P->Init(pres);
         log.cout() << "Re-read eigenfunctions 0-" << _start-1 << endl<<endl;
      }
            
      /* Recalc Energies */
      if (_start > 0) {
         for (int i=0; i < _start; i++){
            _Energies_raw[i] = _H->Expec(_P->Get(i) );
         }
      }
      
      log.Header("EF - Relaxation", Logger::SubSection);
      log.cout() << "EF\tNt\tEnergy\t\tDelta <psi|psi_last>\n";
      log.flush();
      
      /* EF loop */
      for (int i=_start; i < _Nef; i++){
         *Psi = _PsiInitial;
         /* propagation loop */
         int s=0;
         double diff=1;
         while (s < _MaxSteps && diff > _convergence){
            *Psi_old = Psi;
            _U->Apply(Psi);
            /* Remove lower states */
            if ( i>0 ){
               _P->Apply(buf, Psi);
               *Psi -= buf;
            }
            /* normalization and convergence check */
            Psi->Normalize();
            diff = cabs(1-*Psi_old * Psi);

            ++(*clock);                     /* Step the clock */
            s++;
         }
         _P->Add(Psi);
         _efile << Psi;
         
         _Energies_raw[i] = _H->Expec(Psi);
         
         if ( fpclassify(_Energies_raw[i]) == FP_NAN)
            throw ( EOverflow("Energy is not a number") );
        

         log.cout().precision(8);
         log.cout() << i << "\t" << s << fixed <<"\t" << _Energies_raw[i] <<"\t";
         log.cout().precision(2);
         log.cout() << scientific << diff << endl;
         /* Check for bad convergence behavior */
         if ( i > 0 )
            if ( _Energies_raw[i] < _Energies_raw[i-1] )
               log.cout() << "Warning: Energy decreases. You should tighten the convergence criteria!\n";

         log.flush();
      }
      log.cout() << "\n\n";
      
      DELETE_WF(Psi_old);
      DELETE_WF(Psi);
      DELETE_WF(buf);
   }

   /**
    * Calculate eigenfunctions from a autocorrelation.
    */
   void ProgEigen::AutoCorrEF()
   {
      Logger& log = Logger::InstanceRef();
      QDClock *clock = QDGlobalClock::Instance();
      WaveFunction *Psi;
      cVec autocorr;
      cVec spectrum;
      
      /* Propagator intialization */
      GlobalOpList::Instance().Init(_U, _PsiInitial);
      
      /* Report what the propagator has chosen */
      ParamContainer Upm;
      Upm = _U->Params();
      log.cout() << "Propagators init parameters:\n\n" << Upm << endl;

      /* Propagation */
      WFBuffer tbuf;
      tbuf.AutoLock(1);
      tbuf.Size(_MaxSteps);
      tbuf.Init(_PsiInitial);
      
      if ( _read.empty() ){ /* Need to run Propagation */
	 autocorr.newsize(_MaxSteps);
	 spectrum.newsize(_MaxSteps);
         
	 Psi = _PsiInitial->NewInstance();
	 *Psi = _PsiInitial;
	 tbuf.Set(0,_PsiInitial);
	 
	 autocorr[0] = 1;
	 
	 log.cout() << "Run propagation\n\n";
	 log.coutdbg() << "Step\tTime\tNorm\n";
         log.flush();
	 for (int i=1; i < _MaxSteps; i++){/* Propagation loop */
	    _U->Apply(Psi);
	    tbuf.Set(i,Psi);
	    autocorr[i] = *Psi * _PsiInitial;
	    
	    if ( fpclassify(autocorr[i].real()) == FP_NAN)
	       throw ( EOverflow("Correlation is not a number") );
	    
	    ++(*clock);
	    log.coutdbg().precision(2);
	    log.coutdbg() << clock->TimeStep() << "\t" << clock->Time() << "\t";
	    log.coutdbg().precision(8);
	    log.coutdbg() << fixed << Psi->Norm() << endl;
	    log.flush();
	 }
	 log.coutdbg() << endl;
	 log.flush();
      } else { /* Read propagation from disk */
	log.cout() << "Read Propagation from disk\n\n"; log.flush();
	
	if (FS::IsDir(_read)) { /* Look for Propagation meta */
	   KeyValFile MetaFile(_read+"Propagation.meta");
	   ParamContainer meta;
	   string s;
	   int wcycle;
	   
	   MetaFile.Parse(meta);
	   
	   meta.GetValue("CLASS", s);
	   if (s != "Propagation")
	      throw (EParamProblem("Invalid Propagation meta file", _read+"Propagation.meta"));
	   
	   meta.GetValue("dt", _dt);
	   if (_dt <= 0)
	      throw (EParamProblem("Invalid dt"));
	   
	   meta.GetValue("Wcycle", wcycle);
	   if (wcycle <= 0)
	      throw (EParamProblem("Invalid write cycle"));
	   
	   meta.GetValue("Nt", _MaxSteps);
	   if (_MaxSteps < 2)
	      throw (EParamProblem("Invalid number of time steps"));

	   if (!meta.isPresent("WFBaseName"))
	      throw (EParamProblem("No WF base name given"));
	   
	   meta.GetValue("WFBaseName", s);
	   _read += s;
	   
	   _dt *= double(wcycle);
	   
	   log.cout() << "Found Propgation meta:\n";
	   log.IndentInc();
	   log.cout() << "Nt: " << _MaxSteps <<endl;
	   log.cout().precision(2);
	   log.cout() << "dt: " << fixed <<_dt <<endl;
	   log.cout() << "Base name: " << s <<endl<<endl;
	   log.IndentDec();
	   log.flush();
	}
	
	tbuf.ReadFromFiles(_read);
	   
	_MaxSteps = tbuf.Size();
	if (_MaxSteps < 2)
	   throw (EIOError("Not enough files for autocorrelation found"));
	
	autocorr.newsize(_MaxSteps);
	spectrum.newsize(_MaxSteps);
	
	Psi = tbuf[0]->NewInstance();
	
	if (! _H->Valid(Psi) )
	   throw (EIncompatible("The WFs from the propagation are incompatible with the guess"));
	
	*_PsiInitial =  tbuf[0];
	autocorr[0] = 1;
	log.cout() << "Rebuild autocorrelation\n\n"; log.flush();
	for (int i=1; i < _MaxSteps; i++) { /* Rebuild the autocorrelation */
	   autocorr[i] = *(tbuf[i]) * _PsiInitial;
	}
      }
     
      /* Apply a window function and do FFT */
      if (_win)
         WindowFuncs<cVec>::Hann(autocorr);
          
      Reporter::WriteSpectrum(autocorr, clock->Dt(), _dir+_spectrum);
      
      /* Make FFT and find eigenvalues */
      FFT AcFFT(autocorr, spectrum, true);
      PeakFinder PFind;
      dVec PowerSpectrum(_MaxSteps);
           
      AcFFT.forward();
      
      for (int i=0; i < _MaxSteps; i++)
         PowerSpectrum[i] = cabs(spectrum[i]);
      
      PFind.Find(PowerSpectrum);
      
      _Nef = PFind.NumPeaks();
      _Energies_raw.newsize(_Nef);
      
      log.cout() << "\nFound " << _Nef << " eigenvalues in spectrum\n";
      log.cout() << "\nIntegrating to obtain eigenfunctions\n";
      log.cout() << "\nEF#";
      log.coutdbg() << "\tindex\tenergy_est";
      log.cout() << "\tEnergy\n";
      log.flush();
     
      /* Integrate eigenfunctions out */
      for (int i=0; i < _Nef; i++){
         double energy;
         int index;
         index = PFind.NextPeak();

         /* Calculate energy */
         double dw = 2*M_PI / (clock->Dt() * double(clock->Steps()));
         
         if (index < _MaxSteps/2)
            energy = double(index) * dw;
         else
            energy = double(index-_MaxSteps) * dw;

         /* Run time integration */
         *Psi = _PsiInitial;
         for (int s=1; s < _MaxSteps; s++){
            
            AddElements((cVec*) Psi, (cVec*) tbuf[s], cexpI(energy*(double(s))*_dt));
         }
         
         Psi->Normalize(); /* Just normalize. We don't some factors, so we also drop  dt, 1/T */
         _Energies_raw[i] = _H->Expec(Psi);
         
         log.cout() << i;
         log.coutdbg() << "\t" << index << "\t" << energy;
         log.cout() << "\t" << _Energies_raw[i] << endl;
         log.flush();
         _efile << Psi;
         if (_diag) /* the diag proc. takes the basis from the Projector */
            _P->Add(Psi);
      }
      
      log.cout()<< endl;
      log.flush();
      DELETE_WF(Psi);
   }

   
   void ProgEigen::Run()
   {
      
      XmlNode *section;

      QDClock *clock = QDGlobalClock::Instance();
      Logger& log = Logger::InstanceRef();
      
      WaveFunction *Psi, *buf;
      
      _InitParams();
      
      /* Load & Init the propagator */
      _ContentNodes = _EigenNode.NextChild();
      
      log.Header( "QM Initialization", Logger::Section);
      log.Header( "Propagator: ", Logger::SubSection);
      
      section = _ContentNodes->FindNode( "propagator" );
      if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );
      
      _U = ChainLoader::LoadPropagator( section );
      _H = _U->Hamiltonian();
      delete section;
      
      /* Load the initial Wavefunction */
      section = _ContentNodes->FindNode( "wf" );
      if (section == NULL)
	 throw ( EParamProblem ("No inital wave function found") );
      log.Header( "Wavefunction: ", Logger::SubSection);
      log.IndentInc();
      _PsiInitial = ChainLoader::LoadWaveFunctionChain( section );
      delete section;
      log.IndentDec();
      
      /* Make sure our hamiltonian is initalized */
      GlobalOpList::Instance().Init(_H, _PsiInitial);
      _P->Init(_PsiInitial);
      log.cout() << "Initial Norm & energy: " << _PsiInitial->Norm() << "\t" << _H->Expec(_PsiInitial) << endl;
      
      _PsiInitial->Normalize();
      
      /* Let the Propagator do it's initalisation */
      clock->Begin();
      _U->Clock( clock );
      _H->UpdateTime();
      
      /* Init file writer for wf output */
      _efile.Name(_dir+_fname);
      _efile.Suffix(BINARY_WF_SUFFIX);
      _efile.ActivateSequence();

      
      /* Jump to specific method */
      if (_method == imag)
         ImagTimeEF();
      else
         AutoCorrEF();
      
      /* Optional basis diagonalization */
      if (_diag){
         log.Header("Diagonalize Hamiltonian", Logger::SubSection);
         /* Create Hamiltonian Matrix */
         dMat S;
         S.newsize(_Nef,_Nef);
         _Energies_diag.newsize(_Nef);
         S = 0;
         WaveFunction *bra,*ket;
         for (int i=0; i < _Nef; i++){
            bra = _P->Get(i);
            for (int j=0; j < i; j++){
               ket = _P->Get(j);
               S(j,i) =  _H->MatrixElement(bra, ket).real();
            }
            S(i,i) = _Energies_raw[i];
         }

         if (LAPACK::FullDiagHermitian(&S, &_Energies_diag) != 0)
            throw (EOverflow("Problem with diagonlization routine"));
         
         /* Build EFs in diag basis & write to file */
         _efile.ResetCounter();
         log.cout() << "EF\tDelta E [au]\tDelta E [cm-1]\n";
         Psi = _P->Get(0)->NewInstance();
         buf = _P->Get(0)->NewInstance();
         for (int i=0; i < _Nef; i++){
            *Psi = _P->Get(0);
            *Psi *= S(0,i);
            for (int j=1; j < _Nef; j++){ /* make linear combination */
               *buf = _P->Get(j);
               *buf *= S(j,i);
               *Psi += buf;
            }
            log.cout().precision(8);
            log.cout() << i << "\t" <<fixed<< _Energies_diag[i] - _Energies_raw[i];
            log.cout().precision(3);
            log.cout() << "\t" <<fixed<< (_Energies_diag[i] - _Energies_raw[i]) * AU2WAVENUMBERS << endl;
            log.flush();
            _efile << Psi;
         }
      }
      log.cout() << endl;

      WriteEnergyFile();
      
      /* Cleanup */
      DELETE_WF(_PsiInitial);
   }


}




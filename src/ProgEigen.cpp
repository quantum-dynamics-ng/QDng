#include "ProgEigen.h"

#include "tools/Logger.h"

#include "tools/QDGlobalClock.h"
#include "tools/helpers.h"
#include "tools/fstools.h"
#include "ChainLoader.h"

#include "tools/FileSingleDefs.h"
#include "tools/QDGlobalClock.h"

#include "ChainLoader.h"
#include "qdlib/FileWF.h"
#include "qdlib/Conversion.h"

#include "linalg/LapackDiag.h"

namespace QDLIB
{

   ProgEigen::ProgEigen(XmlNode &EigenNode) : _EigenNode(EigenNode),
                            _U(NULL), _H(NULL), _Nef(DEFAULT_NUMBER_EFS),
			    _convergence(DEFAULT_CONVERGENCE_EF_RAW),
			    _MaxSteps(DEFAULT_MAXSTEPS),
                                      _fname(DEFAULT_EF_BASE_NAME), _ename(DEFAULT_EF_ENERGY_NAME), _diag(true),
                                      _start(0)
   {
   }


   ProgEigen::~ProgEigen()
   {
      if (_U != NULL) delete _U;
      if (_H != NULL) delete _H;
      if (_h != NULL) delete _h;
      
      /* remove the clock */
      QDGlobalClock::Destroy();
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
      
      /* Init the clock */
      if ( !attr.isPresent("dt") ) 
	 throw ( EParamProblem ("No Delta t given") );
      
      attr.GetValue("dt", _dt);
      
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
         
      /* Filter diagonalization */
      if ( attr.isPresent("diag"))
         attr.GetValue("diag", _diag);
      
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
      CreateDir(_dir);
           
      
      clock->Dt(_dt);
      clock->Steps(_MaxSteps);
      
      log.Header("Eigenfunction calculation parameters", Logger::Section);
      
      log.cout() << "Number of steps: " <<  clock->Steps() << endl;
      log.cout().precision(2);
      log.cout() << "Time step: " << fixed << clock->Dt() << endl;
      if (_start > 0){
         log.cout() << "Restart Calculation at EF " << _start << endl;
      }
      log.cout() << "Number of Eigenfunctions: " << _Nef << endl;
      log.cout() << "Maximum propagation  time: " << fixed << _MaxSteps * clock->Dt() << endl;
      log.cout() <<  "Requested convergence: " << scientific << _convergence << endl;
      if (_diag)
         log.cout() <<  "Diagonalize basis" << endl;
      log.cout() << "Name for eigenenergy file: " << _ename << endl;
      log.cout() << "Basename for wave function output: " << _fname << endl;
      log.cout().precision(6); log.cout() << scientific;
      log.flush();
   }

   
   void ProgEigen::Run()
   {
      
      XmlNode *section;
      
      WaveFunction *Psi_old, *Psi, *Psi_initial, *_buf;
      Operator *h;
      
      FileWF efile;
      
      QDClock *clock = QDGlobalClock::Instance();
      Logger& log = Logger::InstanceRef();
      
      _InitParams();
      
      /* Load & Init the propagator */
      _ContentNodes = _EigenNode.NextChild();
      
      log.Header( "QM Initialization", Logger::Section);
      log.Header( "Propagator: ", Logger::SubSection);
      
      section = _ContentNodes->FindNode( "propagator" );
      if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );
      
      _U = ChainLoader::LoadPropagator( section, &h );
      delete section;
      
      /* Load the initial Wavefunction */
      section = _ContentNodes->FindNode( "wf" );
      if (section == NULL)
	 throw ( EParamProblem ("No inital wave function found") );
      log.Header( "Wavefunction: ", Logger::SubSection);
      log.IndentInc();
      Psi_initial = ChainLoader::LoadWaveFunctionChain( section );
      delete section;
      log.IndentDec();
      
      /* Make sure our hamiltonian is initalized */
      h->Init(Psi_initial);
      log.cout() << "Initial Norm & energy: " << Psi_initial->Norm() << "\t" << h->Expec(Psi_initial) << endl;
      
      /* Copy, since the propagator will propably scale it/modify etc. */
      _H = h->NewInstance();
      *_H = h;
      
      /* Let the Propagator do it's initalisation */
      clock->Begin();
      _U->Clock( clock );
      _H->UpdateTime();
      /* Force backward imaginary time */
      _U->ImaginaryTime();
      _U->Backward();
      _U->Init(Psi_initial);
      
      /* Report what the propagator has chosen */
      ParamContainer Upm;
    
      Upm = _U->Params();
      
      log.cout() << "Propagators init parameters:\n\n" << Upm << endl;
      
      /* Init file writer for wf output */
      efile.Name(_dir+_fname);
      efile.Suffix(BINARY_WF_SUFFIX);
      efile.ActivateSequence();
      
      _Energies_raw.newsize(_Nef);
         
      Psi_initial->Normalize();
      
      Psi_old = Psi_initial->NewInstance();
      Psi = Psi_initial->NewInstance();
      _buf = Psi_initial->NewInstance();
	    
      /* Prepare Restart */
      if (_start > 0) {
         ParamContainer pres;
         pres.SetValue("start", 0);
         pres.SetValue("num", _start);
         pres.SetValue("step", 1);
         pres.SetValue("files", _dir+_fname);
         _P.Init(pres);
         log.cout() << "Re-read eigenfunctions 0-" << _start-1 << endl<<endl;
      }
      
      _P.Init(Psi_initial);
	    
      /* Recalc Energies */
      if (_start > 0) {
         for (int i=0; i < _start; i++){
            _Energies_raw[i] = _H->Expec(_P.Get(i) );
         }
      }
      
      log.Header("EF - Relaxation", Logger::SubSection);
      log.cout() << "EF\tNt\tEnergy\n";
      
      /* EF loop */
      for (int i=_start; i < _Nef; i++){
	 *Psi = Psi_initial;
	 /* propagation loop */
	 int s=0;
	 double diff=1;
 	 while (s < _MaxSteps && diff > _convergence){
	    *Psi_old = Psi;
	    _U->Apply(Psi);
	    /* Remove lower states */
	    if ( i>0 ){
	       _P.Apply(_buf, Psi);
	       *Psi -= _buf;
	    }
	    /* normalization and convergence check */
            Psi->Normalize();
            diff = cabs(1-*Psi_old * Psi);

	    ++(*clock);                     /* Step the clock */
	    s++;
	 }
	 _P.Add(Psi);
	 efile << Psi;
	 
	 _Energies_raw[i] = _H->Expec(Psi);
	 log.cout().precision(8);
	 log.cout() << i << "\t" << s << fixed <<"\t" << _Energies_raw[i] << endl;
	 log.flush();
      }
      log.cout() << "\n\n";
      
      
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
            bra = _P.Get(i);
            for (int j=0; j < i; j++){
               ket = _P.Get(j);
               S(j,i) =  _H->MatrixElement(bra, ket).real();
            }
            S(i,i) = _Energies_raw[i];
         }

         if (LAPACK::FullDiagHermitian(&S, &_Energies_diag) != 0)
            throw (EOverflow("Problem with diagonlization routine"));
         
         /* Build EFs in diag basis & write to file */
         efile.ResetCounter();
         log.cout() << "EF\tDelta E [au]\tDelta E [cm-1]\n";
         for (int i=0; i < _Nef; i++){
            *Psi = _P.Get(0);
            *Psi *= S(0,i);
            for (int j=1; j < _Nef; j++){ /* make linear combination */
               *_buf = _P.Get(j);
               *_buf *= S(j,i);
               *Psi += _buf;
            }
            log.cout() << i << "\t" <<fixed<< _Energies_diag[i] - _Energies_raw[i];
            log.cout() << "\t" <<fixed<< (_Energies_diag[i] - _Energies_raw[i]) * AU2WAVENUMBERS << endl;
            log.flush();
            efile << Psi;
         }
      }
      log.cout() << endl;

      WriteEnergyFile();
      
      /* Cleanup */
      delete Psi_old;
      delete Psi;
      delete _buf;
      delete h;
      delete Psi_initial;
   }


}




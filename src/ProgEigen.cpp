#include "ProgEigen.h"

#include "tools/Logger.h"

#include "tools/QDGlobalClock.h"
#include "tools/FileSingleDefs.h"
#include "tools/helpers.h"
#include "ChainLoader.h"

#include "tools/FileSingleDefs.h"
#include "tools/QDGlobalClock.h"

#include "ChainLoader.h"
#include "qdlib/Conversion.h"

namespace QDLIB
{

   ProgEigen::ProgEigen(XmlNode &EigenNode) : _EigenNode(EigenNode),
                            _U(NULL), _H(NULL), _Nef(DEFAULT_NUMBER_EFS),
			    _convergence(DEFAULT_CONVERGENCE_EF),
			    _MaxSteps(DEFAULT_MAXSTEPS), _ncycle(DEFAULT_NCYCLE),
			    _fname(DEFAULT_EF_BASE_NAME), _ename(DEFAULT_EF_ENERGY_NAME)
   {
   }


   ProgEigen::~ProgEigen()
   {
      if (_U != NULL) delete _U;
      if (_H != NULL) delete _H;
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
	    enfile.precision(8);
	    enfile << i << "\t" << fixed << _Energies_raw[i];
	    enfile.precision(2);
	    enfile << "\t" << _Energies_raw[i]*AU2WAVENUMBERS;
	    enfile << "\t" << (_Energies_raw[i]-_Energies_raw[0])*AU2WAVENUMBERS;
	    if (i>0)
	       enfile << "\t" << (_Energies_raw[i]-_Energies_raw[i-1])*AU2WAVENUMBERS;
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
      
      /* Normalization & convergence check cycle */
      if ( attr.isPresent("ncycle") ) {
	 attr.GetValue("ncycle", _ncycle);
	 if ( _ncycle < 1)
	    throw ( EParamProblem ("Check cycle smaller than one requested") );
      }
      
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
      
      /* Filter diagonalization */
      if ( attr.isPresent("diag"))
	 throw ( EParamProblem ("Filter diagonalization not implementet yet!") );
	      
      
      clock->Dt(_dt);
      clock->Steps(_MaxSteps);
      
      log.Header("Eigenfunction calculation parameters", Logger::Section);
      
      log.cout() << "Number of steps: " <<  clock->Steps() << endl;
      log.cout().precision(2);
      log.cout() << "Time step: " << fixed << clock->Dt() << endl;
      log.cout() << "Check cycles: " << _ncycle << endl;
      log.cout() << "Maximum propagation  time: " << fixed << _MaxSteps * clock->Dt() << endl;
      log.cout() <<  "Requestet convergence: " << scientific << _convergence << endl;
      log.cout() << "Name for eigenenergy file: " << _ename << endl;
      log.cout() << "Basename for wave function output: " << _fname << endl;
      log.cout().precision(6); cout << scientific;
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
      
      Psi_initial = ChainLoader::LoadWaveFunctionChain( section );
      delete section;
      
      
      /* Make sure our hamiltonian is initalized */
      h->Init(Psi_initial);
      log.cout() << "Initial Norm & energy: " << Psi_initial->Norm() << "\t" << h->Expec(Psi_initial) << endl;
      
      /* Copy, since the propagator will propably scale it/modify etc. */
      _H = h->NewInstance();
      *_H = h;
      
      /* Let the Propagator do it's initalisation */
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
      
      log.cout() << "Eigenfunction\tEnergy\n";
     
   
      Psi_initial->Normalize();
      
      Psi_old = Psi_initial->NewInstance();
      Psi = Psi_initial->NewInstance();
      _buf = Psi_initial->NewInstance();
	    
      _P.Init(Psi_initial);
	    
      /* EF loop */
      for (int i=0; i < _Nef; i++){
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
	    if (s % _ncycle == 0){
	       Psi->Normalize();
	       diff = cabs(1-*Psi_old * Psi);
	    }
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
      

      WriteEnergyFile();
      
      /* Cleanup */
      delete Psi_old;
      delete Psi;
      delete _buf;
      delete Psi_initial;
   }


}




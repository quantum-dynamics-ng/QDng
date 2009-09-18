#include "ProgPropa.h"

#include "tools/FileSingleDefs.h"
#include "ChainLoader.h"
#include "tools/Logger.h"

#include "qdlib/WFMultistate.h"

namespace QDLIB {

   ProgPropa::ProgPropa(XmlNode & PropaNode) :
	 _propaNode(PropaNode), _ContentNodes(NULL),
	 _wcycle(DEFAULT_WRITE_CYCLE), _fname(DEFAULT_BASENAME), _dir(""), _nfile(DEFAULT_NORMFILE), _writenorm(false),
         _U(NULL), _H(NULL), _usepost(false), _usepre(false)
   {
   }
   
   
   ProgPropa::~ProgPropa()
   {
      if (_H != NULL) delete _H;
      if (_U != NULL) delete _U;

      /* remove the clock */
      QDGlobalClock::Destroy();
   }
   
   
   /**
    * Init all the global propa values needed including the reporter values.
    */
   void ProgPropa::_InitParams()
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      ParamContainer attr;
      Logger& log = Logger::InstanceRef();
      
      attr = _propaNode.Attributes();
   
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
   
      /* Init write cycles */
      if ( attr.isPresent("wcycle") ) {
	 attr.GetValue("wcycle", _wcycle);
	 if ( steps < 1)
	    throw ( EParamProblem ("Write cycle less than one defined") );
	 _reporter.WriteCycle(_wcycle);
      }
      
      /* Init propagation output dir */
      if ( attr.isPresent("dir") && _dir.empty() ) {
	 attr.GetValue("dir", _dir);
	 if (_dir[_dir.length()-1] != '/' && ! _dir.empty())
	    _dir += "/";
      }
      
      /* Init propagation file basename */
      if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", _fname);
      }
            
      log.Header("Propagation parameters", Logger::Section);
    
      log.IndentInc();
      log.cout() << "Number of steps: " <<  clock->Steps() << endl;
      log.cout().precision(2);
      log.cout() << "Time step: " << fixed << clock->Dt() << endl;
      log.cout() << "Write cycles: " << _wcycle << endl;
      log.cout() << "Overall time: " << fixed << clock->Steps() * clock->Dt() << endl;
      log.cout() << "Basename for wave function output: " << _fname << endl;
      log.flush();
      
      log.IndentDec();
      log.cout().precision(6); log.cout() << scientific;
      
      /* Check the reporter values */
      bool onoff;
      
      attr.GetValue("norm", onoff, true);
      _reporter.Norm( onoff );

      attr.GetValue("energy", onoff, true);
      _reporter.Energy(onoff);

      attr.GetValue("proj0", onoff);
      _reporter.Proj0(onoff);

      attr.GetValue("proj0Abs", onoff);
      _reporter.Proj0Square(onoff);
	 

      if ( attr.isPresent("spectrum") ) {
	 attr.GetValue("spectrum", s);
	 _reporter.Spectrum( _dir+s );
      }

      if (attr.isPresent("nfile")){
	 _writenorm = true;
	 attr.GetValue("nfile",s);
	 if (! s.empty() ) _nfile = _dir+s;
      }
      
   }


   /**
    * Run the propagation program.
    */
   void ProgPropa::Run()
   {
      Logger& log = Logger::InstanceRef();
      
      XmlNode *section;
      Operator *_h;
      
      /* Init global Propa parameters */
      _InitParams();
      
      /* fetch the ChildNodes */
      _ContentNodes = _propaNode.NextChild();
      
      log.Header( "QM Initialization", Logger::Section);
      log.Header( "Propagator: ", Logger::SubSection);
      
      
      /* Load & Init the propagator */
      section = _ContentNodes->FindNode( "propagator" );
      if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );
      
      _U = ChainLoader::LoadPropagator( section, &_h );
      delete section;
            
      /* Load the initial Wavefunction */
      WaveFunction *Psi;
      section = _ContentNodes->FindNode( "wf" );
      if (section == NULL)
	 throw ( EParamProblem ("No inital wave function found") );
      
      log.Header( "Initial wave function", Logger::SubSection);
      Psi = ChainLoader::LoadWaveFunctionChain( section );
      delete section;
      
      
      /* Pre step filters */
      section = _ContentNodes->FindNode( "filterpre" );
      if (section != NULL) {
	 string s(_dir+DEFAULT_EXPEC_PRE_FILENAME);
	 log.Header( "Using pre propagation step filters", Logger::SubSection);
	 log.IndentInc();
	 _prefilter.SetDefaultName(s);
	 _prefilter.Init( section );
	 _usepre = true;
	 log.IndentDec();
	 delete section;
      }

      /* Post step filters */
      section = _ContentNodes->FindNode( "filterpost" );
      if (section != NULL) {
	 string s(_dir+DEFAULT_EXPEC_POST_FILENAME);
	 log.Header("Using post propagation step filters", Logger::SubSection);
	 log.IndentInc();
	 _postfilter.SetDefaultName(s);
	 _postfilter.Init( section );
	 _usepost = true;
	 log.IndentDec();
	 delete section;
      }
      
      
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
           
      /* Make sure our hamiltonian is initalized */
      _h->Clock( clock );
      _h->Init(Psi);
      log.cout() << "Initial engergy: " << _h->Expec(Psi) << endl;
      
      /* Copy, since the propagator will propably scale it/modify etc. */
      _H = _h->NewInstance();
      *_H = _h; 

       _H->Clock( clock );
      
      /* Give the reporter module what it needs */
      _reporter.PsiInitial( Psi );
      _reporter.Hamilton( _H );
      
      /* Let the Propagator do it's initalisation */
      _U->Clock( clock );
      _H->UpdateTime();
      _U->Init(Psi);
      
      /* Report what the propagator has chosen */
      ParamContainer Upm;
    
      Upm = _U->Params();
      log.cout() << "Propagators init parameters:\n\n" << Upm << endl;
      
      /* Init file writer for wf output */
      FileWF wfile;
      
      /* Dirty hack to get meta data right for multistate WFs */
      /** \todo Remove this hack => make clean File Class */
      WFMultistate *wfm;
      wfm = dynamic_cast<WFMultistate*>(Psi);
      if (wfm != NULL){
	 ParamContainer& pfm = wfm->Params();
	 pfm = wfm->State(0)->Params();
      }
      
      wfile.Name(_dir+_fname);
     
      wfile.Suffix(BINARY_WF_SUFFIX);
      wfile.ActivateSequence();
      wfile << Psi;
      
      /* The propagation loop */
      log.Header( "Free propagation", Logger::Section);
      log.flush();
      /* redirect to to normfile */
      if( _writenorm ) {
         log.cout() << "Propagation table is redirected to Norm file: " << _nfile << endl;
         log.flush();
         log.FileOutput( _nfile );
      }
      for (lint i=1; i <= clock->Steps(); i++){
	 if (_usepre) _prefilter.Apply( Psi ); /* Apply pre-filters*/
	 _reporter.Analyze( Psi );      /* propagation report. */
	_U->Apply(Psi);                 /* Propagate */
	if (_usepost) _postfilter.Apply( Psi );/* Apply post-filters*/
	if (i % _wcycle == 0) wfile << Psi;  /* Write wavefunction */
	++(*clock);                     /* Step the clock */
      }
      if( _writenorm )  log.FileClose();
      _reporter.Finalize();

      /* Write propagation meta file*/
      ParamContainer p;
      p.SetValue("CLASS", "Propagation" );
      if (wfm != NULL){
         p.SetValue("WFCLASS", wfm->Name() );
         p.SetValue("States", wfm->States() );
      } else {
         p.SetValue("WFCLASS", Psi->Name() );
      }
      p.SetValue("Nt", clock->Steps() );
      p.SetValue("dt", clock->Dt() );
      p.SetValue("WFBaseName", _fname );
      p.SetValue("Wcycle", _wcycle );
      KeyValFile meta_file_propa(_dir + "Propagation" + METAFILE_SUFFIX);
      if ( !meta_file_propa.Write(p) ) EIOError("Can not write meta file");
      
      delete _h;
      delete Psi;
   }

}


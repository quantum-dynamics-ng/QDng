#include "ProgPropa.h"

#include "ChainLoader.h"
#include "tools/Logger.h"
#include "tools/fstools.h"
#include "qdlib/WFMultistate.h"
#include "qdlib/FileWF.h"
#include "GlobalOpList.h"

namespace QDLIB {

   ProgPropa::ProgPropa(XmlNode & PropaNode) :
	 _propaNode(PropaNode), _ContentNodes(NULL),
	 _wcycle(DEFAULT_WRITE_CYCLE), _fname(DEFAULT_BASENAME), _start(0), _dir(""), _nfile(DEFAULT_NORMFILE), _writenorm(false),
         _scinorm(false), _U(NULL), _H(NULL), _usepost(false), _usepre(false)
   {
   }
   
   
   ProgPropa::~ProgPropa()
   {
      /* remove the clock */
      QDGlobalClock::Destroy();
      GlobalOpList::Instance().Clear();
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
      clock->Begin();
   
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
      FS::CreateDir(_dir);
      
      /* Init propagation file basename */
      if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", _fname);
      }
            
      /* Init propagation file counter */
      if ( attr.isPresent("start") ) {
	 attr.GetValue("start", _start);
	 if (_start < 0)
	    throw ( EParamProblem ("Only positive counter start counter numbers are allowed") );
      }
      
      log.Header("Propagation parameters", Logger::Section);
    
      log.IndentInc();
      log.cout() << "Number of steps: " <<  clock->Steps() << endl;
      log.cout().precision(2);
      log.cout() << "Time step: " << fixed << clock->Dt() << endl;
      log.cout() << "First step number : " << _start << endl;
      log.cout() << "Write cycles: " << _wcycle << endl;
      log.cout() << "Overall time: " << fixed << clock->Steps() * clock->Dt() << endl;
      log.cout() << "Basename for wave function output: " << _fname << endl;
      log.cout() << "Directory for output: " << _dir << endl;
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
         
         attr.GetValue("win", onoff, true);
         _reporter.Window(onoff);
      }

      /* Optional normfile */
      if (attr.isPresent("nfile")){
	 attr.GetValue("nfile",s);
	 if (! s.empty() ){
            _nfile = _dir+s;
            _writenorm = true;
         }
      }
      
      attr.GetValue("scinorm", _scinorm, false);
      _reporter.SciNorm(_scinorm);
   }


   /**
    * Run the propagation program.
    */
   void ProgPropa::Run()
   {
      Logger& log = Logger::InstanceRef();
      
      XmlNode *section;
      
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
      
      _U = ChainLoader::LoadPropagator( section );
      _H = _U->Hamiltonian();
      delete section;
            
      /* Load the initial Wavefunction */
      WaveFunction *Psi;
      section = _ContentNodes->FindNode( "wf" );
      if (section == NULL)
	 throw ( EParamProblem ("No inital wave function found") );
      
      log.Header( "Initial wave function", Logger::SubSection);
      log.IndentInc();
      Psi = ChainLoader::LoadWaveFunctionChain( section );
      delete section;
      log.IndentDec();
      log.flush();
      
      /* Initialization filters */
      section = _ContentNodes->FindNode( "filterinit" );
      FiltersList filter;
      if (section != NULL) {
         string s(_dir+DEFAULT_EXPEC_INIT_FILENAME);
         log.Header( "Using pre propagation initialization filters", Logger::SubSection);
         log.IndentInc();
         filter.SetDefaultName(s);
         filter.Init( section );
         filter.Apply(Psi);
         log.IndentDec();
         log.cout() << endl;
         log.flush();
         delete section;
      }
      
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
         log.cout() << endl;
         log.flush();
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
	 log.cout() << endl;
         log.flush();
	 delete section;
      }
      
      
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
           
      /* Make sure our hamiltonian is initalized */
      _H->Clock( clock );
      GlobalOpList::Instance().Init(_H, Psi);
      log.cout() << "Initial energy: " << _H->Expec(Psi) << endl;
            
      /* Give the reporter module what it needs */
      _reporter.PsiInitial( Psi );
      _reporter.Hamilton( _H );
      
      /* Let the Propagator do it's initalisation */
      _U->Clock( clock );
      _H->UpdateTime();
      GlobalOpList::Instance().Init(_U, Psi);
      
      /* Report what the propagator has chosen */
      ParamContainer Upm;
    
      Upm = _U->Params();
      log.cout() << "Propagators init parameters:\n\n" << Upm << endl;
      
      /* Init file writer for wf output */
      FileWF wfile;
  
      wfile.Name(_dir+_fname);
      wfile.Suffix(BINARY_WF_SUFFIX);
      wfile.ActivateSequence();
      
      if (_start != 0)
	 wfile.Counter(_start);
      else
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

      lint start;
      if (_start == 0)
	 start=1;
      else
	 start = _start;
      
      clock->TimeStep(_start);
      
      for (lint i=start; i <= clock->Steps(); i++){
	 if (_usepre) _prefilter.Apply( Psi ); /* Apply pre-filters*/
	 _reporter.Analyze( Psi );      /* propagation report. */
	_U->Apply(Psi);                 /* Propagate */
	if (_usepost) _postfilter.Apply( Psi );/* Apply post-filters*/
	if (i % _wcycle == 0){
           wfile << Psi;  /* Write wavefunction */
        }
	++(*clock);                     /* Step the clock */
      }
      if( _writenorm )  log.FileClose();
      _reporter.Finalize();

      /* Write propagation meta file*/
      ParamContainer p;
      p.SetValue("CLASS", "Propagation" );
      p.SetValue("Nt", clock->Steps() );
      p.SetValue("dt", clock->Dt() );
      p.SetValue("WFBaseName", _fname );
      p.SetValue("Wcycle", _wcycle );
      KeyValFile meta_file_propa(_dir + "Propagation" + METAFILE_SUFFIX);
      if ( !meta_file_propa.Write(p) ) EIOError("Can not write meta file");

      DELETE_WF(Psi);
   }

}


/*
 * ProgCfun.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: markus
 */

#include "ProgCfun.h"

#include "ChainLoader.h"
#include "GlobalOpList.h"
#include "tools/Logger.h"
#include "qdlib/WFBuffer.h"
#include "tools/FileSingle.h"

namespace QDLIB
{

  ProgCfun::ProgCfun (XmlNode& CfunNode) :
      CfunNode_(CfunNode), ContentNodes_(NULL), dir_(""), fname_(QD_CFUN_FNAME),
      stepsint_(0), U_(NULL), H_(NULL), ket_first_op_(0), bra_first_op_(0), num_u_(0), wcycle_(1)
  {
  }

  ProgCfun::~ProgCfun ()
  {
    GlobalOpList::Instance().Clear();
  }

  void ProgCfun::InitParams_ ()
  {
    Logger& log = Logger::InstanceRef();
    ParamContainer attr =  CfunNode_.Attributes();


    /* Init output dir */
    if ( attr.isPresent("dir") && dir_.empty() ) {
	 attr.GetValue("dir", dir_);
	 if (dir_[dir_.length()-1] != '/' && ! dir_.empty())
	    dir_ += "/";
    }
    FS::CreateDir(dir_);

    /* Init propagation file basename */
    if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", fname_);
    }


    if ( attr.isPresent("read") ) { /* Read base line propgation from directory */
	attr.GetValue("read", read_dir_);
    }

    if ( attr.isPresent("write") ) { /* dump base line propgation to directory */
	attr.GetValue("write", write_dir_);
    }

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

    if ( attr.isPresent("stepsint") ){
      attr.GetValue("stepsint", stepsint_);
      if (stepsint_ <= 0) {
	  throw(EParamProblem("stepsint must be larger than 0"));
      }
    }

    master_clock_.Dt(dt);
    master_clock_.Steps(steps);

    if ( attr.isPresent("wcycle") ) {
	attr.GetValue("wcycle", wcycle_);
    }
  }

  int ProgCfun::ReadKetBra(const string& name, vector<Operator*>& kb )
  {
    Logger& log = Logger::InstanceRef();
    XmlNode* section = ContentNodes_->FindNode( name );
    int first_op = -1; int i=0;

    if (section != NULL) {
      log.Header( "Read definiton for " + name, Logger::SubSection);
      log.IndentInc();

      XmlNode *child = section->NextChild();
      child->AdjustElementNode();

      while (child->EndNode()){
	  if ( child->Name() == "U" ) {
	      kb.push_back(NULL);  /* empty defintion means propgator*/
	      num_u_++;
	  } else {
	      if (first_op < 0) first_op = i;
	      kb.push_back( ChainLoader::LoadOperatorChain( child ) );
	  }
	  i++;
	  child->NextNode();
      }

      delete section;
      log.IndentDec();
    }

    return first_op;
  }

  void ProgCfun::CreateMetaData(int dims, ParamContainer &pm)
  {
    pm.SetValue("CLASS", "Cfun");
    pm.SetValue("Nt", master_clock_.Steps()/wcycle_);
    pm.SetValue("dt", master_clock_.Dt() * wcycle_);
    pm.SetValue("dim", dims);
  }

  /** Calculate single-point correlation function. */
  void ProgCfun::RunC1 ()
  {
    Logger& log = Logger::InstanceRef();

    Cfunc cfun(master_clock_.Steps()/wcycle_); /* Correlation function */
    cfun.cVec::operator=(dcomplex(0)); /* set zero */
    WaveFunction* psi_t = wfbuffer.Get(0)->NewInstance();

    log.Header("Calculate One-point correclation function", Logger::SubSection);
    log.flush();

    for (int t=0; t < master_clock_.Steps()/wcycle_; t++){
	WaveFunction* ket = wfbuffer.Get(t);
	*psi_t = ket;
	for (int i=ket_first_op_; i < ket_.size(); i++){
	    if (ket_[i] != NULL){
		ket_[i]->Apply(psi_t);
	    } else
	      break;
	}

	cfun[t] = *ket * psi_t;
    }

    DELETE_WF(psi_t);

    log.cout() << "Write correlation function\n";
    /* Write correlation function to file */
    FileSingle<Cfunc> file;
    CreateMetaData(1, cfun.Params());

    file.Format(FileSingle<Cfunc>::binary);
    file.Name("Cfun_1");
    file.Suffix(".cf");
    file.WriteSingleFileToRaw(&cfun);
  }


  /** Calculate two-point correlation function. */
  void ProgCfun::RunC2 ()
  {
    Logger& log = Logger::InstanceRef();

    int Nt = master_clock_.Steps()/wcycle_;
    Cfunc cfun(Nt*Nt); /* Correlation function */
    cfun.cVec::operator=(dcomplex(0)); /* set zero */

    WaveFunction* psi_t_k = wfbuffer.Get(0)->NewInstance();
    WaveFunction* psi_t_b = wfbuffer.Get(0)->NewInstance();

    log.cout() << endl;
    log.Header("Calculate Two-point correclation function", Logger::SubSection);
    log.flush();

    for (int t1=0; t1 < Nt; t1++){
	WaveFunction* ket = wfbuffer.Get(t1);
	*psi_t_k = ket;
	for (int i=ket_first_op_; i < ket_.size(); i++){
	    if (ket_[i] != NULL){
		ket_[i]->Apply(psi_t_k);
	    } else
	      break;
	}

	master_clock_.TimeStep(t1*wcycle_);
	for (int t2=t1; t2 < Nt; t2++){
	  WaveFunction* bra = wfbuffer.Get(t2);
	  *psi_t_b = bra;
	  for (int i=ket_first_op_; i < ket_.size(); i++){
	      if (ket_[i] != NULL){
		  ket_[i]->Apply(psi_t_b);
	      } else
		break;
	  }
	  cfun[t1*Nt+t2] = *psi_t_b * psi_t_k;

	  if (t2-t1 > stepsint_ && stepsint_ > 0) break;

	  for (int i=0; i < wcycle_; i++) /* propgate wcycle steps forward */
	    U_->Apply(psi_t_k);
	    ++master_clock_;
	}
    }

    DELETE_WF(psi_t_k);
    DELETE_WF(psi_t_b);

    log.cout() << "Write correlation function\n";
    /* Write correlation function to file */
    FileSingle<Cfunc> file;
    CreateMetaData(2, cfun.Params());

    file.Format(FileSingle<Cfunc>::binary);
    file.Name("Cfun_2");
    file.Suffix(".cf");
    file.WriteSingleFileToRaw(&cfun);
  }

  void ProgCfun::Run ()
  {
    InitParams_ ();

    Logger& log = Logger::InstanceRef();
    XmlNode *section;

    log.Header( "QM Initialization", Logger::Section);
    log.Header( "Propagator: ", Logger::SubSection);

    /* fetch the ChildNodes */
    ContentNodes_ = CfunNode_.NextChild();

    /* Load & Init the propagator */
    section = ContentNodes_->FindNode( "propagator" );
    if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );

    U_ = ChainLoader::LoadPropagator( section );
    H_ = U_->Hamiltonian();
    delete section;


    log.Header( "Initial wave function", Logger::SubSection);
    log.IndentInc();

    /* Load the initial Wavefunction */

    WaveFunction *Psi;

    if (read_dir_.empty()) { /* propagate baseline */
      section = ContentNodes_->FindNode( "wf" );
      if (section == NULL)
	   throw ( EParamProblem ("No inital wave function found") );

      Psi = ChainLoader::LoadWaveFunctionChain( section );
      delete section;

      wfbuffer.Add(Psi);
    } else { /* Get propgation from file */
	/* Get parameters for propagation */
	ParamContainer propagation_meta;
	if (! propagation_meta.ReadFromFile(read_dir_ + "/" + "Propagation" + METAFILE_SUFFIX) ) {
	    throw(EParamProblem("Propagation meta file not found"));
	}

	int Nt, wcycle;
	double dt;
	propagation_meta.GetValue("Nt", Nt);
	propagation_meta.GetValue("wcycle", wcycle);
	propagation_meta.GetValue("dt", dt);

	master_clock_.Dt(dt);
	master_clock_.Steps(Nt);
	wcycle_ = wcycle;		/* use wcycle from propagation */

	/* Read propagation */
	wfbuffer.ReadFromFiles(read_dir_);

	if (wfbuffer.Size() < Nt/wcycle ) master_clock_.Steps(wfbuffer.Size());
	if (wfbuffer.Size() < 1 )
	  throw (EParamProblem("Wave function buffer is empty"));

	Psi = wfbuffer[0];
    }

    log.IndentDec();
    log.flush();

    /* Let the Propagator do it's initalisation */
    U_->Clock( &master_clock_ );
    U_->Init(Psi);
    H_ = U_->Hamiltonian();
    H_->UpdateTime();

    /* Get defintion for bra */
    bra_first_op_ = ReadKetBra("bra", bra_);
    for (int i=0; i < bra_.size(); i++) {
	if (bra_[i] != NULL) bra_[i]->Init(Psi);
    }


    /* Get defintion for ket */
    ket_first_op_ = ReadKetBra("ket", ket_);
    for (int i=0; i < ket_.size(); i++) {
	if (ket_[i] != NULL) ket_[i]->Init(Psi);
    }

    if (bra_.size() == 0){ /* no bra means: <psi|U+ */
	num_u_ = num_u_+1;
    }

    if (wfbuffer.Size() > 1 && (ket_[0] != NULL || bra_[0] != NULL))
      throw (EParamProblem("Pre-propagated time series requires a propagator as first element int the ket and bra definitions."));

    /* Run initial time series, if not already present */
    if (wfbuffer.Size() == 1){
	log.cout() << "\nRun initial propagation U(t1,0)|psi>\n"; log.flush();
	WaveFunction* psi_t = Psi->NewInstance();

	if (ket_[0] != NULL) {
	    /* apply operator list before propagating */
	    *psi_t = Psi;
	    for (size_t i=0; i < ket_.size(); i++) {
		ket_[i]->Apply(psi_t);
	    }
	} else {
	  *psi_t = Psi;
	}

	/* fill wfbuffer with time series */
	master_clock_.Begin();
	for (int i=1; i < master_clock_.Steps(); i++){
	    U_->Apply(psi_t);
	    ++master_clock_;
	    if (i % wcycle_ == 0) wfbuffer.Add(psi_t);
	}

	DELETE_WF(psi_t);

	/* Dump series to files */
	if (! write_dir_.empty() ){
	  FS::CreateDir(write_dir_);

	  /* Write propagation meta file*/
	  ParamContainer p;
	  p.SetValue("CLASS", "Propagation");
	  p.SetValue("Nt", master_clock_.Steps());
	  p.SetValue("dt", master_clock_.Dt());
	  p.SetValue("WFBaseName", "WF");
	  p.SetValue("Wcycle", wcycle_);

	  if (!p.WriteToFile(write_dir_ + "/Propagation" + METAFILE_SUFFIX))
	     EIOError("Can not write meta file");

	  wfbuffer.SaveToFiles(write_dir_ + "/WF");
	}
    }

    /* Calculate correlation function */
    switch(num_u_ - 1) {
      case 1:
	  RunC1();
	  break;
      case 2:
	  RunC2();
	  break;
      default:
	throw(EParamProblem("Invalid number of propgations: ", num_u_));
    }

    /* Do */



  }

} /* namespace QDLIB */

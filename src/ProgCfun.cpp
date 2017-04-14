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

    log.Header("Correlation function parameters", Logger::Section);

    log.IndentInc();
    log.cout() << "Number of steps: " <<  master_clock_.Steps() << endl;
    log.cout().precision(2);
    log.cout() << "Time step: " << fixed << master_clock_.Dt() << endl;
    log.cout() << "Write cycles: " << wcycle_ << endl;
    log.cout() << "Overall time: " << fixed << master_clock_.Steps() * master_clock_.Dt() << endl;
    log.cout() << "Directory for output: " << dir_ << endl;
    log.flush();

    log.IndentDec();
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

  WaveFunction* ProgCfun::InitializeKetBra(bool bra)
  {
    WaveFunction* psi_t = Psi0->NewInstance();
    int first_op = ket_first_op_;
    vector<Operator*>* ops = &ket_;

    if (bra) {
      first_op = bra_first_op_;
      ops = &bra_;
    }

    if ((*ops)[0] != NULL) {
      /* apply operator list before propagating */
      *psi_t = Psi0;
      for (size_t i=0; i < ops->size(); i++) {
         if ((*ops)[i] == NULL) {
             first_op++;
             break;
         }
         (*ops)[i]->Apply(psi_t);
         first_op++;
      }
    } else {
      *psi_t = Psi0;
    }

    if (bra) {
      bra_first_op_ = first_op;
    }

    return psi_t;
  }

  /**
   * Calculate single-point correlation function.
   *
   *  Case a
   *           /---\
   *  mu_s(t)  |   | U+(t,t0)
   *  U(t,t0)  |   |
   *  t0       |   | t0
   *  [mu_0b]  |   | [mu_0k]
   *
   * */
  void ProgCfun::RunC1 ()
  {
    Logger& log = Logger::InstanceRef();

    Cfunc cfun(master_clock_.Steps()/wcycle_); /* Correlation function */
    cfun.cVec::operator=(dcomplex(0)); /* set zero */
    WaveFunction* psi_t = wfbuffer.Get(0)->NewInstance();
    WaveFunction* bra = NULL;

    log.Header("Calculate One-point correlation function", Logger::SubSection);
    log.flush();

    if (bra_.size() > 0 ){
	bra = InitializeKetBra(true);
    }

    master_clock_.Begin();
    for (int t=0; t < master_clock_.Steps()/wcycle_; t++){
	WaveFunction* ket = wfbuffer.Get(t);
	*psi_t = ket;

	master_clock_.TimeStep(t * wcycle_); // make sure time dependent ops are in sync
	for (uint i=ket_first_op_; i < ket_.size(); i++){
	    if (ket_[i] != NULL){
		ket_[i]->Apply(psi_t);
	    } else
	      break;
	}

	if ( bra == NULL) {
	    cfun[t] = *ket * psi_t;
	} else {
	    for (int tb=0; tb < wcycle_; tb++) {
		U_->Apply(bra);
		++master_clock_;
	    }
	    cfun[t] = *bra * psi_t;
	}
    }

    DELETE_WF(psi_t);

    log.cout() << "Write correlation function\n";
    /* Write correlation function to file */
    FileSingle<Cfunc> file;
    CreateMetaData(1, cfun.Params());

    file.Format(FileSingle<Cfunc>::binary);
    file.Name(dir_ + "Cfun_1");
    file.Suffix(".cf");
    file.WriteSingleFileToRaw(&cfun);
  }


  /**
   * Calculate two-point correlation function.
   *
   *  Case a                   Case b
   *           /---\                    /---\
   *  mu_s(t)  |   | U+(t,t0)  m_s(t)   |   |
   *  U(t,t1)  |   |           U(t,t0)  |   | U+(t,t1)
   *  mu_1(t1) |   |                    |   | mu_1(t1)
   *  U(t1,t0) |   |                    |   | U+(t1,t0)
   *  t0       |   | t0             t0  |   | t0
   *  [mu_0k]  |   | [mu_0b]    [mu_0b] |   | [mu_0k]
   * */
  void ProgCfun::RunC2 ()
  {
    Logger& log = Logger::InstanceRef();

    int Nt = master_clock_.Steps()/wcycle_;
    Cfunc cfun(Nt*Nt); /* Correlation function */
    cfun.cVec::operator=(dcomplex(0)); /* set zero */

    WaveFunction* psi_t_k = wfbuffer.Get(0)->NewInstance();
    WaveFunction* psi_t_s = wfbuffer.Get(0)->NewInstance();
    WaveFunction* psi_t_b = wfbuffer.Get(0)->NewInstance();
    WaveFunction* bra = NULL;

    if (bra_.size() > 0 ){
	bra = InitializeKetBra(true);
    }

    log.cout() << endl;
    log.Header("Calculate Two-point correlation function", Logger::SubSection);
    log.flush();


    // find the first operators after the first U in ket and bra
    uint nUket = 0; uint first_ket = 0;
    for (uint i=0; i < ket_.size(); i++){
	if ( ket_[i] == NULL ) {
	    nUket++;
	} else
	    if (nUket == 1) first_ket = i;
    }

    uint nUbra = 0; uint first_bra = 0;
    if (bra != NULL)
	for (uint i=0; i < bra_.size(); i++){
	    if ( bra_[i] == NULL ) {
		nUbra++;
	    } else
		if (nUbra == 1) first_bra = i;
	}

    // figure out which variant to use
    bool lhsint = false; // case a = true, b = false
    if (nUket == 2 && nUbra <= 1 )
	lhsint = true;
    else if (nUket == 1 && nUbra == 2)
	lhsint = false;
    else
	throw(EParamProblem("Wrong number of propagators in ket/bra definitions!"));

    master_clock_.Begin();
    for (int t1=0; t1 < Nt; t1++){
	master_clock_.TimeStep(t1*wcycle_);
	if (lhsint) {
	    *psi_t_k = wfbuffer.Get(t1); // U|Psi0>
	    for (uint i=first_ket; i < ket_.size(); i++){ // O1 U|Psi0>
		if (ket_[i] != NULL){
		    ket_[i]->Apply(psi_t_k);
		} else
		  break;
	    }
	}

	// Propagate independent bra side
	if (bra != NULL){
	    int step = master_clock_.TimeStep();
	    for (int i=0; i < wcycle_; i++) {
		U_->Apply(bra);
		++master_clock_;
	    }
	    *psi_t_b = bra;
	    master_clock_.TimeStep(step); // restore time step
	}

	if (!lhsint) {
	    for (uint i=first_bra; i < bra_.size(); i++){ //  <Psi0|U+ O1
		if (bra_[i] != NULL){
		    bra_[i]->Apply(psi_t_b);
		} else
		  break;
	    }
	}

	master_clock_.TimeStep(t1*wcycle_);
	for (int t2=t1; t2 < Nt; t2++){
	    if (bra == NULL) {	// Two ops on the ket side, no definition for bra side
		*psi_t_b = wfbuffer.Get(t2);
	    }

	    if (!lhsint) {
		*psi_t_k = wfbuffer.Get(t2);
	    }

	    // Signal interaction on ket
	    *psi_t_s = psi_t_k;
	    int ketsigop = first_ket+2;
	    if (!lhsint)
		ketsigop = first_ket;

	    for (uint i=ketsigop; i < ket_.size(); i++){
		if (ket_[i] != NULL){
		    ket_[i]->Apply(psi_t_s);
		} else
		  break;
	    }

	    if (lhsint)
		cfun[t1*Nt+t2] = *psi_t_b * psi_t_s;
	    else
		cfun[t2*Nt+t1] = *psi_t_b * psi_t_s;

	  if (t2-t1 > stepsint_ && stepsint_ > 0) break;

	  for (int i=0; i < wcycle_; i++){ // U O1 U|Psi0> /* propagate wcycle steps forward */
	    if (lhsint) {
		U_->Apply(psi_t_k);
	    }
	    if (bra != NULL) U_->Apply(psi_t_b);
	    ++master_clock_;
	  }
	}
    }

    DELETE_WF(psi_t_k);
    DELETE_WF(psi_t_b);

    log.cout() << "Write correlation function\n";
    /* Write correlation function to file */
    FileSingle<Cfunc> file;
    CreateMetaData(2, cfun.Params());

    file.Format(FileSingle<Cfunc>::binary);
    file.Name(dir_ + "Cfun_2");
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

    if (read_dir_.empty()) { /* propagate baseline */
      section = ContentNodes_->FindNode( "wf" );
      if (section == NULL)
	   throw ( EParamProblem ("No inital wave function found") );

      Psi0 = ChainLoader::LoadWaveFunctionChain( section );
      delete section;

      wfbuffer.Add(Psi0);
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

	if (wfbuffer.Size() < uint(Nt/wcycle) ) master_clock_.Steps(wfbuffer.Size());
	if (wfbuffer.Size() < 1 )
	  throw (EParamProblem("Wave function buffer is empty"));

	Psi0 = wfbuffer[0];
    }

    log.IndentDec();
    log.flush();

    /* Let the Propagator do it's initalisation */
    U_->Clock( &master_clock_ );
    U_->Init(Psi0);
    H_ = U_->Hamiltonian();
    H_->UpdateTime();

    /* Get defintion for bra */
    bra_first_op_ = ReadKetBra("bra", bra_);
    for (uint i=0; i < bra_.size(); i++) {
	if (bra_[i] != NULL) bra_[i]->Init(Psi0);
    }

    /* Get defintion for ket */
    ket_first_op_ = ReadKetBra("ket", ket_);
    for (uint i=0; i < ket_.size(); i++) {
       if (ket_[i] != NULL) ket_[i]->Init(Psi0);
    }

    if (bra_.size() == 0){ /* no bra means: <psi|U+ */
	num_u_ = num_u_+1;
    } else {
	if (bra_.back() != NULL)
	    throw (EParamProblem("Last operator in bra definition has to be a propagator"));
    }

    if (ket_.back() == NULL)
	throw (EParamProblem("Last operator in bra definition has to be the signal mode operator"));

    if (wfbuffer.Size() > 1 && (ket_[0] != NULL || bra_[0] != NULL))
      throw (EParamProblem("Pre-propagated time series requires a propagator as first element int the ket and bra definitions."));

    /* Run initial time series, if not already present */
    if (wfbuffer.Size() == 1){
	log.cout() << "\nRun initial propagation U(t1,0)|psi>\n"; log.flush();

	WaveFunction* psi_t = InitializeKetBra(false);

	/* fill wfbuffer with time series */
	master_clock_.Begin();
	wfbuffer.Set(0, psi_t);
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

    DELETE_WF(Psi0);

  }

} /* namespace QDLIB */

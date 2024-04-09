// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * ProgCfun.h *
 *  Created on: Nov 23, 2015
 *      Author: markus.kowalewski.uu@gmail.com
 */

#ifndef PROGCFUN_H_
#define PROGCFUN_H_

#include "tools/XmlNode.h"

#include "qdlib/OPropagator.h"
#include "qdlib/WFBuffer.h"

#define QD_CFUN_MAXINT 2
#define QD_CFUN_FNAME "Cfun"

namespace QDLIB
{
  /**
   * Calculate a multi point correlation function.
   *
   * XmlNode Attributes:
   * \li dt       time step for propagator [required]
   * \li steps    number of time steps [required]
   * \li stepsint maximum number of intermediate time steps (optional)
   * \li wcycle   write cycle. The time step in C(t) will be dt*wcycle [1]
   * \li read     read the baseline propagation (first U) from directory given by 'read' instead (optional)
   * \li fname    file name for correlation function
   */
  class ProgCfun {
    private:
      /** Dummy class for usage with FileSingle */
      class Cfunc : public cVec {
	    string name_;
	    ParamContainer pm_;
	  public:
	    Cfunc() : name_("Cfunc") {}
	    Cfunc(size_t N) : cVec(N), name_("Cfunc") {  }

	    ParamContainer& Params() { return pm_; }
	    const string& Name() { return name_; };
      };

      XmlNode &CfunNode_;
      XmlNode *ContentNodes_;
      string dir_;
      string fname_;
      string read_dir_;
      string write_dir_;

      int stepsint_;

      OPropagator* U_;
      Operator* H_;
      WaveFunction* Psi0;
      vector<Operator*> ket_;
      int ket_first_op_;
      vector<Operator*> bra_;
      int bra_first_op_;
      int num_u_;

      WFBuffer wfbuffer;

      QDClock master_clock_;
      int wcycle_;

      void InitParams_();
      int ReadKetBra(const string& name, vector<Operator*>& kb );
      void CreateMetaData(int dims, ParamContainer &pm);
      WaveFunction* InitializeKetBra(bool bra = false);
      void RunC1();
      void RunC2();
    public:
      ProgCfun(XmlNode &CfunNode);
      ~ProgCfun ();

      void SetDirectory(const string &directory) { dir_ = directory; }

      void Run();

  };

} /* namespace QDLIB */

#endif /* PROGCFUN_H_ */

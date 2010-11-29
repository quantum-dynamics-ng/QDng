#include "ProgOCT.h"

#include "tools/Logger.h"
#include "tools/fstools.h"
#include "ChainLoader.h"
#include "qdlib/Butterworth.h"
#include "qdlib/FileWF.h"

namespace QDLIB
{

   ProgOCT::ProgOCT(XmlNode &OCTNode) :
      _octNode(OCTNode), _ContentNodes(NULL), _fname(DEFAULT_BASENAME_LASER), _dir(""),
               _iterations(DEFAULT_ITERATIONS), _convergence(DEFAULT_CONVERGENCE), _writel(false),
               _method(krotov), _coupling(dipole), _ttype(ov), _phase(false),
               _ntargets(1), _mv(false), _alpha(1), _Gobbler(NULL), _opwf(NULL), _membuf_init(false)
   {
      for (int i = 0; i < MAX_TARGETS; i++) {
         PsiI[i] = NULL;
         PsiT[i] = NULL;
         _Otarget[i] = NULL;
      }
   }

   ProgOCT::~ProgOCT()
   {
      DELETE_ALL_WF();
   }

   /**
    * Initalize the global parameters.
    */
   void QDLIB::ProgOCT::_InitParams()
   {
      QDClock *clock = QDGlobalClock::Instance(); /* use the global clock */
      ParamContainer attr;
      Logger& log = Logger::InstanceRef();

      attr = _octNode.Attributes();
      double dt;
      int steps;
      string s;

      double cutoffl, cutoffh; /* Cut off frequencies for freq-shape method */

      /* Init the clock */
      attr.GetValue("dt", dt);
      attr.GetValue("steps", steps);

      if (dt == 0)
         throw(EParamProblem("Zero time step defined"));
      if (steps == 0)
         throw(EParamProblem("Zero number of time step defined"));

      clock->Dt(dt);
      clock->Steps(steps);

      /* Init OCT output dir */
      if (attr.isPresent("dir") && _dir.empty()) {
         attr.GetValue("dir", _dir);
         if (_dir[_dir.length() - 1] != '/' && !_dir.empty())
            _dir += "/";
      }
      FS::CreateDir(_dir);

      /* Init propagation file basename */
      if (attr.isPresent("fname")) {
         attr.GetValue("fname", _fname);
      }

      /* Yield file */
      attr.GetValue("writeyield", _writeyield, false);
      if (_writeyield || attr.isPresent("yfile")) {
         attr.GetValue("yfile", _yfile);
         if (!_yfile.empty())
            _yfile = _dir + _yfile;
         else _yfile = _dir + "Yield";
         _writeyield = true;
      }

      /* Optimization method to use */
      if (attr.isPresent("method")) {
         attr.GetValue("method", s);
         if (s == "krotov")
            _method = krotov;
         else if (s == "rabitz")
            _method = rabitz;
         else if (s == "rabitzfb")
            _method = rabitzfb;
         else if (s == "freq")
            _method = freq;
         else throw(EParamProblem("Unknown OCT method ", s));

      }

      /* Read the shape file */
      if (attr.isPresent("shape")) {
         string shape;
         attr.GetValue("shape", shape);
         Laser::FileLaser file = _shape[0].File();

         file.Suffix(BINARY_O_SUFFIX);
         file.Name(shape);
         file >> &(_shape[0]);
         _shape[0].Clock(clock);
      } else throw(EParamProblem("No shape file given"));

      /* Look for freq. shape params */
      if (_method == freq) {
         double cutoff;
         if (attr.isPresent("freq")) { /* Load mask from file */
            string frqmask;
            attr.GetValue("freq", frqmask);
            Laser::FileLaser file = _gamma[0].File();

            file.Suffix(BINARY_O_SUFFIX);
            file.Name(frqmask);
            file >> &(_frqmask[0]);
            if (_frqmask[0].Dt() != dt || _frqmask[0].Nt() != steps / 2 + 1)
               throw(EIncompatible("Frequency mask doesn't match dt/steps"));
         } else if (attr.isPresent("freql") || attr.isPresent("freqh")) { /* Construct mask from params */
            /* Set params */
            _frqmask[0].Nt(steps / 2 + 1);
            _frqmask[0].Dt(dt);
            ((dVec&) (_frqmask[0])) = 1.0; /* initialize with ones */

            dVec mask(steps / 2 + 1);
            /* Lower cuttoff => Highpass */
            if (attr.isPresent("freql")) {
               attr.GetValue("freql", cutoffl);
               cutoff = abs(cutoffl) * (steps - 1) * dt / (2 * M_PI); /* convert freq. => grid point */
               if (cutoff > steps / 2 + 1)
                  throw(EParamProblem("Lower frequency bound larger than Nyquist frequency"));
               Butterworth<dVec>::Highpass(mask, cutoff, 100);
               for (int i = 0; i < steps / 2 + 1; i++) {
                  (_frqmask[0])[i] *= mask[i];
               }
            }
            /* Higher cuttoff => Lowpass */
            if (attr.isPresent("freqh")) {
               attr.GetValue("freqh", cutoffh);
               cutoff = abs(cutoffh) * (steps - 1) * dt / (2 * M_PI); /* convert freq. => grid point */
               if (cutoff > steps / 2 + 1)
                  throw(EParamProblem("Upper frequency bound larger than Nyquist frequency"));
               Butterworth<dVec>::Lowpass(mask, cutoff, 100);
               for (int i = 0; i < steps / 2 + 1; i++) {
                  (_frqmask[0])[i] *= mask[i];
               }
            }
         } else throw(EParamProblem("Missing frequency shaping parameters (freq, freql or freqh)"));

      } /* if (_method == freq) */

      /* Number of iterations */
      if (attr.isPresent("iterations")) {
         attr.GetValue("iterations", _iterations);
         if (_iterations < 1)
            throw(EParamProblem("Invalid number of iterations", _iterations));
      }

      /* Check Convergence */
      if (attr.isPresent("conv")) {
         attr.GetValue("conv", _convergence);
         if (_convergence < 0)
            throw(EParamProblem("Invalid convergency criteria", _convergence));
      }

      /* Write laserfield in every step */
      if (attr.isPresent("writel"))
         attr.GetValue("writel", _writel);

      /* Type of coupling to use */
      if (attr.isPresent("coup")) {
         attr.GetValue("coup", s);
         if (s == "dipole")
            _coupling = dipole;
         else throw(EParamProblem("Unsupported coupling specified", s));
      }

      /* Target type to use */
      if (attr.isPresent("ttype")) {
         attr.GetValue("ttype", s);
         if (s == "operator")
            _ttype = op;
         else if (s == "overlap")
            _ttype = ov;
         else throw(EParamProblem("Unknown OCT target type", s));
      }

      /* Moving target */
      if (attr.isPresent("mv")) {
	attr.GetValue("mv", _mv);
      }

      /* Check for phase sensitive oct*/
      if (attr.isPresent("phase")) {
	attr.GetValue("phase", _phase);
      }

      /* Target type to use */
      if (attr.isPresent("ntargets")) {
         attr.GetValue("ntargets", _ntargets);
         if (_ntargets < 1)
            throw(EParamProblem("Number of targets to small", _ntargets));
         if (_ntargets > MAX_TARGETS)
            throw(EParamProblem("Number of targets exceeding MAX_TARGETS", _ntargets));
      }

      if (attr.isPresent("alpha")) {
         attr.GetValue("alpha", _alpha);
      }

      /* Get alpha */
      if (abs(_alpha) == 0)
         throw(EParamProblem("Invalid alpha parameter", _alpha));

      /* Print parameter summary */
      log.Header("OCT parameters", Logger::Section);

      log.IndentInc();
      log.cout() << "Number of steps: " << clock->Steps() << endl;
      log.cout().precision(2);
      log.cout() << "Time step: " << fixed << clock->Dt() << endl;
      log.cout() << "Overall time: " << fixed << clock->Steps() * clock->Dt() << endl;
      log.cout() << "Basename for wave function output: " << _fname << endl;
      log.cout() << endl;

      log.cout() << "OCT - Style: ";
      switch (_method) {
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

      switch (_ttype) {
         case ov:
            log.cout() << " - Overlapp";
            break;
         case op:
            log.cout() << " - Excpectation values";
            break;
      }

      if (_mv)
         log.cout() << " - Moving Target";

      if (_phase)
         log.cout() << " - phase sensitive" << endl;
      else log.cout() << endl;

      if (!_phase && (_ttype == op))
         log.cout() << "WARNING : OPOC + phase insensitive is not meaningfull" << endl;

      if (_method == freq) {
         log.cout() << endl << "Frequency mask parameters: ";
         log.cout().precision(8);
         log.cout() << "low = " << cutoffl << "  high = " << cutoffh << endl;
      }

      log.cout().precision(3);
      log.cout() << "alpha : " << _alpha << endl;
      log.cout() << "Number of targets: " << _ntargets << endl;

      switch (_coupling) {
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
      double res = 0;
      dcomplex im(0, 0);

      if (_opwf == NULL)
         _opwf = wfi[0]->NewInstance();

      for (int t = 0; t < _ntargets; t++) {

         /* Imaginary term */
         switch (_coupling) {
            case dipole:
               _Coup->Apply(_opwf, wfi[t]);
               break;
         }

         /* Phase sensitive */
         if (_phase)
            im = (*wft[t] * _opwf);
         else im = (*(wfi[t]) * wft[t]) * (*(wft[t]) * _opwf);

         res += im.imag();
      }
      return res;
   }

   /**
    * Calculate Laserfield for the next time step.
    */
   double ProgOCT::CalcLaserField(WaveFunction** wfi, WaveFunction** wft)
   {
      double res = 0;
      double im;

      im = CalcCorr(wfi, wft);

      /* Method */
      switch (_method) {
         case krotov: /* Add New Laser to previous field */
            res = _laserb[0]->Get();
            res -= _shape[0].Get() / (_alpha * double(_ntargets)) * im;
            _laserobj[0] += (_shape[0].Get() / _alpha * im) * (_shape[0].Get() / _alpha * im);
            break;
         case freq: /* like krotov - but subtract gamma */
            res = _laserb[0]->Get();
            res -= _shape[0].Get() / (_alpha * double(_ntargets)) * (im + _gamma[0].Get());
            _laserobj[0] += (_shape[0].Get() / _alpha * im) * (_shape[0].Get() / _alpha * im);
            break;
         case rabitz:
         case rabitzfb:
            res = -1 * _shape[0].Get() / (_alpha * double(_ntargets)) * im;
            _laserobj[0] += -1 / (double(_ntargets)) * im * res;
            break;
      }

      if (fpclassify(res) == FP_NAN)
         throw(EOverflow("Laserfield is not a number"));

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
      dcomplex opval(0, 0);
      dcomplex ov_sum(0, 0);

      /* Write Header for iteration table */
      if (iteration == 0) {
         log.SetIndent(0);
         log.Header("OCT Iterations:", Logger::SubSection);

         log.cout() << "iteration\t";
         for (int t = 0; t < _ntargets; t++) {
            log.cout() << "Norm_" << t << "\t\t";
            if (_ttype == ov) {
               log.cout() << "Norm_t" << t << "\t\t";
               log.cout() << "Overlapp_" << t << "\t";
            } else log.cout() << "Operator_" << t << "\t";
            if (_phase && _ttype == ov)
               log.cout() << "Phase_" << t << "\t\t";
         }
         log.cout() << "Laser Objective\t\t";
	 log.cout() << "Objective\t\t";
         log.cout() << "Pulse Energy" << endl;
      }

      if (_opwf == NULL)
         _opwf = wfi[0]->NewInstance();

      log.cout() << iteration << "\t\t";
      for (int t = 0; t < _ntargets; t++) {
         double d;
         log.cout() << fixed;
         log.cout().precision(8);
         d = wfi[t]->Norm();
         if (fpclassify(d) == FP_NAN)
            throw(EOverflow("Forward norm is not a number"));

         log.cout() << d << "\t";

         if (_ttype == ov) { /* Print overlapp */
            d = wft[t]->Norm();
            if (fpclassify(d) == FP_NAN)
               throw(EOverflow("Backward norm is not a number"));

            log.cout() << d << "\t";

            overlap = *wfi[t] * wft[t];

            //ov_sum += cabs(overlap)*cabs(overlap);
            log.cout() << cabs(overlap) * cabs(overlap) << "\t";
            if (_phase) {
               log.cout() << phase(overlap) / M_PI << "\t";
               ov_sum += overlap;
            } else ov_sum._real += cabs(overlap) * cabs(overlap);
         } else { /* Print operator */
            opval._real = _Otarget[t]->Expec(wfi[t]);
            ov_sum += cabs(opval);
            log.cout() << opval.real() << "\t";
         }

      }
      if (iteration == 0)
         log.cout() << "-\t" << "\t\t" << "-\t" << "\t\t";
      else {
	  log.cout() << _laserobj[0] << "\t\t";
	  log.cout() << ov_sum.real() - _laserobj[0] << "\t\t";
      }

      log.cout() << _laserf[0]->PulseEnergy() << endl;
      log.flush();

      return cabs(ov_sum);
   }

   /**
    * Propagate Forward from t= 0 => T all targets.
    * 
    * Using the _U and the corresponding Laserfield _laserf
    * membuf[0] => t=0 , membuf[Steps] => t=T
    */
   void ProgOCT::PropagateForward(WaveFunction ** wf)
   {
      QDClock *clock = QDGlobalClock::Instance(); /* use the global clock */

      for (int t = 0; t < _ntargets; t++) /* Save t=0 */
         _memwfbuf[t].Set(0, wf[t]);

      clock->Begin();
      for (int s = 0; s < clock->Steps(); s++) { /* Save t=1..T */
         for (int t = 0; t < _ntargets; t++) {
            _U->Apply(wf[t]);
            if (_Gobbler) {
               _Gobbler->Apply(wf[t]);
               wf[t]->Normalize();
            }
            _memwfbuf[t].Set(s + 1, wf[t]);
         }
         ++(*clock);
      }
   }

   /**
    * Propagate Backward from t= T => 0 all targets.
    *
    * Using the _U and the corresponding Laserfield _laserb
    * membuf[0] => t=0 , membuf[Steps] => t=T
    */
   void ProgOCT::PropagateBackward(WaveFunction **wf)
   {
      QDClock *clock = QDGlobalClock::Instance(); /* use the global clock */

      clock->End();

      /* Exchange laserfields => Switch to backward propagation */
      for (int l = 0; l < _nlaser; l++)
	 _laserb[l]->swap(*(_laserf[l]));
      
      _U->Backward();
      
      for (int t = 0; t < _ntargets; t++) /* Save t=T */
         _memwfbuf[t].Set(clock->TimeStep() + 1, wf[t]);

      for (int s = clock->TimeStep(); s >= 0; s--) {
         for (int t = 0; t < _ntargets; t++) {
            _U->Apply(wf[t]);
            if (_Gobbler) {
               _Gobbler->Apply(wf[t]);
               wf[t]->Normalize();
            }
            _memwfbuf[t].Set(s, wf[t]);
         }
         --(*clock);
      }
      
      /* Exchange laserfields => Switch back to forward propagation */
      for (int l = 0; l < _nlaser; l++)
         _laserf[l]->swap(*(_laserb[l]));
      
      _U->Forward();
   }

   /**
    * Synchronize overlap targets.
    * 
    * Writes to membuf.
    */
   void QDLIB::ProgOCT::SyncTargetOverlap(WaveFunction ** phii, WaveFunction ** phit, int step)
   {

      /* Backpropagation of targets */
      if (!_mv)
         PropagateBackward(phit);

   }

   /**
    * Synchronize operator targets.
    *
    * Writes to membuf.
    */
   void ProgOCT::SyncTargetOperator(WaveFunction ** phii, WaveFunction ** phit, int step)
   {

      /* Exchange laserfields */
      for (int l = 0; l < _nlaser; l++)
         _laserb[l]->swap(*(_laserf[l]));
      /* Propagate forward */
      PropagateForward(phit);
      /* Write Report & Calculate change */

      /* Exchange laserfields */
      for (int l = 0; l < _nlaser; l++)
         _laserb[l]->swap(*(_laserf[l]));

      /* Apply */
      for (int t = 0; t < _ntargets; t++)
         _Otarget[t]->Apply(phit[t]);

      /* Propagate Backward */
      PropagateBackward(phit);
   }

   /**
    * Iterate Krotov, Rabitz
    */
   void ProgOCT::Iterate(WaveFunction ** phii, WaveFunction ** phit, int step)
   {
      WaveFunction* bT[MAX_TARGETS];
      QDClock *clock = QDGlobalClock::Instance(); /* use the global clock */
      double laser;

      if (_ttype == ov)
         SyncTargetOverlap(phii, phit, step);
      else SyncTargetOperator(phii, phit, step);

      /* Refresh intial */
      _CopyWFs(phii, PsiI);

      clock->Begin();
      for (int s = 0; s < clock->Steps(); s++) {
         /* Get new field */
         for (int t=0; t < _ntargets; t++)
            bT[t] = _memwfbuf[t][s];

         laser = CalcLaserField(phii, bT);

         for (int l = 0; l < _nlaser; l++)
            _laserf[l]->Set(laser);

         /* Propagate initial with new field */
         for (int t = 0; t < _ntargets; t++) {
            _U->Apply(phii[t]);
            if (_Gobbler) {
               _Gobbler->Apply(phii[t]);
               phii[t]->Normalize();
            }
         }
         ++(*clock);
      }
      if (!_mv) {
         for (int t=0; t < _ntargets; t++)
            *(phit[t]) = _memwfbuf[t][clock->Steps()];
      }
   }

   /**
    * Iteration Step for the Rabitz feedback method.
    */
   void QDLIB::ProgOCT::IterateRabitzFB(WaveFunction ** phii, WaveFunction ** phit, int step)
   {
      WaveFunction* bT[MAX_TARGETS];
      QDClock *clock = QDGlobalClock::Instance(); /* use the global clock */
      double laser;

      if (step == 1) {
         PropagateForward(phii);
      }
      for (int t=0; t < _ntargets; t++)
         *(phii[t]) =  _memwfbuf[t][0];

      /* Create target */
      if (_ttype == op)
         for (int t = 0; t < _ntargets; t++)
            _Otarget[t]->Apply(phit[t], phii[t]);

      /* Propagate Target Backward with new field (1) */
      clock->End();
      for (int s = 0; s < clock->Steps(); s++) {
         /* Calc new laser field */
         for (int t=0; t < _ntargets; t++)
            bT[t] = _memwfbuf[t][clock->TimeStep() + 1];

         laser = CalcLaserField(bT, phit);


         for (int l = 0; l < _nlaser; l++)
            _laserb[l]->Set(laser);

         /* Save back steps */
         for (int t=0; t < _ntargets; t++)
            _memwfbuf[t].Set(clock->TimeStep() + 1,  phit[t]);

         /* Do one step back */
         for (int t = 0; t < _ntargets; t++) {
            //_Ub->Apply(phit[t]);
            if (_Gobbler) {
               _Gobbler->Apply(phit[t]);
               phit[t]->Normalize();
            }
         }

         --(*clock);
      }
      /* Save last  back steps */
      for (int t=0; t < _ntargets; t++)
         _memwfbuf[t].Set(0, phit[t]);

      for (int t=0; t < _ntargets; t++)
         *(phit[t]) = _memwfbuf[t][clock->Steps()];

      /* Propagate forward initial with new field (2) */
      clock->Begin();
      for (int s = 0; s < clock->Steps(); s++) {
         /* Calc new laser field */
         for (int t=0; t < _ntargets; t++)
            bT[t] = _memwfbuf[t][clock->TimeStep()];

         laser = CalcLaserField(phii, bT);

         for (int l = 0; l < _nlaser; l++)
            _laserf[l]->Set(laser);

         /* Save forward steps */
         for (int t=0; t < _ntargets; t++)
            _memwfbuf[t].Set(clock->TimeStep(), phii[t]);

         /* Do one step forward */
         for (int t = 0; t < _ntargets; t++) {
            _U->Apply(phii[t]);
            if (_Gobbler) {
               _Gobbler->Apply(phii[t]);
               phii[t]->Normalize();
            }
         }

         ++(*clock);
      }
      /* Save last forward steps */
      for (int t=0; t < _ntargets; t++)
         _memwfbuf[t].Set(clock->Steps(), phii[t]);
   }

   /**
    * Iteration Step for Krotov frequency shaping method.
    */
   void ProgOCT::IterateFreq(WaveFunction ** phii, WaveFunction ** phit, int step)
   {
      WaveFunction* bT[MAX_TARGETS];
      QDClock *clock = QDGlobalClock::Instance(); /* use the global clock */

      double laser;
      cVec *freqbuf;
      cVec *freqbufb;

      /* Shape old Laser in the frequency domain & write result to new laser */
      _laserf[0]->FastCopy(*(_laserb[0]));
      freqbuf = _laserf[0]->Spectrum();
      freqbufb = _laserb[0]->Spectrum(); /** \todo dirty hack to correct the time steps */

      for (int s = 0; s < clock->Steps() / 2 + 1; s++) { /* apply mask */
         (*freqbuf)[s] *= (_frqmask[0])[s] / clock->Steps(); /** \todo Replace with low level method */
         (*freqbufb)[s] *= (_frqmask[0])[s] / clock->Steps();
      }

      _laserf[0]->ToTimeDomain();
      _laserb[0]->ToTimeDomain();

      for (int l = 1; l < _nlaser; l++) {
         _laserf[l]-> FastCopy(*(_laserf[0]));
         _laserb[l]-> FastCopy(*(_laserb[0]));
      }

      /* Sync targets */
      if (_ttype == ov)
         SyncTargetOverlap(phii, phit, step);
      else SyncTargetOperator(phii, phit, step);

      /* Determine correction field gamma (propagation with shaped lasers)*/
      clock->Begin();
      for (int s = 0; s < clock->Steps(); s++) {
         if (!_mv)
            _gamma[0].Set(CalcCorr(phii, phit));
         else {
            for (int t=0; t < _ntargets; t++)
               bT[t] = _memwfbuf[t][s];

            _gamma[0].Set(CalcCorr(phii, bT));
         }
         for (int t = 0; t < _ntargets; t++) {
            _U->Apply(phii[t]);
            if (!_mv)
               _U->Apply(phit[t]);

            if (_Gobbler) {
               _Gobbler->Apply(phit[t]);
               _Gobbler->Apply(phii[t]);
               phii[t]->Normalize();
               phit[t]->Normalize();
            }
         }
         ++(*clock);
      }

      /* Apply Bandstop filter */
      freqbuf = _gamma[0].Spectrum();
      for (int s = 0; s < clock->Steps() / 2 + 1; s++) /* apply mask */
         (*freqbuf)[s] *= (1 - (_frqmask[0])[s]) / double(clock->Steps());

      _gamma[0].ToTimeDomain();


      /* Refresh intial */
      for (int t = 0; t < _ntargets; t++) {
         *(phii[t]) = PsiI[t];
      }

      clock->Begin();
      for (int s = 0; s < clock->Steps(); s++) {
         /* Get new field */
         for (int t=0; t < _ntargets; t++)
            bT[t] = _memwfbuf[t][s];

         laser = CalcLaserField(phii, bT);

         for (int l = 0; l < _nlaser; l++)
            _laserf[l]->Set(laser);

         /* Propagate initial with new field */
         for (int t = 0; t < _ntargets; t++) {
            _U->Apply(phii[t]);
            if (_Gobbler) {
               _Gobbler->Apply(phii[t]);
               phii[t]->Normalize();
            }
         }
         ++(*clock);
      }
   }

   /**
    * Run the OCT-Programm.
    */
   void ProgOCT::Run()
   {
      GlobalOpList& OpList = GlobalOpList::Instance();
      Logger& log = Logger::InstanceRef();
      QDClock *clock = QDGlobalClock::Instance(); /* use the global clock */
      XmlNode *section;
      Operator *_H;

      _InitParams();

      /* fetch the ChildNodes */
      _ContentNodes = _octNode.NextChild();

      log.Header("QM Initialization", Logger::Section);
      log.Header("Propagator: ", Logger::SubSection);

      /* Load & Init the propagator */
      section = _ContentNodes->FindNode("propagator");
      if (section == NULL)
         throw(EParamProblem("No propagator found"));

      _U = ChainLoader::LoadPropagator(section);
      _H = _U->Hamiltonian();
      delete section;

      /* Load Initial Wavefunctions */
      log.Header("Initial wave functions", Logger::SubSection);
      log.IndentInc();
      char num[3];
      for (int i = 0; i < _ntargets; i++) {
         snprintf(num, 3, "%d", i);
         section = _ContentNodes->FindNode("wfi" + string(num));
         if (section == NULL)
            throw(EParamProblem("Missing initial wavefunction", i));
         PsiI[i] = ChainLoader::LoadWaveFunctionChain(section);
         delete section;
      }
      log.cout() << endl;
      log.IndentDec();

      /* Initialize Buffer for back propagation */
      for (int t = 0; t < _ntargets; t++){
         _memwfbuf[t].Size(clock->Steps() + 1);
         _memwfbuf[t].Init(PsiI[t]);
         _memwfbuf[t].AutoLock(2);
      }


      if (_ttype == ov) {
         /* Load Target Wavefunctions */
         log.Header("Target wave functions", Logger::SubSection);
         log.IndentInc();
         for (int t = 0; t < _ntargets; t++) {
            snprintf(num, 3, "%d", t);
            section = _ContentNodes->FindNode("wft" + string(num));
            if (section == NULL)
               throw(EParamProblem("Missing target wavefunction", t));
            if (_mv == true) { /* Prepare series for moving targets */
               log.cout() << "\nLoading Propagation for moving target (" << t << ")\n" << endl;
               PsiT[t] = ChainLoader::LoadWaveFunctionChain(section, 0);
               _memwfbuf[t].Set(0, PsiT[t]);
               log.flush();
               log.Supress(true);
               for (int s = 1; s < clock->Steps() + 1; s++) { /* Read time steps */
                  _memwfbuf[t].Set(s, ChainLoader::LoadWaveFunctionChain(section, s));
               }
               log.flush();
               log.Supress(false);

            } else /* Load simple target */
            PsiT[t] = ChainLoader::LoadWaveFunctionChain(section);
            delete section;
         }
         if (_mv){
            for (int t = 0; t < _ntargets; t++)
               *(PsiT[t]) = _memwfbuf[t][clock->Steps()]; /* Use last step as "target" */
         }
         log.cout() << endl;
      } else if (_ttype == op) {
         log.Header("Target operators", Logger::SubSection);
         log.IndentInc();
         for (int t = 0; t < _ntargets; t++) {
            snprintf(num, 3, "%d", t);
            section = _ContentNodes->FindNode("target" + string(num));
            if (section == NULL)
               throw(EParamProblem("Missing target operator", t));
            _Otarget[t] = ChainLoader::LoadOperatorChain(section);
            _Otarget[t]->Clock(clock);
            OpList.Init(_Otarget[t], PsiI[0]);
            delete section;
         }
      }

      log.IndentDec();

      /* Make sure our hamiltonian is initalized */
      _H->Clock(clock);
      OpList.Init(_H, PsiI[0]);
      log.cout() << "Initial energy (first initial wf): " << _H->Expec(PsiI[0]) << endl;
      if (_ttype == ov) {
         log.cout() << "Initial energy (first target wf): " << _H->Expec(PsiT[0]) << endl;
         log.cout() << "Initial Overlapp: " << *(PsiI[0]) * (PsiT[0]) << endl;
      }
      log.flush();

      /* Check & reference the coupling operator */
      bool coupling_ok = false;
      int nlasers = MAX_LASERS;
      string label("OptLaser");
      switch (_coupling) {
         case dipole:
            OLaser *CoupOLaser[MAX_LASERS];
            
            _Coup = FindOperatorType<OLaser> (_H, CoupOLaser, nlasers, &label); /* Try first to find labeled Ops. */
            if (nlasers == 0){
               nlasers = MAX_LASERS;
               _Coup = FindOperatorType<OLaser> (_H, CoupOLaser, nlasers); /* Otherwise use all unlabeled Ops. */
            } else
               log.cout() << "Found " << nlasers << " labeled laser field(s)\n";
            
            _nlaser = nlasers;
            if (nlasers != 0) {
               for (int l = 0; l < _nlaser; l++){
                  _laserf[l] = CoupOLaser[l]->GetLaser();
                  _laserb[l] = new Laser();
	          *(_laserb[l]) = *(_laserf[l]);
	       }
               coupling_ok = true;
            }
            break;
         default:
            throw(EIncompatible("Unknown coupling type"));
      }

      if (!coupling_ok)
         throw(EParamProblem("Given type of coupling operator not found in hamiltonian"));

      section = _ContentNodes->FindNode("coupling");
      if (section == NULL)
         throw(EParamProblem("No labeled coupling operator found in hamiltonian"));

      log.Header("Coupling operator", Logger::SubSection);
      log.IndentInc();
      _Coup = ChainLoader::LoadOperatorChain(section);
      if (_Coup == NULL)
         throw(EParamProblem("No coupling operator"));

      log.IndentDec();

      /* Check the coupling operator */
      switch (_coupling) {
         case dipole:
            OGridPotential* test[MAX_LASERS];
            nlasers = MAX_LASERS;
            FindOperatorType<OGridPotential> (_Coup, test, nlasers);
            for (int l = 0; l < nlasers; l++) {
               if (test[l] == NULL)
                  throw(EParamProblem("Invalid coupling operator"));
            }
            break;
      }

      /* Unset unity in Multistate => Otherwise Operator will be wrong */
      OMultistate* ms;
      ms = dynamic_cast<OMultistate*> (_Coup);
      if (ms != NULL) {
         log.coutdbg() << "Switching of unity\n";
         ms->Unity(false);
      }

      _Coup->Clock(clock);
      delete section;

      OpList.Init(_Coup, PsiI[0]);

      /* Load the Gobbler */
      section = _ContentNodes->FindNode("gobbler");
      if (section != NULL) {
         log.cout() << endl;
         log.Header("Loading gobbler", Logger::SubSection);
         log.IndentInc();
         _Gobbler = ChainLoader::LoadOperatorChain(section);
         log.IndentDec();
	 OpList.Init(_Gobbler, PsiI[0]);
         delete section;
      }

      /* Let the Propagator do it's initalisation */
      _U->Clock(clock);
      
      _H->UpdateTime();
      OpList.Init(_U, PsiI[0]);

      /* Report what the propagator has chosen */
      ParamContainer Upm;

      Upm = _U->Params();
      log.cout() << "Forward Propagators init parameters:\n\n" << Upm << endl;
      log.coutdbg() << _U->Exponent() << endl;

      /* Objects for propagation */
      WaveFunction* phii[MAX_TARGETS];
      WaveFunction* phit[MAX_TARGETS];

      /* Init propa buffers */
      for (int i = 0; i < _ntargets; i++) {
         phii[i] = PsiI[i]->NewInstance();
         if (_ttype == ov)
            phit[i] = PsiT[i]->NewInstance();
         else phit[i] = PsiI[i]->NewInstance();
      }

      /* Prepare laserfile writer */
      Laser::FileLaser file = _laserf[0]->File();
      file.Suffix(BINARY_O_SUFFIX);
      file.Name(_dir + _fname);
      if (_writel) {
         file.ActivateSequence();
         file << _laserf[0];
      }

      /* debuging */
      FileWF wfile;
      wfile.Name(_dir + string("wfb"));
      wfile.ActivateSequence();

      /* Initialize gamma for freq. shape */
      if (_method == freq) {
         _gamma[0].Nt(clock->Steps());
         _gamma[0].Dt(clock->Dt());
         _gamma[0].Clock(clock);
      }

      /* Write Report for guess */
      _CopyWFs(phii, PsiI);
      _laserobj[0] = 0;
      log.cout() << "Propagating forward to check initial guess\n\n";
      log.flush();
      PropagateForward(phii);
      Report(phii, PsiT, 0);

      /* Redirection to file */
      if (_writeyield) {
         log.cout() << "Yield table is redirected to file: " << _yfile << endl;
         log.flush();
         log.FileOutput(_yfile);
      }

      /* Iteration loop */
      int i = 1;
      double deltaTarget[] = { 1, 1, 1 }; /* buffer for convergency */
      double delta = 1;
      while (i <= _iterations && delta > _convergence) {
         /* Init with fresh wfs */
         if ((_method == rabitzfb && i == 1) || _method != rabitzfb) {
            _CopyWFs(phii, PsiI);
            if (_ttype == ov)
               _CopyWFs(phit, PsiT);
            else _CopyWFs(phit, PsiI);
         }

         _laserobj[0] = 0;

         /* The iteration step itself depends on the method */
         if (_method == rabitzfb)
            IterateRabitzFB(phii, phit, i);
         else if (_method == freq)
            IterateFreq(phii, phit, i);
         else {
            Iterate(phii, phit, i);
         }
         _laserobj[0] *= clock->Dt();

         /* Check for convergency  & Write Report */
         for (int it = 0; it < 3 - 1; it++)
            deltaTarget[it] = deltaTarget[it + 1];

         deltaTarget[2] = abs(Report(phii, phit, i));
         delta = 0;

         for (int it = 0; it < 3 - 1; it++)
            delta += abs(deltaTarget[it] - deltaTarget[it + 1]);

         delta = abs(delta);

         /* Write laserfields */
         if (_writel) {
            file << _laserf[0];
         }

         /* Exchange laserfields (New<->Old) */
         for (int l = 0; l < _nlaser; l++)
            _laserb[l]->swap(*(_laserf[l]));

         log.flush();
         i++;
      } /* while (i < _iterations && delta > _convergence) */
      if (_writeyield)
         log.FileClose();

      if (delta <= _convergence)
         log.cout() << endl << "Convergence reached" << endl;

      /* Write the final laser */
      file.StopSequence();
      file << _laserb[0];

      /* Remove tmp WFs */
      for (int i = 0; i < _ntargets; i++) {

         DELETE_WF(phii[i]);
         DELETE_WF(phit[i]);
      }

   }

} /* namespace QDLIB */


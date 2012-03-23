/*
 * OGridGStokes.cpp
 *
 *  Created on: 28.02.2012
 *      Author: mko
 */

#include "OGridGStokes.h"
#include "OGridPosition.h"

/* DBG */
#include "tools/Logger.h"

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridGStokes)



   OGridGStokes::OGridGStokes() : GridSystem(1), _name("OGridGStokes"), _G(NULL), _etaR0(0)
   {
      for (int i=0; i < MAX_DIMS; i++){
//         _p[i] = NULL;
         _x0[i]  = NAN;
         _slope[i] = NAN;
         for (int j=0; j < MAX_DIMS; j++){
            _GnnG[i][j] = NULL;
         }
      }

   }

   OGridGStokes::~OGridGStokes()
   {
      for (int i=0; i < OList::Size(); i++)
         DELETE_OP(Get(i));

      for (int i=0; i < MAX_DIMS; i++){
         for (int j=0; j < MAX_DIMS; j++){
            if (_GnnG[i][j] != NULL) DELETE_OP(_GnnG[i][j]);
         }
      }
   }

   void OGridGStokes::Init(ParamContainer &params)
   {
      _params = params;
      /* Stokes parameters */
      if ( !params.isPresent("eta") )
         throw(EParamProblem("Stokes operators needs a stokes parameters"));

      if ( !params.isPresent("R") )
         throw(EParamProblem("Stokes operators needs a stokes radius"));

      double val;
      params.GetValue("eta", val);
      _etaR0 = -6 * M_PI * val;
      params.GetValue("R", val);
      _etaR0 *= val;

      /* Number of dims */
      if ( params.isPresent("dims") ){
         int dims;
         params.GetValue("dims", dims);
         Dim(dims);
      }

      /* Get params for shape function */
//      for (int i=0; i < Dim(); i++){
//         int res = 8;
//         char key[8];
//
//         res = snprintf(key, res=8, "x0%d", i);
//         if ( params.isPresent(key) )
//            params.GetValue(key, _x0[i]);
//         else
//            throw(EParamProblem("GridGStokes: Missing centre value x0 for sigmoid function"));
//
//         res = snprintf(key, res=8, "slope%d", i);
//         if ( params.isPresent(key) )
//            params.GetValue(key, _slope[i]);
//         else
//            _slope[i]=0;
//      }

      /* Prepage for GnnG matrix */
      if ( !params.isPresent("gnng") )
         throw(EParamProblem("GridGStokes: The filename for the GnnG Matrix is missing"));

      string base;
      params.GetValue("gnng", base);

      OGridSystem::FileOGrid file;
      file.Suffix(BINARY_O_SUFFIX);

      /* Read GnnG matrix */

      for (int i=0; i < Dim(); i++){ /* Loop over matrix elem. */
         for (int j=0; j < Dim(); j++){
            stringstream ss;
            ss << base << "_" << i << j;
            file.Name(ss.str());
            _GnnG[i][j] = new OGridPotential();
            file >> ((OGridSystem*) _GnnG[i][j]);
         }
      }

      /* Create Momentum Operators */
      for (int i=0; i < Dim(); i++){
         OList::Add( new OGridNabla(true, i) );
      }
      /* Create Position Operators */
      for (int i=0; i < Dim(); i++){
         OList::Add( new OGridPosition(i) );
      }


   }

   void OGridGStokes::Init(WaveFunction *Psi)
   {
      WFGridSystem *psi;

      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
         throw(EIncompatible("GridGStokes: Needs a WFGridSystem"));

      /* Last Operator in list must be G-Matrix provided by user input */
//      if (OList::Size() != Dim() + 1)
//         throw(EParamProblem("OGridGStokes: G-Matrix operator is missing"));
//
//      _G = dynamic_cast<OGridGMat*>(Get(Dim()));
//      if (_G == NULL)
//         throw(EParamProblem("OGridGStokes: Operator is not a G-Matrix"));

      /* Copy grid parameters */
      *((GridSystem*) this) = *((GridSystem*) psi);
//
//      /* Init shape function */
//      _s.newsize(GridSystem::Size());
//
//      /* create X exponent */
//      for (int i=0; i < Dim(); i++){
//         dVec X(GridSystem::Size());
//         dVec x(GridSystem::DimSize(i));
//         VectorView<double> view(X, Dim(), DimSizes());
//
//         for (int j=0; j < GridSystem::DimSize(i); i++)
//            x[j] = double(j);
//
//         x -= _x0[i];
//         x *= - _slope[i];
//
//         view.ActiveDim(i);
//         view = x;
//
//         _s += X;
//      }
//
//      /* Create sigmoid */
//      for (int i=0; i < GridSystem::Size(); i++)
//         _s[i] = 1./(1+exp(_s[i]));
//
//
//      /* Create integrated shape functions for position operators */
//      for (int i=0; i < Dim(); i++){
//         _sInt[i].newsize(GridSystem::Size());
//         dVec X(GridSystem::Size());
//         dVec x(GridSystem::DimSize(i));
//         VectorView<double> view(_sInt[i], Dim(), DimSizes());
//
//         for (int j=0; j < GridSystem::DimSize(i); i++)
//            x[j] = double(j);
//
//         x -= _x0[i];
//
//         view.ActiveDim(i);
//         view = x;
//      }

      _etaR0 *= Operator::Clock()->Dt();

      OList::Init(Psi);
   }

   void OGridGStokes::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      Logger& log = Logger::InstanceRef();

      /* <p>*/
      _p.newsize(Dim());
      int i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
      for (i=0; i < Dim(); i++){
         _p[i] = Get(i)->Expec(sourcePsi);
         log.coutdbg() << "\tCalc momentum, dim = " << i << " <p> = " <<  _p[i] << endl;
         log.flush();
      }

      /* Delta pB(q) = -pi * eta * R0 (G-1 n nT G) p */
      _Deltap.newsize(GridSystem::Size()*Dim(),Dim());
      for (i=0; i < Dim(); i++){
         double* deltap = _Deltap.begin(i);

         /* first (col * row) */
         double* gnng = _GnnG[i][0]->begin(0);
         for (int k=0; k < GridSystem::Size(); k++){
            deltap[k] = _etaR0 * gnng[k] * _p[0];
         }

         /* rest of (col * row) */
         for (int j=1; j < Dim(); j++){
            gnng = _GnnG[i][j]->begin(0);
            for (int k=0; k < GridSystem::Size(); k++){
               deltap[k] += _etaR0 * gnng[k] * _p[j];
            }
         }
      }

      if ( _p.Norm() < _Deltap.Norm())
         log.cout() << "Warning: Stokes momentum larger than existing momentum!\n";


      /* Delta pB(q) dot q*/
      _exponent.newsize(GridSystem::Size());
      double* kq = _exponent.begin(0);
      double* q = dynamic_cast<OGridSystem*>(Get(Dim()))->begin(0); /* Position Operator */
      double* deltap = _Deltap.begin(0);
      for (int k=0; k < GridSystem::Size(); k++){
         kq[k] =  deltap[k] * q[k];
      }

      for (int i=1; i < Dim(); i++){
         q = dynamic_cast<OGridSystem*>(Get(i+Dim()))->begin(0);  /* Position Operator */
         deltap = _Deltap.begin(i);
         for (int k=0; k < GridSystem::Size(); k++){
            kq[k] += deltap[k] * q[k];
         }
      }

      /* e^(i Delta pB(q) * q */
      ExpElements( (cVec*) destPsi, &_exponent, dcomplex(0,1) );
      MultElements( (cVec*) destPsi, (cVec*) sourcePsi);

      if (log.Debug()){

         for (int i=0; i < Dim(); i++){
            _p[i] = Get(i)->Expec(destPsi);
            log.coutdbg() << "\tCalc momentum, dim = " << i << " <p> = " <<  _p[i] << endl;
            log.flush();
         }
      }
   }

   void OGridGStokes::Apply(WaveFunction *Psi)
   {
      WaveFunction* buf;
      buf = Psi->NewInstance();
      *buf = Psi;

      Apply(Psi, buf);
      DELETE_WF(buf);
   }

   Operator* OGridGStokes::operator=(Operator* O)
   {
      return Copy(O);
   }

   Operator* OGridGStokes::Copy(Operator* O)
   {

      OGridGStokes* o = dynamic_cast<OGridGStokes*>(O);

      if (o == NULL )
         throw ( EIncompatible("OGridStokes: Can't copy ", O->Name()) );

      throw( EIncompatible("OGridStokes: Copy not implemented") );

      OList::Copy(O);
      return this;
   }

   bool OGridGStokes::Valid(WaveFunction *Psi)
   {
      return OList::Valid(Psi);
   }

} /* namespace QDLIB */


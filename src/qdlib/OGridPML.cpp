/*
 * OGridPML.cpp
 *
 *  Created on: Jun 18, 2012
 *      Author: markus
 */

#include "qdlib/OGridPML.h"
#include "tools/helpers.h"
#include "qdlib/WFGridSystem.h"

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridPML)

   OGridPML::OGridPML() : _name("OGridPML"), _mass(MAX_DIMS), _buf1(NULL), _buf2(NULL)
   {
      Diff::Collective(false);
      Diff::Single(true);
      Diff::Mixed(false);
      Diff::Derivative(1);
      Diff::Method(FFT);
   }

   OGridPML::~OGridPML()
   {
      DELETE_WF(_buf1);
      DELETE_WF(_buf2);
   }

   void OGridPML::Init(ParamContainer& params)
   {
      _params = params;

      /* Get masses */
      vector<double> vec;
      _params.GetArray("mass", vec);
      ConvertArray(vec, _mass);
   }

   void OGridPML::Init(WaveFunction* Psi)
   {
      if (_buf1 != NULL) return; /* Avoid init twice */

      WFGridSystem* psi;

      psi = dynamic_cast<WFGridSystem*>(Psi);

      if (psi == NULL)
         throw( EWrongClassType("WFGridSystem", Psi->Name()) );


      *((GridSystem*) this) = *((GridSystem*) psi);

      Diff::SetGrid(*this);
      Diff::InitParams(_params);
      Diff::_InitKspace();

      for (int i=0; i < GridSystem::Dim(); i++){
         _pml[i].SetGrid(*this);
         _pml[i].InitParams(_params, i);
      }

      _buf1 = Psi->NewInstance();
      _buf1->Retire();
      _buf2 = Psi->NewInstance();
      _buf2->Retire();

   }

   dcomplex OGridPML::Emax()
   {
      /* Calc Tmax on the Grid */
      double T = 0;
      for (int i = 0; i < GridSystem::Dim(); i++)
         if (_mass[i] > 0)
            T += 1 / (_mass[i] * GridSystem::Dx(i) * GridSystem::Dx(i));

      T *= (M_PI * M_PI / 2);
      return dcomplex(T);
   }

   dcomplex OGridPML::Emin()
   {
      //cVec f(_pml[0].Layer());
      dcomplex sum(0);
      double kmax = M_PI/GridSystem::Dx(0);
      double gamma = _pml[0].gamma();
      double smax = _pml[0].sigma();
//      double p = double(_pml[0].p());
//      double d = _pml[0].Layer();
//      for (int i=0; i < d; i++){
//         dcomplex pml = _pml[0].f1()[i];
//         sum += -1 * cexpI(gamma) * p * pow(double(i), p-1) / pml / pml / pml / pow(d,p) + I * kmax / pml / pml;
//      }
//
//      sum *= - GridSystem::Dx(0) *  I * kmax / 2. / _mass[0] / d;
//

      cout << "emin: " << (kmax*kmax/2/_mass[0] /  (1+smax*cexpI(gamma)))<<endl;
      cout << -Emax().real()/2<<endl;
      return dcomplex(0, (kmax*kmax/2/_mass[0] /  (1+smax*cexpI(gamma))).imag() );  /* Crude approximation */
   }

   void OGridPML::Apply(WaveFunction* destPsi, WaveFunction* sourcePsi)
   {
      _buf1->Reaquire();
      _buf2->Reaquire();

      *destPsi = 0.0;
      for (int i=0; i < GridSystem::Dim(); i++){
         DnDxn(_buf1, sourcePsi, i);
         _pml[i].ApplyTransform(_buf1);
         DnDxn(_buf2, _buf1, i, -1./2./_mass[i]);
         _pml[i].ApplyTransform(_buf2);
         *destPsi += _buf2;
      }
      _buf1->Retire();
      _buf2->Retire();
   }

   Operator* OGridPML::operator =(Operator* O)
   {
      return OGridPML::Copy(O);
   }

   Operator* OGridPML::Copy(Operator* O)
   {
      DELETE_WF(_buf1);
      DELETE_WF(_buf2);

      OGridPML* o = dynamic_cast<OGridPML*>(O);
      if (o == NULL)
         throw(EWrongClassType("OGridPML", O->Name()));

      _buf1 = NULL;
      _buf2 = NULL;
      _params = o->_params;
      _mass = o->_mass;
      Init(o->_buf1);

      return this;
   }

   bool OGridPML::Valid(WaveFunction* Psi)
   {
      WFGridSystem* psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL) return false;

      if ( *((GridSystem*) this) != *((GridSystem*) psi) ) return false;

      return true;
   }

} /* namespace QDLIB */

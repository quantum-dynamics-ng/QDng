// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "OLaser.h"

namespace QDLIB {

   OLaser::OLaser() : _numlasers(1), _Pol(NULL), _UsePol(false)
   {
      _laser = new Laser[1];
      _isTimedependent = true;
   }
   
   QDLIB::OLaser::OLaser(int num) :  _numlasers(num), _Pol(NULL), _UsePol(false)
   {
      _laser = new Laser[num];
      _isTimedependent = true;
   }
   
   OLaser::~OLaser()
   {
      delete[] _laser;
      if (_Pol != NULL) delete[] _Pol;
   }

   void OLaser::Clock( QDClock * cl )
   {
      clock = cl;
      
      /* Generate Laser */
      if (_genParams.isPresent("gen")) {
         string type;
         _genParams.GetValue("gen", type);
         
         if (type == "gauss" || type == "gauss2"){
            /* Init pulse parameters */
            double Emax=0.0001;
            double t0 = cl->Steps()/2 * cl->Dt();
            double sigma = cl->Steps()/8 * cl->Dt();
            double w0 = 0.05695; // lambda = 800nm
            double phi = 0;
            double b = 0;        // Quadratic chirp
                           
            if (_genParams.isPresent("Emax"))
               _genParams.GetValue("Emax", Emax);
                  
            if (_genParams.isPresent("t0"))
               _genParams.GetValue("t0", t0);
      
            if (_genParams.isPresent("sigma"))
               _genParams.GetValue("sigma", sigma);
      
            if (_genParams.isPresent("w0"))
               _genParams.GetValue("w0", w0);
                  
            if (_genParams.isPresent("phi"))
               _genParams.GetValue("phi", phi);
      
            if (_genParams.isPresent("b"))
               _genParams.GetValue("b", b);
      
            /* Init Laser */
            Laser *Et = GetLaser();
            Et->Dt(cl->Dt());
            Et->Nt(cl->Steps());
            Et->Clock(cl);
                  
            for (int i=0; i < Et->size(); i++){
               double t = i * cl->Dt();
               (*Et)[i] = Emax * exp(-(t-t0)*(t-t0) / 2 / (sigma*sigma) ) * cos(w0 * (t-t0) + phi + b*(t-t0)*(t-t0)) ;
            }
            
            /* Generate 2. pulse */
            if (type == "gauss2"){
               Emax=0.0001;
               t0 = cl->Steps()/2 * cl->Dt();
               sigma = cl->Steps()/8 * cl->Dt();
               w0 = 0.05695; // lambda = 800nm
               phi = 0;
               b = 0;        // Quadratic chirp
                           
               if (_genParams.isPresent("Emax2"))
                  _genParams.GetValue("Emax2", Emax);
                  
               if (_genParams.isPresent("t02"))
                  _genParams.GetValue("t02", t0);
      
               if (_genParams.isPresent("sigma2"))
                  _genParams.GetValue("sigma2", sigma);
      
               if (_genParams.isPresent("w02"))
                  _genParams.GetValue("w02", w0);
                  
               if (_genParams.isPresent("phi2"))
                  _genParams.GetValue("phi2", phi);
      
               if (_genParams.isPresent("b2"))
                  _genParams.GetValue("b2", b);
               
               for (int i=0; i < Et->size(); i++){
                  double t = i * cl->Dt();
                  (*Et)[i] += Emax * exp(-(t-t0)*(t-t0) / 2 / (sigma*sigma) ) * cos(w0 * (t-t0) + phi + b*(t-t0)*(t-t0)) ;
               }

            }
         }
         
         /* Write generated laser to file */
         if (_genParams.isPresent("lasero")){
            string fname;
            Laser::FileLaser file = GetLaser()->File();
            
            _genParams.GetValue("lasero", fname);
            file.Suffix(BINARY_O_SUFFIX);
            file.Name(fname);
            file << GetLaser();
         }
      } else { /* Init existing laser with clock */
         for (int i=0; i < _numlasers; i++)
            GetLaser(i)->Clock(cl);
      }

      /* Set scaling factor */
      if ( _params.isPresent("scale") ){
         double s;
         _params.GetValue("scale", s);
         for (int i=0; i < _numlasers; i++)
            GetLaser(i)->Scale(s);
      }
   }
   
   void OLaser::Init(ParamContainer &params)
   {
      /* Read the laser field */
      if (_numlasers > 1) {
         for (int i=0; i < _numlasers; i++) {
            stringstream id;
            string name;
            Laser::FileLaser file = GetLaser(i)->File();
            id << "laser" << i;
            if ( params.isPresent(id.str()) ) /* Try laser# */
               params.GetValue(id.str(), name);
            else { /* Otherwise try default "laser" */
               if (! params.isPresent("laser") )
                  throw( EParamProblem("Missing laser field") );
               else
                 params.GetValue("laser", name);
            }
            file.Suffix(BINARY_O_SUFFIX);
            file.Name(name);
            file >> GetLaser(i);
         }
      } else if (params.isPresent("laser")){
         string name;
         Laser::FileLaser file = GetLaser()->File();

         params.GetValue("laser", name);
         file.Suffix(BINARY_O_SUFFIX);
         file.Name(name);
         file >> GetLaser();
      } else if (params.isPresent("gen")) {
         string type;
         params.GetValue("gen", type);
         
         _genParams = params;
         
         if (type == "gauss" || type == "gauss2"){
         } else
            throw (EParamProblem("Unknown laser type", type));
         
      } else
         throw (EParamProblem("No laser given"));
   }
   
   /**
    * This decides wether the to use polarisation vectors or not.
    * 
    * All former vectors will be destroyed.
    */
   void OLaser::UsePol(bool use)
   {
      _UsePol = use;
      
      if (use){
	 if (_Pol != NULL) delete[] _Pol;
	 _Pol = new dVec[_numlasers];
      }
   }


   /**
    * Set the number of laser fields.
    * 
    * All previously contained fields will be destroyed.
    */
   void OLaser::NumLaser(int num)
   {
      delete[] _laser;
      if (_Pol != NULL) delete[] _Pol;
      
      _laser = new Laser[num];
      if (_UsePol) _Pol = new dVec[num];
      
      _numlasers = num;
   }

   /**
    * Set Laser (index=0).
    */
   void OLaser::SetLaser(Laser & laser)
   {
      SetLaser(0, laser);
   }

   /**
    * Set Laser with index n.
    */
   void OLaser::SetLaser(int n, Laser & laser)
   {
      _laser[n] = laser;
   }
   
   
   void OLaser::SetPol(dVec & pol)
   {
      SetPol(0, pol);
   }

   void OLaser::SetPol(int n, dVec & pol)
   {
      _Pol[n] = pol;
   }
   
}







/*
 * fftglobal.cpp
 *
 *  Created on: 29.12.2010
 *      Author: Markus Kowalewski
 */

extern "C" {
#include "fftw3.h"
}


#include "tools/Logger.h"
#include "tools/GlobalParams.h"

#include "FFTGlobal.h"

namespace QDLIB {

   /**
    * Default constructor.
    */
   FFTGlobal::FFTGlobal()
   {
      ParamContainer& gp = GlobalParams::Instance();
      Logger& log = Logger::InstanceRef();

#ifdef _OPENMP
      /* Init threads */
      _nthreads = omp_get_max_threads();

      if (fftw_init_threads() == 0)
          cerr << "FFTW init thread error" << endl;

      fftw_plan_with_nthreads(_nthreads);
#endif

      /* Check optimization strategy for ND-transforms */
      gp.GetValue("fftwOptimize", _fftwOptimize, true);

      if (_fftwOptimize)
         _fftwFlag = FFTW_PATIENT;
      else
         _fftwFlag = FFTW_MEASURE;


      /* Read wisdom */
      string wisdom("wisdom"); /* This is the default: wisdom in the current directory */

      if (gp.isPresent("wisdom"))
         gp.GetValue("wisdom", wisdom);

      /* Read wisdom */
      FILE * pFile;
      if ((pFile = fopen(wisdom.c_str(), "r"))) {
         if (fftw_import_wisdom_from_file(pFile) == 1)
            log.cout() << "Reading fftw3 wisdom from file: " << wisdom << endl;
         else
            log.cout() << "Reading fftw3 wisdom from file: " << wisdom << " failed!" << endl;

         fclose(pFile);
         _fftwFlag = FFTW_MEASURE;
      } else {
         if (_fftwOptimize) { /* Optimal fftw plan */
            _fftwFlag = FFTW_PATIENT;
            log.cout() << "No wisdom found. Creating an optimal plan. This can take a while..."
                     << endl;
         } else {
            _fftwFlag = FFTW_ESTIMATE;
         }
      }
      log.flush();
   }


   FFTGlobal::~FFTGlobal()
   {
      Logger& log = Logger::InstanceRef();
      ParamContainer& gp = GlobalParams::Instance();
      string wisdom("wisdom"); /* This is the default: wisdom in the current directory */

      /* Save wisdom to file */
      if ( gp.isPresent("wisdomsave") )
         gp.GetValue("wisdomsave", wisdom);

      FILE * pFile;
      if ((pFile = fopen(wisdom.c_str(), "w"))){
         log.cout() << "Write fftw3 wisdom to file: " << wisdom << endl;
         fftw_export_wisdom_to_file(pFile);
         fclose(pFile);
      }

      /* Cleanup memory */
#ifdef _OPENMP
      //fftw_cleanup_threads();
#else
      //fftw_cleanup();
#endif
   }

   /**
    * Return reference to singleton instance.
    */
   FFTGlobal& FFTGlobal::Instance()
   {
      static FFTGlobal instance;
      return instance;
   }
}


#ifndef QDLIBPEAKPICKER_H
#define QDLIBPEAKPICKER_H


#include <vector>
#include "math/typedefs.h"

namespace QDLIB
{

   /**
    * Find peaks in a spectrum.
    * 
    * The Finder tries to reduce the sample to seperated regions
    * which are above treshold (mean+sigma*tol). Inside a region
    * it looks for the maximum value.
    */
   class PeakFinder
   {
      private:
         double _tol;
         double _mean;
         double _sigma;
         vector<int> _peaks;
         vector<int>::iterator _it;
      public:
         PeakFinder();
         ~PeakFinder();
         
         /**
          * Set the tolerance for the peak finder.
          * 
          * The value is in units of sigma
          */
         void Tolerance(double tol) {_tol = tol; }
         
         void Find(dVec &data);
         
         /** Returns the number of found peaks. */
         int NumPeaks() { return _peaks.size(); }
         
         /** The mean value of data. */
         double Mean() { return _mean; }
         
         /** Standard deviation of the data */
         double Sigma() { return _sigma; }
         
         int NextPeak();
         bool End();
         void Reset();
   };

}

#endif

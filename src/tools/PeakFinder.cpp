#include "PeakFinder.h"

namespace QDLIB
{

   PeakFinder::PeakFinder() :  _tol(1), _mean(0), _sigma(0)
   {
   }
   
   
   PeakFinder::~PeakFinder()
   {
   }
   
   
   /**
    * Find the peaks in data.
    * 
    * The peaks are saved internally and can be accessed laster on.
    */
   void PeakFinder::Find(dVec &data)
   {
      double mean = 0;
      double sigma = 0;
      int size = data.size();
      
      _peaks.clear();
      
      /* Calculate mean value */
      for (int i=0; i < size; i++){
         mean += data[i];
      }
      mean /= double(size);
  
      /* Calculate standard deviation */
      for (int i=0; i < size; i++){
         sigma += (data[i]-mean) * (data[i]-mean);
      }
      sigma = sqrt(1./double(size) * sigma);
      
      /* Find regions above mean+toll*simga */
      bool start=false;
      double dmax=0;
      int indmax;
      for (int i=0; i < size; i++){
         if (!start &&  data[i] > mean+_tol*sigma ) { /* indicate start of region */
            start = true; 
            dmax = 0;
            indmax = -1;
         }
         if (start &&  data[i] < mean+_tol*sigma ) { /* Close region */
            start=false;
            _peaks.push_back(indmax);
         }
         if (start) {
            if (data[i] > dmax) { /* Look for maximum inside peak region */
               dmax = data[i];
               indmax = i;
            }
         }
      }
      /* Close region - at the end of the array if the last value is not the maximum in  the region */
      if (start &&  indmax != -1 && data[size-1] < dmax) {
	 start=false;
	 _peaks.push_back(indmax);
      }
      
      _mean = mean;
      _sigma  = sigma;
      _it = _peaks.begin();
   }

   
   /**
    * Return the next index of a peak series.
    */
   int PeakFinder::NextPeak()
   {
      lint value = *_it;
      _it++;
   
      return value;
   }

   /**
    * Indicates that there is no next element.
    */
   bool PeakFinder::End()
   {
      if (_it >= _peaks.end()) return true;
      else return false;
   }

   /**
    * Set pointer to the peak storage to the beginning.
    */
   void PeakFinder::Reset()
   {
      _it = _peaks.begin();
   }

}







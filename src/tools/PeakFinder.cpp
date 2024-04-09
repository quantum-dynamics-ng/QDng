// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "PeakFinder.h"
#include <list>

namespace QDLIB
{

   PeakFinder::PeakFinder() :  _tol(1), _mean(0), _sigma(0)
   {
   }
   
   
   PeakFinder::~PeakFinder()
   {
   }
   
   double PeakFinder::_Mean(dVec &data, uint begin, uint end)
   {
      double mean = 0;
      /* Calculate mean value */
      for (uint i = begin; i < end; i++)
      {
         mean += data[i];
      }
      mean /= double(end-begin);

      return mean;
   }

   double PeakFinder::_Sigma(dVec &data, uint begin, uint end, double mean)
   {
      double sigma = 0;
      /* Calculate standard deviation */
      for (uint i = begin; i < end; i++)
      {
         sigma += (data[i] - mean) * (data[i] - mean);
      }
      sigma = sqrt(1. / double(end-begin) * sigma);

      return sigma;
   }

   void PeakFinder::_FindData(dVec &data, uint begin, uint end)
   {
      double mean = _Mean(data, begin, end);
      double sigma = _Sigma(data, begin, end, mean);
      int size = end-begin;

      /* Find regions above mean+toll*simga */
      bool start = false;
      double dmax = 0;
      int indmax;
      for (uint i = begin; i < end; i++)
      {
         if (!start && data[i] > mean + _tol * sigma)
         { /* indicate start of region */
            start = true;
            dmax = 0;
            indmax = -1;
         }
         if (start && data[i] < mean + _tol * sigma)
         { /* Close region */
            start = false;
            _peaks.push_back(indmax);
         }
         if (start)
         {
            if (data[i] > dmax)
            { /* Look for maximum inside peak region */
               dmax = data[i];
               indmax = i;
            }
         }
      }
      /* Close region - at the end of the array if the last value is not the maximum in  the region */
      if (start && indmax != -1 && data[size - 1] < dmax)
      {
         start = false;
         _peaks.push_back(indmax);
      }
   }


   /**
    * Find the peaks in data.
    *
    * The peaks are saved internally and can be accessed laster on.
    */
   void PeakFinder::Find(dVec &data)
   {

      _peaks.clear();

      _FindData(data, 0, data.size());
      
      _mean = _Mean(data, 0, data.size());
      _sigma  = _Sigma(data, 0, data.size(), _mean);
      _it = _peaks.begin();
   }

   /*
    * Find the peaks, but apply a inside a given window to find smaller maximas better
    */
   void PeakFinder::Find(dVec &data, uint winsize)
   {
      _peaks.clear();
      uint size = data.size();


      /* Look for local maxima in the data */
      set<uint>* maxi = Maximas(data);
      set<uint>::iterator it;

      /* Check the local maxima for significance */
      for (it = maxi->begin(); it != maxi->end(); it++){
         uint min, max;
         if ( *it < winsize )
            min = 0;
         else
            min = *it - winsize;

         if ( *it + winsize > size-1)
            max = size-1;
         else
            max = *it + winsize;

         double mean = _Mean(data, min, max);
         double sigma = _Sigma(data, min, max, mean);

         if ( data[*it]  > mean + _tol * sigma)
            _peaks.push_back(*it);
      }


      delete maxi;

      _mean = _Mean(data, 0, data.size());
      _sigma  = _Sigma(data, 0, data.size(), _mean);
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


   /**
    * Return all local maximas
    */
   set<uint>* PeakFinder::Maximas(dVec &data)
   {
      set<uint>* ml = new set<uint>();
      double delta=0;

      for (int i=1; i < data.size(); i++){
         if (data[i] < data[i-1] && delta > 0)
            ml->insert(i-1);

         delta = data[i] - data[i-1];
      }

      return ml;
   }

   set<uint>* PeakFinder::Minimas(dVec &data)
   {
      set<uint>* ml = new set<uint>();
      double last = 0;

      for (int i=1; i < data.size(); i++){
         if (data[i] > last) ml->insert(i-1);
         last = data[i];
      }

      return ml;

   }

}







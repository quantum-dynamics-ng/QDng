/*
 * ProgFilter.h
 *
 *  Created on: 14.06.2011
 *      Author: mko
 */

#ifndef PROGFILTER_H_
#define PROGFILTER_H_

#include "FiltersList.h"

#define DEFAULT_EXPEC_FILENAME "expec.dat"

namespace QDLIB
{

   /**
    * This Programm type is intended for propagation analysis.
    *
    * The behaviour of the filter is the same as in the propagation programm,
    * except that changes of the wf are not written to disk.
    * applyonly doesn't make sense in this context.
    *
    * Needed sections:
    * \li filter
    * \li path to a propagation directory (containing a Propagation.meta)
    */
   class ProgFilter
   {
      private:
         XmlNode &_FilterNode;
         //XmlNode *_ContentNodes;

         string _dir; /* Directory for output */
         string _path; /* Directory for input */

         string _fname; /* base name for WF files */

         FiltersList _filter;

         int _start;

         void Init();

      public:
         ProgFilter(XmlNode &FilterNode);
         ~ProgFilter();

         void SetDirectory(const string &directory) {_dir = directory;}
         void Run();
   };

}

#endif /* PROGFILTER_H_ */

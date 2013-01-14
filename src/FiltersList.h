#ifndef QDLIBFILTERSLIST_H
#define QDLIBFILTERSLIST_H

#include <iostream>
#include <fstream>

#include "qdlib/Operator.h"
#include "tools/XmlNode.h"
#include "tools/QDGlobalClock.h"

#define MAX_FILTERS 128           /* Maximum number of filters */
#define DEFAULT_EXPEC_FILENAME "expec.dat"  /* Default filename for expectation values */

namespace QDLIB
{

   /**
    *  This class holds a list of operators.
    * 
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class FiltersList
   {
      private:
         typedef enum
         {
            expec, apply, expeconly, normalize
         } _FilterAction;

         typedef enum
         {
            real, imag, complex
         } _FilterValue;

         struct filter
         {
            Operator* op;           /* Operator to use*/
            _FilterAction action;   /* action for corresponding operator */
            _FilterValue  value;    /* Representation of number */
            dcomplex sum;           /* Accumulated value */
            bool integrate;         /* Use time integrated/accumulated value */
            string label;           /* User defined label for expec table*/

            filter() : op(NULL), action(expeconly), value(real), sum(0), integrate(false) {}
         };

         bool _renorm;

         lint _size;
         bool _initalized;


         filter _filter[MAX_FILTERS];

//         Operator* _olist[MAX_FILTERS]; /* Operator list */
//         _FilterAction _action[MAX_FILTERS]; /* action for corresponding operator */
//         _FilterValue _value[MAX_FILTERS]; /* action for corresponding operator */
//         dcomplex _sum[MAX_FILTERS]; /* Sum for integretion */
//         bool _integrate[MAX_FILTERS]; /* Turn on integration */
//         string _labels[MAX_FILTERS]; /* user defined labels for expec table  */

         string _fname;
         bool _writefile; /* write to file or stdout */

         ofstream _ofile;

         QDClock *_clock;

         void _destroy();
      public:
         FiltersList();
         ~FiltersList();

         void SetDefaultName(const string &s);

         void Init(XmlNode* section);

         void UseRenormalizedValues(bool renorm) { _renorm = renorm; }

         void Add(const Operator* O, const string& header, const string& faction, const string& val, bool integrate = false);

         void PrepareOutput();

         lint Size() const
         {
            return _size;
         }

         void Apply(WaveFunction *Psi);
   };

}

#endif

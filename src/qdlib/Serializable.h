/*
 * OExternalData.h
 *
 *  Created on: Jan 6, 2014
 *      Author: markus
 */

#ifndef OEXTERNALDATA_H_
#define OEXTERNALDATA_H_

#include "qdlib/Operator.h"

namespace QDLIB
{

   /**
    * Interface for serialization methods.
    *
    * Every Operator which loads external data via the stream
    * format must derived from this class.
    */
   class Serializiable
   {
      public:
         virtual ~Serializiable() {}

         virtual void Serialize (std::ostream& os) = 0;
         virtual void DeSerialize (std::istream& is) = 0;

         virtual const string& Name() = 0;
         virtual ParamContainer& Params() = 0;
   };

} /* namespace QDLIB */
#endif /* OEXTERNALDATA_H_ */

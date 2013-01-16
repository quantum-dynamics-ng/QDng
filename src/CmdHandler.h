/*
 * CmdHandler.h
 *
 *  Created on: Jan 15, 2013
 *      Author: markus
 */

#ifndef CMDHANDLER_H_
#define CMDHANDLER_H_

#include <string>

using namespace std;

namespace QDLIB
{

   class CmdHandler
   {
      private:
         void SendBinary();
         void RecvBinary();
      public:
         CmdHandler(const string& file);
         ~CmdHandler();

         void Run();
   };

} /* namespace QDLIB */
#endif /* CMDHANDLER_H_ */

/*
 * CmdHandler.h
 *
 *  Created on: Jan 15, 2013
 *      Author: markus
 */

#ifndef CMDHANDLER_H_
#define CMDHANDLER_H_

#include "tools/FIFO.h"
#include "IACmds.pb.h"

#include <string>
#include <iostream>


using namespace std;

namespace QDLIB
{

   class CmdHandler
   {
      public:
         static void RunXML(const char* buf, size_t size, const string& dir);

      private:
         char* buffer;
         FIFO* fifo_;
         istream* sin_;
         ostream* sout_;

         void SendBinary();
         void RecvBinary();

         void SendResponse(Response& resp);
      public:
         CmdHandler(istream& in, ostream& out);
         CmdHandler(const string& file);
         CmdHandler();

         ~CmdHandler();

         void RunInteractive(const string& dir);
   };

} /* namespace QDLIB */
#endif /* CMDHANDLER_H_ */

/*
 * CmdHandler.h
 *
 *  Created on: Jan 15, 2013
 *      Author: markus
 */

#ifndef CMDHANDLER_H_
#define CMDHANDLER_H_

#include <string>
#include <iostream>
#include "tools/FIFO.h"

using namespace std;

namespace QDLIB
{

   class CmdHandler
   {
      public:
         static void RunXML(const char* buf, size_t size, const string& dir);

      private:
         FIFO* _fifo;
         istream* _sin;
         ostream* _sout;

         void SendBinary();
         void RecvBinary();
      public:
         CmdHandler(istream& in, ostream& out);
         CmdHandler(const string& file);
         CmdHandler();

         ~CmdHandler();

         void RunInteractive(const string& dir);
   };

} /* namespace QDLIB */
#endif /* CMDHANDLER_H_ */

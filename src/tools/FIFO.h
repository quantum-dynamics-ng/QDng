/*
 * FIFO.h
 *
 *  Created on: Jan 16, 2013
 *      Author: markus
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <iostream>
#include <fstream>


using namespace std;

#define QDNG_FIFO_BASENAME "QDng_cmd_XXXXXX"  /* File name template. Must be exactly 6 X's a the end ! */

namespace QDLIB
{

   /**
    * This class handles a temporary FIFO
    */
   class FIFO
   {
      private:
         string _fname;
         fstream _istream;
         fstream _ostream;

         void _Open();
      public:
         FIFO();
         FIFO(const string& path);

         ~FIFO();

         /**
          * Get the associated stream object for input.
          */
         fstream& GetRecvStream() { return _istream; };

         /**
          * Get the associated stream object for output.
          */
         fstream& GetSendStream() { return _ostream; };

         /**
          * Get the path to the FIFO file.
          *
          * Add "tx" to the send stream and "rx" to the receive stream.
          */
         const string& GetName() { return _fname; }
   };

} /* namespace QDLIB */
#endif /* FIFO_H_ */

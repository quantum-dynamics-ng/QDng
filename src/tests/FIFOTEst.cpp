/*
 * FIFOTEst.cpp
 *
 *  Created on: Jan 16, 2013
 *      Author: markus
 */

#include "FIFOTEst.h"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FIFOTEst);

void FIFOTEst::IO_Test()
{
   FIFO* fifo = NULL;

   /**
    * Open a FIFO
    */
   CPPUNIT_ASSERT_NO_THROW(fifo = new FIFO());

   cout << fifo->GetName() << endl;

   fstream& serverRecv = fifo->GetRecvStream();
   fstream& serverSend = fifo->GetSendStream();

   CPPUNIT_ASSERT( serverRecv.is_open() );
   CPPUNIT_ASSERT( serverSend.is_open() );

   /**
    * Create another end point for testing.
    */
   fstream clientSend;
   fstream clientRecv;
   CPPUNIT_ASSERT_NO_THROW( clientRecv.open((fifo->GetName() + "tx").c_str(), fstream::in) );
   CPPUNIT_ASSERT_NO_THROW( clientSend.open((fifo->GetName() + "rx").c_str(), fstream::out) );
   CPPUNIT_ASSERT( clientRecv.is_open() );
   CPPUNIT_ASSERT( clientSend.is_open() );


   /**
    * Try to get message through.
    */

   string mesg = "s->c";
   char buf[256];

//   while (string(buf) != "exit"){
//      cin >> mesg;
//      serverSend << mesg << endl;
//      cout << "echo:\n";
//      clientRecv.getline(buf, 256);
//      cout << buf <<endl;
//   }
//
//   buf[0] = 0;
//   while (string(buf) != "exit"){
//      cin >> mesg;
//      clientSend << mesg << endl;
//      cout << "echo:\n";
//      serverRecv.getline(buf, 256);
//      cout << buf <<endl;
//   }

   serverSend << mesg << endl;
   clientRecv.getline(buf, 256);

   CPPUNIT_ASSERT( mesg == buf);

   mesg = "c->s";
   clientSend << mesg << endl;
   serverRecv.getline(buf, 256);

   CPPUNIT_ASSERT( mesg == buf);

   CPPUNIT_ASSERT_NO_THROW(delete fifo);
}

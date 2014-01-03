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


   istream* serverRecv = fifo->GetRecvStream();
   ostream* serverSend = fifo->GetSendStream();

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

   *serverSend << mesg << endl;
   clientRecv.getline(buf, 256);

   CPPUNIT_ASSERT( mesg == buf);

   mesg = "c->s";
   clientSend << mesg << endl;
   serverRecv->getline(buf, 256);

   CPPUNIT_ASSERT( mesg == buf);

   CPPUNIT_ASSERT_NO_THROW(delete fifo);
}

void FIFOTEst::IO_RxTest()
{
   FIFO fifo("fifo_test");

   istream* fin = fifo.GetRecvStream();
   ofstream cl_out("fifo_testrx", ios::app);

   int num = 12345; // This is a magic number
   int num_in = 0;
   CPPUNIT_ASSERT(cl_out.is_open());
   cl_out.write(reinterpret_cast<char*>(&num), sizeof(num));
   cl_out.close();

   fin->read(reinterpret_cast<char*>(&num_in), sizeof(num_in));

   CPPUNIT_ASSERT(num == num_in);
}


void FIFOTEst::IO_TxTest()
{
   FIFO fifo("fifo_test");

   ostream* fout = fifo.GetSendStream();
   ifstream cl_in("fifo_testtx", ios::app);

   int num = 12345; // This is a magic number
   int num_in = 0;

   fout->write(reinterpret_cast<char*>(&num), sizeof(num));
   fout->flush();

   CPPUNIT_ASSERT(cl_in.is_open());
   cl_in.read(reinterpret_cast<char*>(&num_in), sizeof(num_in));
   cl_in.close();


   CPPUNIT_ASSERT(num == num_in);
}

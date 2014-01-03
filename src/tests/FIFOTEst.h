/*
 * FIFOTEst.h
 *
 *  Created on: Jan 16, 2013
 *      Author: markus
 */

#ifndef FIFOTEST_H_
#define FIFOTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "tools/FIFO.h"

using namespace QDLIB;



class FIFOTEst : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( FIFOTEst );
   CPPUNIT_TEST( IO_Test );
   CPPUNIT_TEST( IO_RxTest );
   CPPUNIT_TEST( IO_TxTest );
   CPPUNIT_TEST_SUITE_END();

   protected:
      void IO_Test();
      void IO_RxTest();
      void IO_TxTest();
};

#endif /* FIFOTEST_H_ */


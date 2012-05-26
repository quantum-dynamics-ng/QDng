/*
 * OGridHOFDTest.h
 *
 *  Created on: 09.05.2012
 *      Author: markus
 */

#ifndef OGRIDHOFDTEST_H_
#define OGRIDHOFDTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/OGridHOFD.h"
#include "qdlib/Diff.h"
#include "qdlib/WFGridCartesian.h"

class OGridHOFDTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( OGridHOFDTest );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( NUMERIC_D1dx_Test );
   CPPUNIT_TEST( NUMERIC_D2dx_Test );
   CPPUNIT_TEST( NUMERIC_D2dx_2D_Test );
   CPPUNIT_TEST_SUITE_END();

      QDLIB::ParamContainer pm;
      QDLIB::OGridHOFD* Op;
      QDLIB::WFGridCartesian* wf;
      QDLIB::WFGridCartesian* wf_res;
      QDLIB::WFGridCartesian* wf_resIP;
      QDLIB::dVec x;

   public:
      OGridHOFDTest();
      ~OGridHOFDTest();

      virtual void setUp();
      virtual void tearDown();

   protected:
      void API_Test();
      void NUMERIC_D1dx_Test();
      void NUMERIC_D2dx_Test();
      void NUMERIC_D2dx_2D_Test();

};

#endif /* OGRIDHOFDTEST_H_ */

/*
 * OGridHOFDTest.cpp
 *
 *  Created on: 09.05.2012
 *      Author: markus
 */

#include "OGridHOFDTest.h"
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(OGridHOFDTest);

using namespace QDLIB;

#define NX HOFD_PERDIFF*2+1

OGridHOFDTest::OGridHOFDTest()
{
   x.newsize(NX);
   for (int i=0; i < NX; i++)
      x[i] = double(i)/2;
}

OGridHOFDTest::~OGridHOFDTest()
{
}

void OGridHOFDTest::API_Test()
{
   /* Check Derviative */
   pm.SetValue("deriv", 1);
   pm.SetValue("order", 2);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );

   pm.SetValue("deriv", HOFD_NDIFF);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );

   pm.SetValue("deriv", 3);
   CPPUNIT_ASSERT_THROW( Op->Init(pm), EParamProblem );

   /* Check Order */
   pm.SetValue("deriv", 2);
   pm.SetValue("order", 2);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );

   pm.SetValue("order", HOFD_PERDIFF*2);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );

   pm.SetValue("order", 3);
   CPPUNIT_ASSERT_THROW( Op->Init(pm), EParamProblem );

   pm.clear();
   pm.SetValue("deriv", 1);
   pm.SetValue("order", 2);
   pm.SetValue("dim", 0);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(wf) );


}

void OGridHOFDTest::NUMERIC_D1dx_Test()
{
   pm.SetValue("deriv", 1);
   pm.SetValue("dim", 0);
   cout << " order ";
   for (int order=2; order <= HOFD_PERDIFF*2; order+=2){
      pm.SetValue("order",  order);
      cout << " " << order;
      CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );
      CPPUNIT_ASSERT_NO_THROW( Op->Init(wf) );

      Op->Apply(wf_res, wf);

      wf_resIP->FastCopy(*wf);
      Op->Apply(wf_resIP);

      /* Only check values at the centre of the grid. forget ghost points */
      for (int i=order/2; i < NX-order/2; i++){
         CPPUNIT_ASSERT_DOUBLES_EQUAL(2*x[i], (*( (cVec*) (wf_res)))[i].real(), LOOSE_EPS);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*( (cVec*) (wf_res)))[i].imag(), TIGHT_EPS);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(2*x[i], (*( (cVec*) (wf_resIP)))[i].real(), LOOSE_EPS);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*( (cVec*) (wf_resIP)))[i].imag(), TIGHT_EPS);
      }
   }

}

void OGridHOFDTest::NUMERIC_D2dx_Test()
{
   pm.SetValue("deriv", 2);
   pm.SetValue("dim", 0);
   cout << " order ";
   for (int order=2; order <= HOFD_PERDIFF*2; order+=2){
      pm.SetValue("order",  order);
      cout << " " << order;
      CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );
      CPPUNIT_ASSERT_NO_THROW( Op->Init(wf) );

      Op->Apply(wf_res, wf);

      wf_resIP->FastCopy(*wf);
      Op->Apply(wf_resIP);

      /* Only check values at the centre of the grid. forget ghost points */
      for (int i=order/2; i < NX-order/2; i++){
         CPPUNIT_ASSERT_DOUBLES_EQUAL(2, (*( (cVec*) (wf_res)))[i].real(), LOOSE_EPS);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*( (cVec*) (wf_res)))[i].imag(), TIGHT_EPS);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(2, (*( (cVec*) (wf_resIP)))[i].real(), LOOSE_EPS);
         CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*( (cVec*) (wf_resIP)))[i].imag(), TIGHT_EPS);
      }
   }
}

void OGridHOFDTest::setUp()
{
   pm.clear();
   Op = new OGridHOFD();
   wf = new WFGridCartesian();
   wf_res = new WFGridCartesian();
   wf_resIP = new WFGridCartesian();

   pm.SetValue("dims", 1);
   pm.SetValue("N0", NX);
   pm.SetValue("xmin0", 0);
   pm.SetValue("xmax0", x[NX-1]);
   wf->Init(pm);
   wf_res->Init(pm);
   wf_resIP->Init(pm);

   pm.clear();

   (*((cVec*) wf_res)) = dcomplex(0);
   (*((cVec*) wf_resIP)) = dcomplex(0);
   (*((cVec*) wf)) = dcomplex(0);
   /* create test parabola */
   for (int i=0; i < NX; i++)
      (*wf)[i] = x[i] * x[i];

}

void OGridHOFDTest::tearDown()
{
   DELETE_ALL_OP();
   DELETE_WF(wf);
   DELETE_WF(wf_res);
   DELETE_WF(wf_resIP);

}





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

#define NX HOFD_MAXORDER*2+1

OGridHOFDTest::OGridHOFDTest()
{
   x.newsize(2*NX);
   for (int i=0; i < 2*NX; i++)
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

   DELETE_OP(Op);  Op = new OGridHOFD();
   pm.SetValue("deriv", HOFD_NDIFF);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );

   DELETE_OP(Op);  Op = new OGridHOFD();
   pm.SetValue("deriv", 3);
   CPPUNIT_ASSERT_THROW( Op->Init(pm), EParamProblem );

   /* Check Order */
   DELETE_OP(Op);  Op = new OGridHOFD();
   pm.SetValue("deriv", 2);
   pm.SetValue("order", 2);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );

   DELETE_OP(Op);  Op = new OGridHOFD();
   pm.SetValue("order", HOFD_MAXORDER);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );

   DELETE_OP(Op);  Op = new OGridHOFD();
   pm.SetValue("order", 3);
   CPPUNIT_ASSERT_THROW( Op->Init(pm), EParamProblem );

   DELETE_OP(Op);  Op = new OGridHOFD();
   pm.clear();
   pm.SetValue("deriv", 1);
   pm.SetValue("order", 2);
   pm.SetValue("dim", 0);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );
   CPPUNIT_ASSERT_NO_THROW( Op->Init(wf) );

}

void OGridHOFDTest::NUMERIC_D1dx_Test()
{
   pm.SetValue("deriv", 1);
   pm.SetValue("dim", 0);
   cout << " order ";
   for (int order=2; order <= HOFD_MAXORDER; order+=2){
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

      if (order > 10){
         /* create test parabola */
         for (int i=0; i < NX; i++){
            (*wf_res)[i] = cos(x[i]*4*M_PI/(double(NX)));
            (*wf_resIP)[i] = -4*M_PI/(double(NX)) * sin(x[i]*4*M_PI/(double(NX)));
         }

         Op->Apply(wf_res);

         /* Check points - include boundary (PBC)*/
         for (int i=0; i < NX; i++){
            CPPUNIT_ASSERT_DOUBLES_EQUAL((*wf_resIP)[i].real(), (*( (cVec*) (wf_res)))[i].real(), ROUGH_EPS);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*( (cVec*) (wf_res)))[i].imag(), TIGHT_EPS);
         }
      }
   }

}

void OGridHOFDTest::NUMERIC_D2dx_Test()
{
   pm.SetValue("deriv", 2);
   pm.SetValue("dim", 0);
   cout << " order ";
   for (int order=2; order <= HOFD_MAXORDER; order+=2){
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

void OGridHOFDTest::NUMERIC_D2dx_2D_Test()
{
   WFGridCartesian* wf2 = new WFGridCartesian();
   WFGridCartesian* wf2_res = new WFGridCartesian();

   /**
    * Init a 2D-WF
    */
   pm.SetValue("dims", 2);
   pm.SetValue("N0", NX);
   pm.SetValue("N1", 2*NX);
   pm.SetValue("xmin0", 0);
   pm.SetValue("xmax0", x[NX-1]);
   pm.SetValue("xmin1", 0);
   pm.SetValue("xmax1", x[2*NX-1]);

   wf2->Init(pm);
   wf2_res->Init(pm);

   (*((cVec*) wf2_res)) = dcomplex(0);
   (*((cVec*) wf2)) = dcomplex(0);
   pm.SetValue("deriv", 2);
   pm.SetValue("dim", 0);

   /* creat test func.: x_0^2 + 3*x_1^2 */
   int n = 0;
   for (int i=0; i < 2*NX; i++){
      for (int j=0; j < NX; j++){
         (*wf2)[n] = x[j] * x[j] + 3 * x[i] * x[i];
         //cout << "i = " << i << " , j = " << j << " ; x1 = "<<  x[i] << " ; x0 = "<<  x[j] << endl;
         n++;
      }
   }

   /* Setup operator for diff. w respect to dim 0 */
   pm.clear();
   pm.SetValue("deriv", 2);
   pm.SetValue("dim", 0);
   pm.SetValue("order", 8);

   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );
   CPPUNIT_ASSERT_NO_THROW( Op->Init(wf2) );

   Op->Apply(wf2_res, wf2);

   /* check dim 0*/
   n = 0;
   for (int i=0; i < NX*2; i++){
      for (int j=0; j < NX; j++){
         if (j >= 8/2 && j < NX-8/2){ /* don't care about ghosts */
            CPPUNIT_ASSERT_DOUBLES_EQUAL(2, (*( (cVec*) (wf2_res)))[n].real(), LOOSE_EPS);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*( (cVec*) (wf2_res)))[n].imag(), TIGHT_EPS);
         }
         n++;
      }
   }

   /* check dim 1*/
   pm.SetValue("dim", 1);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );
   CPPUNIT_ASSERT_NO_THROW( Op->Init(wf2) );

   Op->Apply(wf2_res, wf2);

   n = 0;
   for (int i=0; i < NX*2; i++){
      for (int j=0; j < NX; j++){
         if (i >= 8/2 && i < 2*NX-8/2){ /* don't care about ghosts */
            CPPUNIT_ASSERT_DOUBLES_EQUAL(6, (*( (cVec*) (wf2_res)))[n].real(), LOOSE_EPS);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*( (cVec*) (wf2_res)))[n].imag(), TIGHT_EPS);
         }
         n++;
      }
   }

   /* check All dims */
   DELETE_OP(Op);  Op = new OGridHOFD();
   pm.clear();
   pm.SetValue("deriv", 2);
   pm.SetValue("order", 8);
   CPPUNIT_ASSERT_NO_THROW( Op->Init(pm) );
   CPPUNIT_ASSERT_NO_THROW( Op->Init(wf2) );

   (*((cVec*) wf2_res)) = dcomplex(0);
   Op->Apply(wf2_res, wf2);

   n = 0;
   for (int i=0; i < NX*2; i++){
      for (int j=0; j < NX; j++){
         if (i >= 8/2 && i < 2*NX-8/2 && j >= 8/2 && j < NX-8/2){ /* don't care about ghosts */
            CPPUNIT_ASSERT_DOUBLES_EQUAL(6.+ 2., (*( (cVec*) (wf2_res)))[n].real(), LOOSE_EPS);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*( (cVec*) (wf2_res)))[n].imag(), TIGHT_EPS);
         }
         n++;
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





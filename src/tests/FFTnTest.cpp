/*
 * FFTnTest.cpp
 *
 *  Created on: 31.12.2010
 *      Author: Markus Kowalewski
 */
#include "FFTnTest.h"
#include "fft/fft.h"
#include "math/VectorView.h"
#include <string.h>
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FFTnTest);

using namespace QDLIB;

FFTnTest::FFTnTest()
{

}

FFTnTest::~FFTnTest()
{

}

void FFTnTest::NUMERIC_Test()
{
   /* Grid Description */
   GridSystem grid;

   grid.Dim(3);
   grid.DimSize(0, 4);
   grid.DimSize(1, 6);
   grid.DimSize(2, 8);

   cVec vec1;
   cVec vec3;
   cVec vec3out;

   vec3.newsize(4*6*8);
   vec3out.newsize(4*6*8);

   vec3 = dcomplex(0);

   VectorView<dcomplex> view(vec3, 3, grid.DimSizes());

   view.ActiveDim(0);

   /* sine along fastest dimension */
   vec1.newsize(4);
   vec1[0] = 0;
   vec1[1] = 1;
   vec1[2] = 0;
   vec1[3] = -1;

   view = vec1;

   /* Setup fft */
   FFT ft(grid, vec3, vec3out);

   ft.forward(0);

//   cout << vec3out;

   for (int i=0; i < 4*6*8; i+=2){
      CPPUNIT_ASSERT_DOUBLES_EQUAL(0, cabs(vec3out[i]), LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(2, cabs(vec3out[i+1]), LOOSE_EPS);
   }

   /* sine along dim 1 */
   view.ActiveDim(1);

   vec1.newsize(6);
   vec1[0] = 0;
   vec1[1] = sin(1./3. * M_PI);
   vec1[2] = sin(2./3. * M_PI);
   vec1[3] = 0;
   vec1[4] = -sin(1./3. * M_PI);
   vec1[5] = -sin(2./3. * M_PI);

   view = vec1;
   cout << fixed;
   cout.precision(8);

   ft.forward(1);

   /* This should be the outcome */
   vec1 = dcomplex(0);
   vec1[1] = 3;
   vec1[5] = 3;

   view = vec1;

   for (int i=0; i < 4*6*8; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL(cabs(vec3[i]), cabs(vec3out[i]), LOOSE_EPS);
   }


   /* sine along dim 2 */
   view.ActiveDim(2);

   vec1.newsize(8);
   vec1[0] = 0;
   vec1[1] = sin(1./4. * M_PI);
   vec1[2] = 1.;
   vec1[3] = sin(3./4. * M_PI);;
   vec1[4] = 0;
   vec1[5] = sin(5./4. * M_PI);
   vec1[6] = -1;
   vec1[7] = sin(5./4. * M_PI);
   view = vec1;
   ft.forward(2);

   /* This should be the outcome */
   vec1 = dcomplex(0);
   vec1[1] = 4;
   vec1[7] = 4;
   view = vec1;

   for (int i=0; i < 4*6*8; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL(cabs(vec3[i]), cabs(vec3out[i]), LOOSE_EPS);
   }

}


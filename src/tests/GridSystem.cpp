#include "GridSystem.h"

CPPUNIT_TEST_SUITE_REGISTRATION(GridSystemTest);

using namespace QDLIB;

GridSystemTest::GridSystemTest()
 : CppUnit::TestFixture()
{
}

GridSystemTest::~GridSystemTest()
{
}

void GridSystemTest::API_Test()
{
   GridSystem g1,g2;
   
   /* Use Setters */
   g1.Dim(2);
   g1.DimSize(0, 11);
   g1.Xmin(0, 3);
   g1.Xmax(0, 13);
   
   g1.DimSize(1, 6);
   g1.Xmin(1, 4);
   g1.Xmax(1, 24);
   
   /* Check Getters */
   CPPUNIT_ASSERT(g1.Dim() == 2);
   
   CPPUNIT_ASSERT(g1.DimSize(0) == 11);
   CPPUNIT_ASSERT(g1.DimSize(1) == 6);
   
   CPPUNIT_ASSERT(g1.Xmin(0) == 3);
   CPPUNIT_ASSERT(g1.Xmin(1) == 4);

   CPPUNIT_ASSERT(g1.Xmax(0) == 13);
   CPPUNIT_ASSERT(g1.Xmax(1) == 24);
   
   CPPUNIT_ASSERT(g1.Dx(0) == 1);
   CPPUNIT_ASSERT(g1.Dx(1) == 4);

   const int *sizes = g1.DimSizes();
   CPPUNIT_ASSERT(sizes[0] == 11);
   CPPUNIT_ASSERT(sizes[1] == 6);
   
   /* Comparisons */
   CPPUNIT_ASSERT(g1 == g1);
   CPPUNIT_ASSERT( !(g1 != g1));
   CPPUNIT_ASSERT(g1 != g2);
   /* Copy test */
   g2 = g1;
   CPPUNIT_ASSERT(g2.Dim() == 2);
  
   CPPUNIT_ASSERT(g2.DimSize(0) == 11);
   CPPUNIT_ASSERT(g2.DimSize(1) == 6);
   
   CPPUNIT_ASSERT(g2.Xmin(0) == 3);
   CPPUNIT_ASSERT(g2.Xmin(1) == 4);

   CPPUNIT_ASSERT(g2.Xmax(0) == 13);
   CPPUNIT_ASSERT(g2.Xmax(1) == 24);
   
   CPPUNIT_ASSERT(g2.Dx(0) == 1);
   CPPUNIT_ASSERT(g2.Dx(1) == 4);
   
   CPPUNIT_ASSERT(g1 == g2);
   
}



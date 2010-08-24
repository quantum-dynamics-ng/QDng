#include "TmpFileTest.h"
#include "tools/TmpFile.h"
#include <string.h>


CPPUNIT_TEST_SUITE_REGISTRATION(TmpFileTest);

using namespace QDLIB;

TmpFileTest::TmpFileTest()
{

}

TmpFileTest::~TmpFileTest()
{

}

/**
 * Test only the mmap buffer functions.
 */
void TmpFileTest::API_Test()
{
   TmpFile<int> tfile;
   int vec[] = {1,2,3};
   int *buf;
   
   CPPUNIT_ASSERT_NO_THROW( buf = tfile.Open(3, true) );
   
   CPPUNIT_ASSERT ( buf != NULL );
   CPPUNIT_ASSERT ( tfile.isOpen() );
   CPPUNIT_ASSERT ( tfile.Size() == 3 );
   
   memcpy((char*) buf,  reinterpret_cast<char*>(vec), 3*sizeof(int));
   
   for (int i=0; i < 3; i++)
      CPPUNIT_ASSERT ( buf[i] == i+1 );

   /* Resize -> larger */
   CPPUNIT_ASSERT_NO_THROW(buf = tfile.Resize(4));

   buf[3] = 4;

   for (int i=0; i < 4; i++){
      CPPUNIT_ASSERT ( buf[i] == i+1 );
   }

   /* Resize -> smaller */
   CPPUNIT_ASSERT_NO_THROW(buf = tfile.Resize(2));

   for (int i=0; i < 2; i++){
      CPPUNIT_ASSERT ( buf[i] == i+1 );
   }

   
   CPPUNIT_ASSERT_NO_THROW( tfile.Close() );
   
}


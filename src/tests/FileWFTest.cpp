#include "FileWFTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FileWFTest);

FileWFTest::FileWFTest()
 : CppUnit::TestFixture()
{
}


FileWFTest::~FileWFTest()
{
}

void FileWFTest::API_Test()
{
   /* Counter - Check for initial value */
   CPPUNIT_ASSERT(file.Counter() == 0);
   
   file.Counter(3);
   CPPUNIT_ASSERT(file.Counter() == 3);
   
   file.ResetCounter();
   CPPUNIT_ASSERT(file.Counter() == 0);
   
   /* Meta-Handling - default */
   CPPUNIT_ASSERT(file.DropMeta() == false);
   file.DropMeta(true);
   CPPUNIT_ASSERT(file.DropMeta() == true);
   file.DropMeta(false);
   
   /* Name & Suffix */
   file.Name("WFABCD");
   CPPUNIT_ASSERT(file.Name() == "WFABCD");
}

void FileWFTest::IO_Test()
{
}



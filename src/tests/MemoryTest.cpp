/*
 * MemoryTest.cpp
 *
 *  Created on: 23.10.2011
 *      Author: markus
 */


#include "MemoryTest.h"
#include "tools/Memory.h"
#include "tools/GlobalParams.h"

using namespace QDLIB;

CPPUNIT_TEST_SUITE_REGISTRATION(MemoryTest);



MemoryTest::MemoryTest()
{
}

MemoryTest::~MemoryTest()
{
}

void MemoryTest::CORE_Test()
{

   /* User defined max mem value */
   ParamContainer& params = GlobalParams::Instance();
   params.SetValue("MaxMem", 1024*1024); /* 1MB, All follow up test should pass with this limit */

   Memory& mem = Memory::Instance();
   mem.Reconfigure();

   /* Check metric of empty mem */
   CPPUNIT_ASSERT( mem.MaximumSize() == 1024*1024 ); /* should be 1MB */
//   CPPUNIT_ASSERT( mem.CurrentSizeAvail() == 1024*1024 );
//   CPPUNIT_ASSERT( mem.CurrentSizeUsed() == 0 );
//   CPPUNIT_ASSERT( mem.MaximumSizeUsed() == 0 );


   /* Allocate memory */
   char* block1;
   CPPUNIT_ASSERT_NO_THROW( mem.Align((void**) &block1, 512) ); /* Alloc 512 bytes */

   /* Check metrics with alloctation */
   CPPUNIT_ASSERT( mem.MaximumSize() == 1024*1024 ); /* should be 1MB */
//   CPPUNIT_ASSERT( mem.CurrentSizeAvail() == 1024*1024 - 512 );
//   CPPUNIT_ASSERT( mem.CurrentSizeUsed() == 512 );
//   CPPUNIT_ASSERT( mem.MaximumSizeUsed() == 512 );

   /* Try to allocate to much */
   CPPUNIT_ASSERT_THROW( mem.Align((void**) &block1, 1024*1024+-512+1), EMemory ); /* Alloc addiotnal 1MB */

   /* Free memory */
   CPPUNIT_ASSERT_NO_THROW( mem.Free((void*) block1) ); /* Alloc 512 bytes */


   /* Check metrics after alloctation */
   CPPUNIT_ASSERT( mem.MaximumSize() == 1024*1024 ); /* should be 1MB */
//   CPPUNIT_ASSERT( mem.CurrentSizeAvail() == 1024*1024 );
//   CPPUNIT_ASSERT( mem.CurrentSizeUsed() == 0 );
//   CPPUNIT_ASSERT( mem.MaximumSizeUsed() == 512 );
}


/**
 * Internal memcopy test. Vector support
 */
void MemoryTest::COPY_Test()
{
   Memory& mem = Memory::Instance();

   char *src=NULL, *dst=NULL;

   mem.Align((void**) &src, 68);
   mem.Align((void**) &dst, 68);

   sprintf(src, "It's the string for the test. Should be 67 bytes + terminatin zero.");
   mem.Copy(dst, src, 68);

   CPPUNIT_ASSERT( string(dst) == "It's the string for the test. Should be 67 bytes + terminatin zero.");

}

/** Check static members for formating */
void MemoryTest::API_Test()
{
   Memory& mem = Memory::Instance();

   /* Test formated reading */
   string s("2K");
   CPPUNIT_ASSERT( mem.ReadFromString(s) == 1024 * 2);
   s = "2MB";
   CPPUNIT_ASSERT( mem.ReadFromString(s) == 1024*1024 * 2);
   s = "2GB";
   CPPUNIT_ASSERT( mem.ReadFromString(s) == 1024l*1024l*1024l * 2);
   s = "2TB";
   CPPUNIT_ASSERT( mem.ReadFromString(s) == 1024l*1024l*1024l*1024l * 2l);

   /* Test formating writing */
   CPPUNIT_ASSERT( mem.Format(1023) == "1023");
   CPPUNIT_ASSERT( mem.Format(1025) == "1.0KB");
   CPPUNIT_ASSERT( mem.Format(1024l*1024l+1) == "1.0MB");
   CPPUNIT_ASSERT( mem.Format(1024l*1024l*1024l+1) == "1.0GB");
   CPPUNIT_ASSERT( mem.Format(1024l*1024l*1024l*1024l+1) == "1.0TB");

}


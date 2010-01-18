#ifndef FILEWFTEST_H
#define FILEWFTEST_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/FileWF.h"


using namespace QDLIB;

/**
 * Testing for FileWF & Underling Template FileSingle.
 * 
 * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class FileWFTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( FileWFTest );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( IO_Test_Single );
   CPPUNIT_TEST( IO_Test_Multistate );
   CPPUNIT_TEST_SUITE_END();
   public:
      
      FileWFTest();
   
      ~FileWFTest();
     
      
   protected:
      void API_Test();
      void IO_Test_Single();
      void IO_Test_Multistate();
   private:
      FileWF file;
};

#endif

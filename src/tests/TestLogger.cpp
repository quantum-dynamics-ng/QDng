#include "TestLogger.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestLogger);

using namespace QDLIB;

TestLogger::TestLogger()
{
}


TestLogger::~TestLogger()
{
}

void TestLogger::RunLogger()
{
   Logger *log = Logger::Instance();

}



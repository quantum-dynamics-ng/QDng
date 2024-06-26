#include "cppunit/BriefTestProgressListener.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestResultCollector.h"
#include "cppunit/TestRunner.h"

#include "ComplexNumbers.h"

#ifdef HAVE_MPI
#include "mpi.h"
#endif

using namespace std;
int
main( int argc, char* argv[] )
{
#ifdef HAVE_MPI
   MPI::Init();
#endif

  // Create the event manager and test controller
   CPPUNIT_NS::TestResult controller;
   CPPUNIT_NS::TestResultCollector result;
   CPPUNIT_NS::BriefTestProgressListener progress;


#ifdef HAVE_MPI
   if (MPI::COMM_WORLD.Get_rank() == 0){
#endif
  // Add a listener that colllects test result
   controller.addListener( &result );

  // Add a listener that print dots as test run.
   controller.addListener( &progress );
#ifdef HAVE_MPI
   }
#endif

  // Add the top suite to the test runner
   CPPUNIT_NS::TestRunner runner;
   runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
   runner.run( controller );

#ifdef HAVE_MPI
   if (MPI::COMM_WORLD.Get_rank() == 0){
#endif
   cout << endl;
  // Print test in a compiler compatible format.
   CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
   outputter.write();
#ifdef HAVE_MPI
   }

   MPI::Finalize();
#endif
   return result.wasSuccessful() ? 0 : 1;
}


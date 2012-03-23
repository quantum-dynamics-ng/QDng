#include "OHermitianMatrixTest.h"
#include "defs.h"

#include "qdlib/OHermitianMatrix.h"
#include "qdlib/WFLevel.h"
#include "tools/fstools.h"


using namespace std;
using namespace QDLIB;

CPPUNIT_TEST_SUITE_REGISTRATION(OHermitianMatrixTest);


OHermitianMatrixTest::OHermitianMatrixTest()
 : TestFixture()
{
}


OHermitianMatrixTest::~OHermitianMatrixTest()
{
}

void OHermitianMatrixTest::API_Test()
{
   OHermitianMatrix *M = new OHermitianMatrix();
   OHermitianMatrix *R;
   ParamContainer pm;

   /* Basic stuff */

   CPPUNIT_ASSERT(M->Name() == "OHermitianMatrix");

   M->Size(2);
   CPPUNIT_ASSERT(M->Size() == 2 && M->Size() == M->cols() && M->Size() == M->rows());

   M->Size(0);

   /* Empty init*/
   CPPUNIT_ASSERT_THROW( M->Init(pm), EParamProblem);

   /* Correct init */
   pm.SetValue("size", 2);
   CPPUNIT_ASSERT_NO_THROW( M->Init(pm) );

   /* Min, Max, Scaling */
   (*M)(0,0)=0;
   (*M)(1,1)=0;
   (*M)(1,0) = (*M)(0,1) = 2;

   M->InitDspace(); /* Need to do this here because of Manual filing */

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Emin", -2, M->Emin().real(), LOOSE_EPS);
   CPPUNIT_ASSERT_MESSAGE("Emax", 1 < cabs(M->Emax()));

   /* Check the FileWriter */
   M->File()->Suffix(".test");
   M->File()->Name("mat");
   CPPUNIT_ASSERT_NO_THROW( *(M->File()) << M);

   R = new OHermitianMatrix();
   CPPUNIT_ASSERT_NO_THROW( *(M->File()) >> R);

   CPPUNIT_ASSERT_NO_THROW(FS::Remove("mat.test"));
   CPPUNIT_ASSERT_NO_THROW(FS::Remove("mat.meta"));


   /* Try Init with WF */
   WFLevel *wf = new WFLevel();
   pm.clear();

   pm.SetValue("size", "2");
   wf->Init(pm);

   CPPUNIT_ASSERT_NO_THROW(M->Init(wf));
   
   DELETE_ALL_OP();
   DELETE_ALL_WF();
}

void OHermitianMatrixTest::NUMERIC_Test()
{
   OHermitianMatrix *M = new OHermitianMatrix();
   
   WFLevel *wf = new WFLevel();
   
   WaveFunction *wfo;
   ParamContainer pm;

   /* Setup the test */
   pm.SetValue("size", "2");

   CPPUNIT_ASSERT_NO_THROW ( wf->Init(pm) );
   CPPUNIT_ASSERT_NO_THROW ( M->Init(pm) );
   CPPUNIT_ASSERT_NO_THROW ( M->Init(wf)  );

   /* Init wf */
   (*((cVec*) wf))[0] = 1;
   (*((cVec*) wf))[1] = 1;

   wfo = wf->NewInstance();

   /* Init Matrix */
   (*M)(0,0)=1;
   (*M)(1,1)=3;
   (*M)(1,0) = (*M)(0,1) = 2;

   /* Apply 1 */
   M->Apply(wfo, wf);

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply", 3,  (*wfo)[0].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply", 5,  (*wfo)[1].real(), TIGHT_EPS );

   /* Apply */
   *wfo  = wf;
   M->Apply(wfo);

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply", 3,  (*wfo)[0].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply", 5,  (*wfo)[1].real(), TIGHT_EPS );

   /* Expec */
   wf->Normalize();
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Expec", 4,  M->Expec(wf), LOOSE_EPS );

   /* Init the exponential */
   cVec exp(2);

   M->InitDspace();
   M->InitExponential(&exp, dcomplex(0,1));

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("expM", 0.972265115697226, exp[0].real() , LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("expM", -0.233881475962204, exp[0].imag() , LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("expM", -0.458512806049217, exp[1].real() , LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("expM", -0.888687800461373, exp[1].imag() , LOOSE_EPS );
   
   DELETE_ALL_OP();
   DELETE_ALL_WF();
}



#include "FileWFTest.h"
#include "qdlib/WFGridCartesian.h"
#include "qdlib/WFMultistate.h"
#include "function_gens.h"
#include "tools/fstools.h"

#define WF_TEST_SIZE 64

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
   
   file.Suffix("XYZ");
   CPPUNIT_ASSERT(file.Suffix() == "XYZ");
}

/** 
 * Check for Writing & Reading a WF-file.
 * 
 * Probe for correct LoadByMeta.
 */
void FileWFTest::IO_Test_Single()
{
   ParamContainer p, p_in;
   WaveFunction *psi, *psi_in;
   string s;
   int n;
   
   /* Init Test WF */
   psi = new WFGridCartesian();
   psi_in = new WFGridCartesian();
   p.SetValue("dims",1);
   p.SetValue("N0", WF_TEST_SIZE);
   p.SetValue("xmin0", -5);
   p.SetValue("xmax0", 5);
   
   psi->Init(p);
   
   fgen_sin_norm(*psi, -5, 5);
   
   *psi *= cexpI(M_PI);
   
   /* Setup FileWF class for writing to disk */
   file.Name("TEST");
   file.Suffix(".wftest");
   
   CPPUNIT_ASSERT_NO_THROW(file << psi);
         
   /* Reread single file with pre-initialized class */
   CPPUNIT_ASSERT_NO_THROW(file >> psi_in);
   
   /* both wfs should match */
   p_in = psi_in->Params();
   CPPUNIT_ASSERT( psi->size() == psi_in->size() );

   p_in.GetValue("CLASS", s);
   CPPUNIT_ASSERT(s == "WFGridCartesian");
   
   p_in.GetValue("dims",n);
   CPPUNIT_ASSERT(n == 1);
   p_in.GetValue("N0", n);
   CPPUNIT_ASSERT(n == WF_TEST_SIZE);
   p_in.GetValue("xmin0", n);
   CPPUNIT_ASSERT(n == -5);
   p_in.GetValue("xmax0", n);
   CPPUNIT_ASSERT(n == 5);
   
   /* Values should match exactly */
   for (int i=0; i < psi->size(); i++){
      CPPUNIT_ASSERT( (*psi)[i].real() == (*psi_in)[i].real() );
      CPPUNIT_ASSERT( (*psi)[i].imag() == (*psi_in)[i].imag() );
   }
   delete psi_in;
   
 
   /* Check class initialization by meta file */
   psi_in = NULL;
   CPPUNIT_ASSERT_NO_THROW( file >> &psi_in );
   
   CPPUNIT_ASSERT (psi_in != NULL);
   
   CPPUNIT_ASSERT (psi_in->Name() == "WFGridCartesian");
   
   /* both wfs should match */
   p_in = psi_in->Params();
   CPPUNIT_ASSERT( psi->size() == psi_in->size() );

   p_in.GetValue("CLASS", s);
   CPPUNIT_ASSERT(s == "WFGridCartesian");
   
   p_in.GetValue("dims",n);
   CPPUNIT_ASSERT(n == 1);
   p_in.GetValue("N0", n);
   CPPUNIT_ASSERT(n == WF_TEST_SIZE);
   p_in.GetValue("xmin0", n);
   CPPUNIT_ASSERT(n == -5);
   p_in.GetValue("xmax0", n);
   CPPUNIT_ASSERT(n == 5);
   
   /* Values should match exactly */
   for (int i=0; i < psi->size(); i++){
      CPPUNIT_ASSERT( (*psi)[i].real() == (*psi_in)[i].real() );
      CPPUNIT_ASSERT( (*psi)[i].imag() == (*psi_in)[i].imag() );
   }

   CPPUNIT_ASSERT_NO_THROW(Remove("TEST.wftest"));
   CPPUNIT_ASSERT_NO_THROW(Remove("TEST.meta"));
   
   delete psi;
   if (psi_in != NULL) delete psi_in;
}

/**
 * Check for correct Multistate handling.
 * 
 * (Single & Multistate WFs)
 * Sequence reading & writing is checked here
 */
void FileWFTest::IO_Test_Multistate()
{
   ParamContainer p, p_in;
   WaveFunction *psi0, *psi1, *psi0_in, *psi1_in;
   WFMultistate *wfm, *wfm_in;
   string s;
   int n;
   
   /* Init Test WF */
   wfm = new WFMultistate();
   psi0 = new WFGridCartesian();
   psi1 = new WFGridCartesian();
   p.SetValue("dims",1);
   p.SetValue("N0", WF_TEST_SIZE);
   p.SetValue("xmin0", -5);
   p.SetValue("xmax0", 5);
   
   
   psi0->Init(p);
   fgen_sin_norm(*psi0, -5, 5);
   *psi0 *= cexpI(M_PI);
   
   fgen_sin_norm(*psi1, -5, 6);
   p.SetValue("xmax0", 6);
   psi1->Init(p);
   *psi1 *= cexpI(M_PI/3);
   
   CPPUNIT_ASSERT_NO_THROW( wfm->Add(psi0, 0) );
   CPPUNIT_ASSERT_NO_THROW( wfm->Add(psi1, 1) );
      
   /* Setup file writer */
   file.Name("TESTMS");
   file.Suffix(".wftest");
   
   CPPUNIT_ASSERT_NO_THROW(wfm->Init(p));
            
   /* Write File */
   CPPUNIT_ASSERT_NO_THROW(file << wfm);
   
   /* Re-read & check */
   wfm_in = new WFMultistate();
   psi0_in = new WFGridCartesian();
   psi1_in = new WFGridCartesian();
   
   
   psi0_in->Init(p);  /* At least one vector must be initialzed */
   
   p.clear();
   wfm_in->Add(psi0_in, 0);
   p.SetValue("states", 2);
   wfm_in->Init(p);
   
   CPPUNIT_ASSERT_NO_THROW (file >> wfm_in); /* Read file */
   
   /* Check MS container */
   p_in = wfm_in->Params();
   p_in.GetValue("CLASS", s);
   CPPUNIT_ASSERT( s == "WFMultistate" );
   CPPUNIT_ASSERT( wfm->States() == 2);
   
   /* Check state 0 */
   
   /* both wfs should match */
   psi0_in = wfm_in->State(0);
   p_in.clear();
   p_in = psi0_in->Params();
   CPPUNIT_ASSERT( psi0->size() == psi0_in->size() );

   p_in.GetValue("CLASS", s);
   CPPUNIT_ASSERT(s == "WFGridCartesian");
   
   p_in.GetValue("dims",n);
   CPPUNIT_ASSERT(n == 1);
   p_in.GetValue("N0", n);
   CPPUNIT_ASSERT(n == WF_TEST_SIZE);
   p_in.GetValue("xmin0", n);
   CPPUNIT_ASSERT(n == -5);
   p_in.GetValue("xmax0", n);
   CPPUNIT_ASSERT(n == 5);
  
   /* Values should match exactly */
   for (int i=0; i < psi0->size(); i++){
      CPPUNIT_ASSERT( (*psi0_in)[i].real() == (*psi0)[i].real() );
      CPPUNIT_ASSERT( (*psi0_in)[i].imag() == (*psi0)[i].imag() );
   }
   
   
   /* Check state 1 */
   
   /* both wfs should match */
   psi0_in = wfm_in->State(1);
   p_in.clear();
   p_in = psi0_in->Params();
   CPPUNIT_ASSERT( psi0->size() == psi0_in->size() );

   p_in.GetValue("CLASS", s);
   CPPUNIT_ASSERT(s == "WFGridCartesian");
   
   p_in.GetValue("dims",n);
   CPPUNIT_ASSERT(n == 1);
   p_in.GetValue("N0", n);
   CPPUNIT_ASSERT(n == WF_TEST_SIZE);
   p_in.GetValue("xmin0", n);
   CPPUNIT_ASSERT(n == -5);
   p_in.GetValue("xmax0", n);
   CPPUNIT_ASSERT(n == 6);
   
   /* Values should match exactly */
   for (int i=0; i < psi0->size(); i++){
      CPPUNIT_ASSERT( (*psi0_in)[i].real() == (*psi1)[i].real() );
      CPPUNIT_ASSERT( (*psi0_in)[i].imag() == (*psi1)[i].imag() );
   }

   delete wfm_in;
   wfm_in = NULL;
  
   /* Check load-by-meta style */
   CPPUNIT_ASSERT_NO_THROW (file >> (WaveFunction**) &wfm_in);
   
   /* Quick check if contents are there */
   CPPUNIT_ASSERT( wfm_in != NULL );
   CPPUNIT_ASSERT( wfm_in->States() == 2 );
   CPPUNIT_ASSERT( dynamic_cast<WFGridCartesian*>(wfm_in->State(0)) != NULL );
   CPPUNIT_ASSERT( dynamic_cast<WFGridCartesian*>(wfm_in->State(1)) != NULL );
   CPPUNIT_ASSERT( wfm_in->State(0) !=  wfm_in->State(1));
   
   p_in = wfm_in->State(0)->Params();
   p_in.GetValue("CLASS", s);
   CPPUNIT_ASSERT(s == "WFGridCartesian");
   
   p_in = wfm_in->State(1)->Params();
   p_in.GetValue("CLASS", s);
   CPPUNIT_ASSERT(s == "WFGridCartesian");

   /* Cleanup */
   CPPUNIT_ASSERT_NO_THROW(Remove("TESTMS.meta"));
   CPPUNIT_ASSERT_NO_THROW(Remove("TESTMS-0.meta"));
   CPPUNIT_ASSERT_NO_THROW(Remove("TESTMS-0.wftest"));
   CPPUNIT_ASSERT_NO_THROW(Remove("TESTMS-1.meta"));
   CPPUNIT_ASSERT_NO_THROW(Remove("TESTMS-1.wftest"));
   
   
   /* Check if sequence Naming works correct */
   file.ResetCounter();
   file.ActivateSequence();
   
   char name[256];
   FILE *fh;
   
   for (int i=0; i < 3; i++){
      CPPUNIT_ASSERT_NO_THROW(file << wfm);
      if (i==0){ /* Check for meta files */
         sprintf(name, "TESTMS.meta");
         CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
         fclose(fh);
         sprintf(name, "TESTMS-0.meta");
         CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
         fclose(fh);
         sprintf(name, "TESTMS-1.meta");
         CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
         fclose(fh);
      }
      /* check for data */
      sprintf(name, "TESTMS-0_%d.wftest", i);
      CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
      fclose(fh);
      sprintf(name, "TESTMS-1_%d.wftest", i);
      CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
      fclose(fh);
   }
   
   /* Check for re-reading the sequence */
   file.ResetCounter();
   for (int i=0; i < 3; i++){
      CPPUNIT_ASSERT_NO_THROW(file >> wfm);
   }
   
   delete wfm;
   
}






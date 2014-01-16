#include <cppunit/extensions/HelperMacros.h>

#include "FileWFTest.h"
#include "qdlib/WFGridCartesian.h"
#include "qdlib/WFMultistate.h"
#include "function_gens.h"
#include "tools/fstools.h"
#include "defs.h"

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
   file.Compress(false);
   file.Name("TEST");
   file.Suffix(".wftest");
   
   CPPUNIT_ASSERT_NO_THROW(file << psi);
         
   /* Reread single file with pre-initialized class */
   CPPUNIT_ASSERT_NO_THROW(file >> psi_in);
   
   /* both wfs should match */
   p_in = psi_in->Params();
   CPPUNIT_ASSERT( psi->size() == psi_in->size() );

   CPPUNIT_ASSERT(file.GetMeta().class_() == "WFGridCartesian");

   WFGridCartesian* psi_in_gs = dynamic_cast<WFGridCartesian*>(psi_in);
   CPPUNIT_ASSERT( psi_in_gs->Dim() == 1 );
   CPPUNIT_ASSERT( psi_in_gs->DimSize(0) ==  WF_TEST_SIZE);
   CPPUNIT_ASSERT( psi_in_gs->Xmin(0) ==  -5);
   CPPUNIT_ASSERT( psi_in_gs->Xmax(0) ==   5);
   
   /* Values should match exactly */
   for (int i=0; i < psi->size(); i++){
      CPPUNIT_ASSERT( (*psi)[i].real() == (*psi_in)[i].real() );
      CPPUNIT_ASSERT( (*psi)[i].imag() == (*psi_in)[i].imag() );
   }
   DELETE_WF(psi_in);
   
 
   /* Check class initialization by meta file */
   psi_in = NULL;
   CPPUNIT_ASSERT_NO_THROW( file >> (WaveFunction**) &psi_in );
   
   CPPUNIT_ASSERT (psi_in != NULL);
   CPPUNIT_ASSERT (psi_in->Name() == "WFGridCartesian");
   
   /* both wfs should match */
   p_in = psi_in->Params();
   CPPUNIT_ASSERT( psi->size() == psi_in->size() );

   CPPUNIT_ASSERT(file.GetMeta().class_() == "WFGridCartesian");
   
   psi_in_gs = dynamic_cast<WFGridCartesian*>(psi_in);
   CPPUNIT_ASSERT( psi_in_gs->Dim() == 1 );
   CPPUNIT_ASSERT( psi_in_gs->DimSize(0) ==  WF_TEST_SIZE);
   CPPUNIT_ASSERT( psi_in_gs->Xmin(0) ==  -5);
   CPPUNIT_ASSERT( psi_in_gs->Xmax(0) ==   5);
   
   /* Values should match exactly */
   for (int i=0; i < psi->size(); i++){
      CPPUNIT_ASSERT( (*psi)[i].real() == (*psi_in)[i].real() );
      CPPUNIT_ASSERT( (*psi)[i].imag() == (*psi_in)[i].imag() );
   }

   CPPUNIT_ASSERT_NO_THROW(FS::Remove("TEST.wftest"));
   //CPPUNIT_ASSERT_NO_THROW(FS::Remove("TEST.meta"));

   DELETE_ALL_WF();
}

#ifdef HAVE_LIBZ
/**
 * Check for Writing & Reading a ZLIB compressed WF-file.
 */
void FileWFTest::IO_Test_ZLIB()
{
   ParamContainer p, p_in;
   WaveFunction *psi, *psi_in;
   string s;

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
   file.Name("ZLIBTEST");
   file.Suffix(".wftest");
   file.Compress(true);
   file.CompressionTolerance(ROUGH_EPS * 3);
   file.CompressMethod(FileWF::ZLIB);

   CPPUNIT_ASSERT_NO_THROW(file << psi);

   /* Reread single file with pre-initialized class */
   CPPUNIT_ASSERT_NO_THROW(file >> psi_in);

   /* both wfs should match */
   p_in = psi_in->Params();
   CPPUNIT_ASSERT( psi->size() == psi_in->size() );

   WFGridCartesian* psi_in_gs = dynamic_cast<WFGridCartesian*>(psi_in);
   CPPUNIT_ASSERT( psi_in_gs->Dim() == 1 );
   CPPUNIT_ASSERT( psi_in_gs->DimSize(0) ==  WF_TEST_SIZE);
   CPPUNIT_ASSERT( psi_in_gs->Xmin(0) ==  -5);
   CPPUNIT_ASSERT( psi_in_gs->Xmax(0) ==   5);

   /* Values should match roughly due to lossy compression */
   for (int i=0; i < psi->size(); i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( (*psi)[i].real(), (*psi_in)[i].real(),  ROUGH_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( (*psi)[i].imag(), (*psi_in)[i].imag(),  ROUGH_EPS);
   }
   DELETE_WF(psi_in);

   CPPUNIT_ASSERT_NO_THROW(FS::Remove("ZLIBTEST.wftest"));

   DELETE_ALL_WF();
   file.Compress(false);
}
#endif


#ifdef HAVE_LIBBZ2
/**
 * Check for Writing & Reading a ZLIB compressed WF-file.
 */
void FileWFTest::IO_Test_BZIP()
{
   ParamContainer p, p_in;
   WaveFunction *psi, *psi_in;
   string s;

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
   file.Name("BZIPTEST");
   file.Suffix(".wftest");
   file.Compress(true);
   file.CompressionTolerance(ROUGH_EPS * 3);
   file.CompressMethod(FileWF::BZIP);
   file.Format(FileWF::binary); // BZIP is only supported for the raw binary format

   CPPUNIT_ASSERT_NO_THROW(file << psi);

   /* Reread single file with pre-initialized class */
   //try { file >> psi_in; } catch (Exception e) { cout << endl << e.GetMessage() << endl; }
   CPPUNIT_ASSERT_NO_THROW(file >> psi_in);

   /* both wfs should match */
   p_in = psi_in->Params();
   CPPUNIT_ASSERT( psi->size() == psi_in->size() );

   WFGridCartesian* psi_in_gs = dynamic_cast<WFGridCartesian*>(psi_in);
   CPPUNIT_ASSERT( psi_in_gs->Dim() == 1 );
   CPPUNIT_ASSERT( psi_in_gs->DimSize(0) ==  WF_TEST_SIZE);
   CPPUNIT_ASSERT( psi_in_gs->Xmin(0) ==  -5);
   CPPUNIT_ASSERT( psi_in_gs->Xmax(0) ==   5);

   /* Values should match roughly due to lossy compression */
   for (int i=0; i < psi->size(); i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( (*psi)[i].real(), (*psi_in)[i].real(),  ROUGH_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( (*psi)[i].imag(), (*psi_in)[i].imag(),  ROUGH_EPS);
   }
   DELETE_WF(psi_in);

   CPPUNIT_ASSERT_NO_THROW(FS::Remove("BZIPTEST.wftest"));

   DELETE_ALL_WF();
   file.Compress(false);
}
#endif



/**
 * Check for correct Multistate handling.
 * 
 * (Single & Multistate WFs)
 * Sequence reading & writing is checked here
 */
void FileWFTest::IO_Test_Multistate()
{
   ParamContainer pms, p, p_in;
   WaveFunction *psi0, *psi1, *psi0_in;
   WFMultistate *wfm, *wfm_in;
   string s;
   
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

   p.SetValue("xmax0", 6); // Assure that we can distinguish state 0 and 1
   psi1->Init(p);
   fgen_sin_norm(*psi1, -5, 6);
   *psi1 *= cexpI(M_PI/3);

   CPPUNIT_ASSERT_NO_THROW( wfm->Add(psi0, 0) );
   CPPUNIT_ASSERT_NO_THROW( wfm->Add(psi1, 1) );

   /* Setup file writer */
   file.Compress(false);
   file.Name("TESTMS");
   file.Suffix(".wftest");

   CPPUNIT_ASSERT_NO_THROW(wfm->Init(pms));

   /* Write File */
   CPPUNIT_ASSERT_NO_THROW(file << wfm);
   
   /* Re-read & check */
   wfm_in = new WFMultistate();
   psi0_in = new WFGridCartesian();
   


   psi0_in->Init(p);  /* At least one vector must be initialzed */
   
   p.clear();
   wfm_in->Add(psi0_in, 0);
   pms.SetValue("states", 2);
   wfm_in->Init(pms);
   
   CATCH_EXCEPTION(file >> wfm_in);
   CPPUNIT_ASSERT_NO_THROW (file >> wfm_in); /* Read file */
   
   /* Check MS container */
   p_in = wfm_in->Params();
   CPPUNIT_ASSERT( dynamic_cast<WFMultistate*>(wfm) != NULL );
   CPPUNIT_ASSERT( wfm->States() == 2);
   
   /* Check state 0 */

   /* both wfs should match */
   psi0_in = wfm_in->State(0);
   CPPUNIT_ASSERT( psi0->size() == psi0_in->size() );
   
   CPPUNIT_ASSERT(dynamic_cast<GridSystem*>(psi0_in)->Dim() == 1);
   CPPUNIT_ASSERT(dynamic_cast<GridSystem*>(psi0_in)->DimSize(0) == WF_TEST_SIZE);
   CPPUNIT_ASSERT(dynamic_cast<GridSystem*>(psi0_in)->Xmin(0) == -5);
   CPPUNIT_ASSERT(dynamic_cast<GridSystem*>(psi0_in)->Xmax(0) == 5);
  
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

   CPPUNIT_ASSERT(dynamic_cast<GridSystem*>(psi0_in)->Dim() == 1);
   CPPUNIT_ASSERT(dynamic_cast<GridSystem*>(psi0_in)->DimSize(0) == WF_TEST_SIZE);
   CPPUNIT_ASSERT(dynamic_cast<GridSystem*>(psi0_in)->Xmin(0) == -5);
   CPPUNIT_ASSERT(dynamic_cast<GridSystem*>(psi0_in)->Xmax(0) == 6);
   
   /* Values should match exactly */
   for (int i=0; i < psi0->size(); i++){
      CPPUNIT_ASSERT( (*psi0_in)[i].real() == (*psi1)[i].real() );
      CPPUNIT_ASSERT( (*psi0_in)[i].imag() == (*psi1)[i].imag() );
   }

   DELETE_WF(wfm_in);
   wfm_in = NULL;
  
   /* Check load-by-meta style */
   CPPUNIT_ASSERT_NO_THROW (file >> (WaveFunction**) &wfm_in);
   
   /* Quick check if contents are there */
   CPPUNIT_ASSERT( wfm_in != NULL );
   CPPUNIT_ASSERT( wfm_in->States() == 2 );
   CPPUNIT_ASSERT( dynamic_cast<WFGridCartesian*>(wfm_in->State(0)) != NULL );
   CPPUNIT_ASSERT( dynamic_cast<WFGridCartesian*>(wfm_in->State(1)) != NULL );
   CPPUNIT_ASSERT( wfm_in->State(0) !=  wfm_in->State(1));
   
   /* Cleanup */
     CPPUNIT_ASSERT_NO_THROW(FS::Remove("TESTMS.wftest"));
//   CPPUNIT_ASSERT_NO_THROW(FS::Remove("TESTMS.meta"));
//   CPPUNIT_ASSERT_NO_THROW(FS::Remove("TESTMS-0.meta"));
//   CPPUNIT_ASSERT_NO_THROW(FS::Remove("TESTMS-0.wftest"));
//   CPPUNIT_ASSERT_NO_THROW(FS::Remove("TESTMS-1.meta"));
//   CPPUNIT_ASSERT_NO_THROW(FS::Remove("TESTMS-1.wftest"));
//
   
   /* Check if sequence Naming works correct */
   file.ResetCounter();
   file.ActivateSequence();
   
   char name[256];
   FILE *fh;
   
   for (int i=0; i < 3; i++){
      CPPUNIT_ASSERT_NO_THROW(file << wfm);
//      if (i==0){ /* Check for meta files */
//         sprintf(name, "TESTMS.meta");
//         CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
//         fclose(fh);
//         sprintf(name, "TESTMS-0.meta");
//         CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
//         fclose(fh);
//         sprintf(name, "TESTMS-1.meta");
//         CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
//         fclose(fh);
//      }
      /* check for data */
      sprintf(name, "TESTMS_%d.wftest", i);
      CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );

//      sprintf(name, "TESTMS-0_%d.wftest", i);
//      CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
//      fclose(fh);
//      sprintf(name, "TESTMS-1_%d.wftest", i);
//      CPPUNIT_ASSERT( (fh = fopen(name, "r")) != NULL );
//      fclose(fh);
   }
   
   /* Check for re-reading the sequence */
   file.ResetCounter();
   for (int i=0; i < 3; i++){
      CPPUNIT_ASSERT_NO_THROW(file >> wfm);
      sprintf(name, "TESTMS_%d.wftest", i);
      CPPUNIT_ASSERT_NO_THROW(FS::Remove(name));
   }
   
   DELETE_ALL_WF();
   
}






#include "TestGetopt.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestGetopt);

using namespace QDLIB;

TestGetopt::TestGetopt()
{
}


TestGetopt::~TestGetopt()
{
}

void TestGetopt::Init()
{
   int argc;
   char **argv_empty;
   char **argv_help;
   Getopt cmdline;
   
   argv_empty = new char*[1];
   argv_empty[0] = new char[256];
   strcpy(argv_empty[0], "MyName\0");
   
   argv_help = new char*[2];
   argv_help[0] = new char[256];
   argv_help[1] = new char[256];
   strcpy(argv_help[0], "MyName\0");
   strcpy(argv_help[1], "-b\0");
   
   cmdline.ReadArgs(1, argv_empty);
   CPPUNIT_ASSERT( cmdline.CheckOptions() );
   
   cmdline.ReadArgs(2,(char**) argv_help);
   CPPUNIT_ASSERT( cmdline.CheckOptions() );
   
}

void TestGetopt::ShowHelp()
{
   int argc;
   char **argv_help;
   Getopt cmdline;
   stringstream ss;
   streambuf *buf, *orgbuf;
   string s;
   
   optind = 0;
   
   argv_help = new char*[4];
   argv_help[0] = new char[256];
   argv_help[1] = new char[256];
   argv_help[2] = new char[256];
   argv_help[3] = new char[256];
   strcpy(argv_help[0], "MyName\0");
   strcpy(argv_help[1], "-b\0");
   strcpy(argv_help[2], "-c param_is_present\0");
   strcpy(argv_help[3], "-o param_is_present\0");
   
   cmdline.SetDescription("I describe myself");
   cmdline.SetHelp('b', "Activate feature b");
   cmdline.SetHelp('c', "parameterforc", true ,"Give c a parameter", "default_valuec");
   cmdline.SetHelp('o', "parameterforo", false ,"Give o a parameter", "default_valueo");
   
   /* redirects output*/
   buf = ss.rdbuf();
   orgbuf = cerr.rdbuf();
   cerr.rdbuf(buf);
   cmdline.ShowHelp();
   cerr.rdbuf(orgbuf);     /* give it back */
   
//    cerr << ss.str();
   CPPUNIT_ASSERT(ss.str().find("Activate feature b") != string::npos);
   CPPUNIT_ASSERT(ss.str().find("parameterforc") != string::npos);
   /* -c is required - default values doesn't make sense */
   CPPUNIT_ASSERT(ss.str().find("default_valuec") == string::npos);
   /* o is not required -> default value must be present */
   CPPUNIT_ASSERT(ss.str().find("default_valueo") != string::npos);
   
   
}

void TestGetopt::CheckArgs()
{
   int argc;
   char **argv_help;
   Getopt cmdline;
   stringstream ss;
   streambuf *buf, *orgbuf;
   string s;
   
   optind = 0;
   
   argv_help = new char*[4];
   argv_help[0] = new char[256];
   argv_help[1] = new char[256];
   argv_help[2] = new char[256];
   argv_help[3] = new char[256];
   strcpy(argv_help[0], "MyName\0");
   strcpy(argv_help[1], "-b\0");
   strcpy(argv_help[2], "-c param_is_present\0");
   strcpy(argv_help[3], "-o param_is_present\0");
   
   cmdline.SetHelp('b', "Activate feature b");
   cmdline.SetHelp('c', "parameterforc", true ,"Give c a parameter", "default_valuec");
   cmdline.SetHelp('o', "parameterforo", false ,"Give o a parameter", "default_valueo");
   
   cmdline.ReadArgs(4, argv_help);
   
   CPPUNIT_ASSERT(cmdline.CheckOptions());
   
   cmdline.SetHelp('d', "d", true ,"", "1");
  
   /* redirects output*/
   buf = ss.rdbuf();
   orgbuf = cerr.rdbuf();
   cerr.rdbuf(buf);
   
   CPPUNIT_ASSERT(!cmdline.CheckOptions());  /* Should fail -> d is not present*/
   
   cerr.rdbuf(orgbuf);     /* give it back */
   
   
   
}

void TestGetopt::Values()
{
   int argc=11;
   char **argv_help;
   Getopt cmdline;
   string s;
   int i;
   double d;
   char c;
   
   optind = 0;
   
   argv_help = new char*[argc];
   argv_help[0] = new char[256];
   argv_help[1] = new char[256];
   argv_help[2] = new char[256];
   argv_help[3] = new char[256];
   argv_help[4] = new char[256];
   argv_help[5] = new char[256];
   argv_help[6] = new char[256];
   argv_help[7] = new char[256];
   argv_help[8] = new char[256];
   argv_help[9] = new char[256];
   argv_help[10] = new char[256];
   
   strcpy(argv_help[0], "MyName\0");
   strcpy(argv_help[1], "-a\0");
   strcpy(argv_help[2], "string\0");
   strcpy(argv_help[3], "-b");
   strcpy(argv_help[4], "6699");
   strcpy(argv_help[5], "-c");
   strcpy(argv_help[6], "3.14127");
   strcpy(argv_help[7], "-d");
   strcpy(argv_help[8], "c");
   strcpy(argv_help[9], "-i");
   strcpy(argv_help[10], "filename");
   
   
   cmdline.SetHelp('a', " ", true ," ", " ");
   cmdline.SetHelp('b', " ", true ," ", " ");
   cmdline.SetHelp('c', " ", true ," ", " ");
   cmdline.SetHelp('d', " ", true ," ", " ");
   cmdline.SetHelp('e', " ", false , " ", "str");
   cmdline.SetHelp('f', " ", false , " ", "7799");
   cmdline.SetHelp('g', " ", false , " ", "1.2345");
   cmdline.SetHelp('h', " ", false , " ", "rt");
   cmdline.SetHelp('i', " ");
   
   
   cmdline.ReadArgs(argc, argv_help);
   
   CPPUNIT_ASSERT(cmdline.CheckOptions());
   
   cmdline.GetOption('a', s);
   cmdline.GetOption('b', i);
   cmdline.GetOption('c', d);
   cmdline.GetOption('d', c);
   
   
   
   CPPUNIT_ASSERT(s == "string");
   CPPUNIT_ASSERT(i == 6699);
   CPPUNIT_ASSERT(d == 3.14127);
   CPPUNIT_ASSERT(c == 'c');
   
   cmdline.GetOption('e', s);
   cmdline.GetOption('f', i);
   cmdline.GetOption('g', d);
   cmdline.GetOption('h', c);
   
   CPPUNIT_ASSERT(s == "str"); /* default value */
   CPPUNIT_ASSERT(i == 7799); /* default value */
   CPPUNIT_ASSERT(d == 1.2345); /* default value */
   CPPUNIT_ASSERT(c == 'r'); /* default value */
   
   CPPUNIT_ASSERT(cmdline.GetOption('i'));
   
   CPPUNIT_ASSERT(cmdline.GetNonOption(0, s));
   CPPUNIT_ASSERT(s == "filename");
   CPPUNIT_ASSERT(!cmdline.GetNonOption(1, s));
}



// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include "tools/Exception.h"
#include "tools/Getopt.h"

#include "qdlib/FileWF.h"


using namespace QDLIB;

/**
 * Prepare commandline options & help
 * 
 */
void PrepareCmdline(Getopt &cmdline)
{
   cmdline.SetDescription("QDng wave function compression utility");
   
   cmdline.SetHelp('d', "Decompress files rather than compression");
   
   cmdline.SetHelp('s', "Treat a whole series instead of single files");
   
   cmdline.SetHelp('t', "compression tolerance", false, 
                   "Cut off tolerance for compression", "1e-5");

   cmdline.SetHelp('l', "compression level", false, 
                   "compression level (0=no compression, 9=best & slowest)", "4");

   cmdline.SetHelp('m', "compression method", false,
                   "compression method (z=zlib [default], j=bzip2)", "m");


}

/**
 * Decompress WF files.
 * 
 * \param fname basename of WF files
 * \param series single file or series?
 */
void DeCompress(string fname, bool series)
{
   FileWF wio;
   WaveFunction *WF;
   
   
   wio.Suffix(BINARY_WF_SUFFIX);
   wio.Name(fname);
   
   if (!series) {
      WF = wio.LoadWaveFunctionByMeta();
      /* Remove compression flag */
      ParamContainer &p = WF->Params();
      p.SetValue("compress", false);
      wio << WF;
   } else {
      int counter;
      wio.ActivateSequence();
      
      counter = wio.Counter();
      WF = wio.LoadWaveFunctionByMeta();
      
      /* Remove compression flag */
      ParamContainer &p = WF->Params();
      p.SetValue("compress", false);
      cout << "inflate " << counter << endl;
      wio.Counter(counter);
      wio << WF;
      
      /* Loop over rest of series */
      try {
         while (true){
            counter = wio.Counter();
            wio >> WF;
            
            cout << "inflate " << counter << endl;
            /* Remove compression flag */
            p = WF->Params();
            p.SetValue("compress", false);
            
            wio.Counter(counter);
            wio << WF;
         }
      } catch (Exception e) {
         /** \todo capture real errors here */
      }
         
   }
      
   DELETE_WF(WF);
}


/**
 * Compress WF files.
 * 
 * \param fname basename of WF files
 * \param series single file or series?
 * \param tol Cut off tolerance
 * \param level zlib compression level
 */
void Compress(string fname, bool series, double tol, int level, const string &method)
{
   FileWF wio;
   WaveFunction *WF;
   
   /* Init */
   wio.Suffix(BINARY_WF_SUFFIX);
   wio.Name(fname);
   
   wio.Compress(true);
   wio.CompressionTolerance(tol);
   wio.CompressionLevel(level);
   if (method == "z") wio.CompressMethod(FileWF::ZLIB);
   else if (method == "j") wio.CompressMethod(FileWF::BZIP);
   else wio.CompressMethod(FileWF::INVALID);
   
   if (!series) {
      WF = wio.LoadWaveFunctionByMeta();
      wio << WF;
   } else {
      int counter;
      wio.ActivateSequence();
      
      counter = wio.Counter();
      WF = wio.LoadWaveFunctionByMeta();
      
      cout << "deflate " << counter << endl;
      wio.Counter(counter);
      wio << WF;
      
      /* Loop over rest of series */
      try {
         while (true){
            counter = wio.Counter();
            wio >> WF;
            
            cout << "deflate " << counter << endl;
            
            wio.Counter(counter);
            wio << WF;
         }
      } catch (Exception e) {
         /** \todo capture real errors here */
      }

   }
   
   DELETE_WF(WF);
}


/**
 * Small commandline utility for compression/decompression of WFs.
 * 
 * 
 */
int main (int argc, char **argv)
{
   Getopt cmdline;
   string fname;
   bool series;
   int retval = EXIT_SUCCESS;
   
   PrepareCmdline(cmdline);
   
   
   try {
      cmdline.ReadArgs(argc, argv);
      cmdline.CheckOptions();
      
      /*Show the help ?*/
      if ( cmdline.GetOption('h') ) exit(retval);
      
      
      if(! cmdline.GetNonOption(0, fname) )
         throw ( EParamProblem ("No input file given") );
      
   } catch (Exception e) {
      cerr << e.GetMessage() << "\n";
      cmdline.ShowHelp();
      exit(0);
   }
   
   /* Global try-catch block */
   try {
      series = cmdline.GetOption('s');
      
      if ( cmdline.GetOption('d') ) {
         cout << "\nStarting WF decompression...\n";
         DeCompress(fname, series);
      } else {
         double tol;
         int level;
         string method;
         
         cmdline.GetOption('t', tol);
         if (tol > 1)
            throw (EParamProblem("Tolerance value larger one is nonsense"));
         
         cmdline.GetOption('l', level);
         if (level < 1 || level > 9)
            throw (EParamProblem("Compression Level must be between 0 and 9"));
         
         cmdline.GetOption('m', method);
         if (method != "z" && method != "j")
            throw (EParamProblem("Compression must be either z or j"));

         cout << "\nStarting WF compression...\n";
         Compress(fname, series, abs(tol), level, method);
      }
   } catch (Exception e) {
      cerr << "\n\n\t!!!A problematic error occured:\n\t";
      cerr << e.GetMessage() << "\n\n\n";
      cout << "Error termination\n\n";
      retval = EXIT_FAILURE;
   }
  
   exit(retval);
}


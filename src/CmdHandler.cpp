/*
 * CmdHandler.cpp
 *
 *  Created on: Jan 15, 2013
 *      Author: markus
 */

#include "CmdHandler.h"

#include <cstdlib>
#include <time.h>
#include <sys/times.h>

#include "tools/Exception.h"
#include "tools/XmlParser.h"
#include "tools/Logger.h"
#include "tools/GlobalParams.h"

#include "qdlib/FileWF.h"

#include "GlobalOpList.h"
#include "GlobalWFList.h"

#include "ChainLoader.h"
#include "ProgPropa.h"
#include "ProgFilter.h"
#include "ProgEigen.h"
#include "ProgOCT.h"

namespace QDLIB
{

   /**
    * Run XML the main programmes by XML defintions.
    */
   void CmdHandler::RunXML(const char* buf, size_t size, const string& dir)
   {
      Logger& log = Logger::InstanceRef();
      ParamContainer& gp = GlobalParams::Instance();

      XmlParser XMLfile;
      XmlNode rnodes;
      XmlNode *prognodes;

      time_t wall_time;
      struct tms proc_time, proc_time_new;

      /* Open input file and check programm nodes */
      XMLfile.Parse(buf, size);

      rnodes = XMLfile.Root();

      /* enables multiple programm nodes*/
      if (rnodes.Name() == "multi" || rnodes.Name() == "qdng") {
         /* Read out global attributes/parameters */
         gp += rnodes.Attributes();
         prognodes = rnodes.NextChild();
      } else prognodes = &rnodes;

      if (prognodes == NULL)
         throw(EParamProblem("Empty parameter file provided"));


      /* Look for global Operator definitions */
      if (prognodes->Name() == "opdefs") {
         XmlNode *opdefs = prognodes->NextChild();
         if (opdefs != NULL) {
            log.Header("Loading global operator definitions", Logger::Section);
            while (opdefs->EndNode()) {
               Operator *O;

               log.Header(opdefs->Name(), Logger::SubSection);
               log.IndentInc();
               if (opdefs->Name() == "udef") {
                  O = ChainLoader::LoadPropagator(opdefs, true);
               } else if (opdefs->Name() == "opdef") {
                  O = ChainLoader::LoadOperatorChain(opdefs, true);
               } else {
                  throw(EParamProblem("Unknown definition type: ", opdefs->Name()));
               }
               O = O;
               log.IndentDec();
               opdefs->NextNode();
               log.cout() << endl;
            }
         }
         prognodes->NextNode();

         if (log.Debug())
            log.cout() << endl;
         log.Header("Internal Operator list", Logger::SubSection);
         log.IndentInc();
         GlobalOpList::Instance().PrintList();
      }

      /* Look for global wave function definitions */
      if (prognodes->EndNode() && prognodes->Name() == "wfdefs") {
         XmlNode *wfdefs = prognodes->NextChild();
         if (wfdefs != NULL) {
            log.Header("Loading global operator definitions", Logger::Section);
            while (wfdefs->EndNode()) {
               WaveFunction *WF = NULL;

               log.Header(wfdefs->Name(), Logger::SubSection);
               log.IndentInc();
               if (wfdefs->Name() == "wfdef") {
                  WF = ChainLoader::LoadWaveFunctionChain(wfdefs);
               } else {
                  throw(EParamProblem("Unknown definition type: ", wfdefs->Name()));
               }
               WF = WF;
               log.IndentDec();
               wfdefs->NextNode();
               log.cout() << endl;
            }
         }
         prognodes->NextNode();

         if (log.Debug())
            log.cout() << endl;
         log.Header("Internal Operator list", Logger::SubSection);
         log.IndentInc();
         GlobalWFList::Instance().PrintList();
      }

      /* Loop over program nodes */
      while (prognodes->EndNode()) {
         wall_time = times(&proc_time);
         wall_time = time(NULL);
         string progname = prognodes->Name();
         if (progname == "propa") {
            ProgPropa propa(*prognodes);
            propa.SetDirectory(dir);
            log.Header("Propagation", Logger::Chapter);
            propa.Run();
         } else if (progname == "filter") {
            ProgFilter filter(*prognodes);
            filter.SetDirectory(dir);
            log.Header("Filter", Logger::Chapter);
            filter.Run();
         } else if (progname == "eigen") {
            ProgEigen eigen(*prognodes);
            eigen.SetDirectory(dir);
            log.Header("Eigenfunctions calculation", Logger::Chapter);
            eigen.Run();
         } else if (progname == "oct") {
            ProgOCT oct(*prognodes);
            oct.SetDirectory(dir);
            log.Header("Optimal control theory", Logger::Chapter);
            oct.Run();
         } else if (progname == "densmat") {
            throw(EParamProblem("Density matrix propagation not implementet yet"));
         } else {
            throw(EParamProblem("Programm type not known ", progname));
         }

         /* Show memory consumption */
         log.cout() << "\nMaximum amount of memory used: " << Memory::Format(Memory::Instance().MaximumSizeUsed()) << endl;

         /* Show time consumption */
         log.cout().precision(0);
         log.cout() << "\nWall time: " << fixed << difftime(time(NULL), wall_time) << " s";

         wall_time = times(&proc_time_new);
         log.cout() << "\tCPU time: "
                  << double(proc_time_new.tms_utime - proc_time.tms_utime) / double(sysconf(_SC_CLK_TCK)) << " s";
         log.cout() << "\tSYS time: "
                  << double(proc_time_new.tms_stime - proc_time.tms_stime) / double(sysconf(_SC_CLK_TCK)) << " s \n\n";

         prognodes->NextNode();
      }

   }

   /**
    * Initialize interactive mode with given I/O streams to exchange commands.
    */
   CmdHandler::CmdHandler(istream& in, ostream& out) : _fifo(NULL), _sin(&in), _sout(&out)
   {
   }


   /**
    * Initialize interactive mdoe with base name for FIFO file.
    */
   CmdHandler::CmdHandler(const string& file)
   {
      _fifo = new FIFO(file);
   }

   CmdHandler::CmdHandler() : _fifo(NULL), _sin(NULL), _sout(NULL)
   {

   }

   CmdHandler::~CmdHandler()
   {
      if (_fifo != NULL) delete _fifo;
   }



   /**
    * Run commands.
    *
    * accepted commands:
    *
    * \li quit end interactive mode
    * \li XML run XML code as from file input.
    *     format:
    *        XML
    *        <size>
    *        <XML input>
    *
    * \li
    */
   void CmdHandler::RunInteractive(const string& dir)
   {

      Logger& log = Logger::InstanceRef();

      string cmd;

      while (cmd != "quit"){ /* command loop */
         *_sin >> cmd;

         if (cmd == "XML"){ /* run pure XML input */
            size_t size;
            char* buf;

            *_sin >> size;
            buf = (char*) malloc(size+1);
            _sin->read(buf, size);

            log.coutdbg() << buf << endl;
            log.cout() << "Running XML input...\n";
            log.flush();

            RunXML(buf, size, dir);
            free(buf);
         } else if (cmd == "lsop") { /* list available operators */
            GlobalOpList& oplist = GlobalOpList::Instance();
            oplist.PrintList();
         } else if (cmd == "lswf") {
            GlobalWFList& wflist = GlobalWFList::Instance();
            wflist.PrintList();
         } else if (cmd == "showgp") {
            ParamContainer& pm = GlobalParams::Instance();
            *_sout << pm << endl;
         } else if (cmd == "showop") {
            GlobalOpList& oplist = GlobalOpList::Instance();
            string name;

            *_sin >> name;
            *_sout << oplist[name]->Params() << endl;
         } else if (cmd == "getwf") {
            GlobalWFList& wflist = GlobalWFList::Instance();
            FileWF io;
            string name;

            /* get name */
            *_sin >> name;

            /* get wf */
            WaveFunction* wf = wflist.GetCopy(name);

            /* Prepare for stream writing */
            io.Compress(false);
            io.SetOutputStream(*_sout);
            io.Format(FileSingle<WaveFunction>::stream);

            io << wf;

            DELETE_WF(wf);
         } else if (cmd == "setwf") {
            GlobalWFList& wflist = GlobalWFList::Instance();
            FileWF io;
            WaveFunction* wf;
            string name;

            /* get name */
            *_sin >> name;

            /* set wf */

            /* Prepare for stream reading */
            io.Compress(false);
            io.SetOutputStream(*_sout);
            io.SetInputStream(*_sin);
            io.Format(FileSingle<WaveFunction>::stream);

            wf = io.LoadWaveFunctionByMeta();

            wflist.Set(name, wf);

            DELETE_WF(wf);
         }

         log.flush();
         *_sout << "ENDOFCMD" << endl;
      }
      // Open cmd pipe line
      // loop over commands
         // run pure XML
         // Get Op
            //name
         // Get WF
         // Apply Op
         // Matel/Expec
         // Create WF
         // Create Op
         // lsop
         // lswf
         // exit
   }

} /* namespace QDLIB */

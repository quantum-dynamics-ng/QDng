/*
 * CmdHandler.cpp
 *
 *  Created on: Jan 15, 2013
 *      Author: markus
 */

#include "CmdHandler.h"

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

#include <cstdlib>
#include <time.h>
#include <sys/times.h>

namespace QDLIB
{

   void CmdHandler::SendResponse(Response& resp)
   {
      uint32_t msg_size = resp.ByteSize();

      sout_->write(reinterpret_cast<char*>(&msg_size), sizeof(msg_size));

      if (!resp.SerializeToOstream(sout_))
         throw(EIOError("Couldn't send response to last command"));

      sout_->flush();

   }

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
   CmdHandler::CmdHandler(istream& in, ostream& out) : fifo_(NULL), sin_(&in), sout_(&out)
   {
   }


   /**
    * Initialize interactive mdoe with base name for FIFO file.
    */
   CmdHandler::CmdHandler(const string& file) : buffer(NULL)
   {
      fifo_ = new FIFO(file);

      sin_ = fifo_->GetRecvStream();
      sout_ = fifo_->GetSendStream();
   }

   CmdHandler::CmdHandler() : buffer(NULL), fifo_(NULL), sin_(NULL), sout_(NULL)
   {

   }

   CmdHandler::~CmdHandler()
   {
      if (fifo_ != NULL) delete fifo_;
      if (buffer != NULL) delete[] buffer;
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

      uint32_t msg_size;
      Command cmd;
      Response resp;

      do { /* command loop */
         log.coutdbg() << "Waiting for input.." << endl;
         log.flush();
         sin_->read(reinterpret_cast<char*>(&msg_size), sizeof(msg_size));
         cout << "msg_size: " << msg_size << endl;

         buffer = new char[msg_size];
         sin_->read(reinterpret_cast<char*>(buffer), msg_size);

         if ( !cmd.ParseFromArray(buffer, msg_size) )
            throw (EParamProblem("Parsing the command from input failed"));

         delete[] buffer; buffer = NULL;

         switch (cmd.cmd()){
            case Command_command_t_RUN_PROG:
            {
               log.coutdbg() << "Received RUN_PROG" << endl;
               log.flush();

               resp.Clear();

               if (!cmd.has_xml())
                  throw (EParamProblem("RUN_PROG is missing the XML input"));

               try {
                  RunXML(cmd.xml().c_str(), cmd.xml().size(), dir);
                  resp.set_response(Response_response_t_OK);
                  SendResponse(resp);
               } catch (Exception& e) {
                  resp.set_response(Response_response_t_ERROR_MSG);
                  resp.set_msg(e.GetMessage());
                  SendResponse(resp);
                  throw (e);
               }
            }
               break;
            case Command_command_t_READ_WF:
            {
               if (!cmd.has_param1())
                  throw (EParamProblem("READ_WF is missing a file name"));

               resp.Clear();

               FileWF file;
               WaveFunction* wf;

               // Load WF;
               file.Name(cmd.param1());

               try {
                  wf = file.LoadWaveFunctionByMeta();
               } catch (Exception& e) {
                  resp.set_response(Response_response_t_ERROR_MSG);
                  resp.set_msg(e.GetMessage());
                  SendResponse(resp);
                  throw (e);
               }

               // Write to stream
               file.Compress(false);
               file.SetOutputStream(*sout_);
               file.Format(FileSingle<WaveFunction>::stream);

               // The format in the stream is as follows:
               // In case of success the file is sent which can
               // be detected by the file header
               // In case of an error a message is sent: msg_len, Response.
               try {
                  file.WriteWaveFunction(wf);
                  sout_->flush();
               }  catch (Exception& e) {
                  resp.set_response(Response_response_t_ERROR_MSG);
                  resp.set_msg(e.GetMessage());
                  SendResponse(resp);
                  throw (e);
               }

               DELETE_WF(wf);
            }
               break;
            case Command_command_t_WRITE_WF:
            {
               if (!cmd.has_param1())
                  throw (EParamProblem("WRITE_WF is missing a file name"));

               FileWF file;
               WaveFunction* wf;
               resp.Clear();

               // Read from Stream;
               file.SetInputStream(*sin_);

               try {
                  wf = file.LoadWaveFunctionByMeta();
               } catch (Exception& e) {
                  resp.set_response(Response_response_t_ERROR_MSG);
                  resp.set_msg(e.GetMessage());
                  SendResponse(resp);
                  throw (e);
               }

               // Write to file
               try {
                  file.Name(cmd.param1());
                  file.Format(FileSingle<WaveFunction>::stream);
                  file.WriteWaveFunction(wf);
               } catch (Exception& e) {
                  resp.set_response(Response_response_t_ERROR_MSG);
                  resp.set_msg(e.GetMessage());
                  SendResponse(resp);
                  throw (e);
               }

               resp.set_response(Response_response_t_OK);
               SendResponse(resp);

               DELETE_WF(wf);
            }
               break;
            case Command_command_t_QUIT:
               log.coutdbg() << "Received QUIT" << endl;
               log.flush();
               break;
            default:
            break;
         }
      } while  (cmd.cmd() != Command_command_t_QUIT);

   }

} /* namespace QDLIB */

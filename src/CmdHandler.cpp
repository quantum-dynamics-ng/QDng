// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * CmdHandler.cpp
 *
 *  Created on: Jan 15, 2013
 *      Author: markus
 */

#include "CmdHandler.h"

#include "fft/FFTGlobal.h"
#include "modules/ModuleLoader.h"
#include "qdlib/Serializable.h"

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
#include "ProgCfun.h"

#include <cstdlib>
#include <time.h>
#include <sys/times.h>

namespace QDLIB
{

   void CmdHandler::send_response(Response& resp)
   {
      uint32_t msg_size = resp.ByteSize();

      sout_->write(reinterpret_cast<char*>(&msg_size), sizeof(msg_size));

      if (!resp.SerializeToOstream(sout_))
         throw(EIOError("Couldn't send response to last command"));

      sout_->flush();

   }

   void CmdHandler::send_OK(bool data_follows)
   {
      Response resp;
      resp.set_response(Response_response_t_OK);
      resp.set_data_follows(data_follows);
      send_response(resp);
   }

   void CmdHandler::send_error_message(const string& msg)
   {
      Response resp;
      resp.set_response(Response_response_t_ERROR_MSG);
      resp.set_msg(msg);
      resp.set_data_follows(false);
      send_response(resp);
   }


   /**
    * Run XML the main programmes by XML defintions.
    */
   void CmdHandler::RunXML(const char* buf, size_t size, const string& def_dir)
   {
      string dir(def_dir);
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

      /* overide path from command line */
      if (!dir.empty())
         gp.SetValue("dir", dir);

      if (gp.isPresent("dir")) {
         gp.GetValue("dir", dir);
         if (dir[dir.length() - 1] != '/' && !dir.empty())
            dir += "/";
         gp.SetValue("dir", dir);
      }

      /* Print global parameters */
      if (gp.Size() > 0) {
         log.cout() << endl;
         log.Header("Global Parameters", Logger::SubSection);
         log.cout() << gp;
         log.cout() << endl;
         log.flush();
      }

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
         } else if (progname == "cfunc") {
            ProgCfun cfunc(*prognodes);
            cfunc.SetDirectory(dir);
            log.Header("Multipoint Correlation Function", Logger::Chapter);
            cfunc.Run();
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
      // this is a dummy, make sure fft handler is up early.
      // fixes the problem of segfaults in the destruction process.
      FFTGlobal::Instance();
   }


   /**
    * Initialize interactive mdoe with base name for FIFO file.
    */
   CmdHandler::CmdHandler(const string& file) : buffer(NULL)
   {
      // this is a dummy, make sure fft handler is up early.
      // fixes the problem of segfaults in the destruction process.
      FFTGlobal::Instance();

      fifo_ = new FIFO(file);

      sin_ = fifo_->GetRecvStream();
      sout_ = fifo_->GetSendStream();
   }

   CmdHandler::CmdHandler() : buffer(NULL), fifo_(NULL), sin_(NULL), sout_(NULL)
   {
      // this is a dummy, make sure fft handler is up early.
      // fixes the problem of segfaults in the destruction process.
      FFTGlobal::Instance();

   }

   CmdHandler::~CmdHandler()
   {
      if (fifo_ != NULL) delete fifo_;
      if (buffer != NULL) delete[] buffer;
   }


   Operator* CmdHandler::get_operator_from_command(Command& cmd)
   {
      try {
         if (cmd.has_xml()) { // load by XML definition
            XmlParser XMLfile;
            Operator* op;

            XMLfile.Parse(cmd.xml().c_str(), cmd.xml().size());

            XmlNode& root = XMLfile.Root();
            op = ChainLoader::LoadOperatorChain(&root, true);

            if (op == NULL)
               throw(EIncompatible("Operator load from command failed"));

            return op;
         } else if (cmd.has_param_s1()) { // get it from reference
            GlobalOpList& gol = GlobalOpList::Instance();
            Operator* op = gol[cmd.param_s1()];

            if (op == NULL)
               throw(EIncompatible("Operator load from command by reference failed"));

            return op;
         } else {
            throw (EParamProblem("Can't load operator, neither XML nor reference is given."));
         }

      } catch (Exception& e) {
         send_error_message(e.GetMessage());
      }

      return NULL;
   }

   /**
    * Get a wf from stream.
    *
    * Shortcut with error response message.
    */
   WaveFunction* CmdHandler::get_wf(FileWF& file)
   {
      WaveFunction* wf = NULL;

      try {
         wf = file.LoadWaveFunctionByMeta();
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
      }

      return wf;
   }

   /**
    * put a wf the the com stream.
    *
    * Shortcut with error response message.
    */
   void CmdHandler::put_wf_to_stream(FileWF& file, WaveFunction* wf)
   {
      try {
         file.WriteWaveFunction(wf);
         sout_->flush();
      }  catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw (e);
      }
   }

   void CmdHandler::cmd_run_prog(Command& cmd, const string& dir)
   {
      if (!cmd.has_xml()){
         send_error_message("RUN_PROG is missing the XML input");
         return;
      }

      try {
         RunXML(cmd.xml().c_str(), cmd.xml().size(), dir);
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw (e);
      }

      send_OK(false);
   }

   void CmdHandler::cmd_read_wf(Command& cmd)
   {
      if (!cmd.has_param_s1()){
         send_error_message("READ_WF is missing a file name");
         return;
      }

      send_OK(true);

      FileWF file;


      // Load WF;
      file.Name(cmd.param_s1());

      WaveFunction* wf = get_wf(file);

      // Write to stream
      file.Compress(false);
      file.SetOutputStream(*sout_);
      file.Format(FileSingle<WaveFunction>::stream);

      // The format in the stream is as follows:
      // In case of success the file is sent which can
      // be detected by the file header
      // In case of an error a message is sent: msg_len, Response.

      put_wf_to_stream(file, wf);

      DELETE_WF(wf);
   }


   void CmdHandler::cmd_write_wf(Command& cmd)
   {
      if (!cmd.has_param_s1()){
         send_error_message("WRITE_WF is missing a file name");
         return;
      }

      send_OK(false);

      FileWF file;

      // Read from Stream;
      file.SetInputStream(*sin_);

      WaveFunction* wf = get_wf(file);
      if (wf == NULL) return;

      // Set compression
      if (cmd.has_param_s2()){
         if (cmd.param_s2() == "gzip")
            file.Compress(true);
      }

      // Write to file
      try {
         file.Name(cmd.param_s1());
         file.Format(FileSingle<WaveFunction>::stream);
         file.WriteWaveFunction(wf);
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw (e);
      }

      send_OK(false);
      DELETE_WF(wf);
   }

   void CmdHandler::cmd_read_op(Command& cmd)
   {
      Response resp;

      if (!cmd.has_param_s1()){
         send_error_message("READ_OP is missing a file name");
         return;
      }


      if (!cmd.has_param_s2()){
         send_error_message("READ_OP is missing a class name");
         return;
      }

      Operator* op = ModuleLoader<Operator>::Instance()->Load(cmd.param_s2());

      if (op == NULL)
         throw(EParamProblem("Operator module not found"));


      Serializiable* ser = dynamic_cast<Serializiable*>(op);

      if (ser == NULL)
         throw(EIncompatible("Not a serializable object", op->Name()));

      // Read Operator from file
      FileSingle<Serializiable> file;

      file.Name(cmd.param_s1());

      try {
         file.ReadSingleFileFromStream(ser);
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw(e);
      }

      file.Compression(FileSingle<Serializiable>::UNCOMPRESSED);
      file.SetOutputStream(*sout_);

      // Write operator to stream
      try {
         file.WriteSingleFileToStream(ser);
         sout_->flush();
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw(e);
      }

      DELETE_OP(op);
   }

   void CmdHandler::cmd_write_op(Command& cmd)
   {
      if (!cmd.has_param_s1()){
         send_error_message("WRITE_OP is missing a file name");
         return;
      }


      if (!cmd.has_param_s2()){
         send_error_message("WRITE_OP is missing a class name");
         return;
      }

      // Load respective class
      Operator* op=NULL;
      Laser* laser=NULL;
      Serializiable* ser;

      if (cmd.param_s2() == "Laser") {
         laser = new Laser();
         ser = dynamic_cast<Serializiable*>(laser);
      } else {
         string op_name(cmd.param_s2());
         if (op_name.length() == 0)
            throw(EParamProblem("Empty operator name"));

         if (op_name[0] == 'O') op_name.erase(0, 1); // Kill the leading O

         op = ModuleLoader<Operator>::Instance()->Load(op_name);

         if (op == NULL)
            throw(EParamProblem("Operator module not found"));

         ser = dynamic_cast<Serializiable*>(op);

         if (ser == NULL)
            throw(EIncompatible("Not a serializable object", op->Name()));
      }

      send_OK(false);

      FileSingle<Serializiable> file;

      // Read from Stream;
      file.SetInputStream(*sin_);

      try {
         file.ReadSingleFileFromStream(ser);
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw (e);
      }

      // Set compression
      if (cmd.has_param_s3()){
         if (cmd.param_s3() == "gzip")
            file.Compression(FileSingle<Serializiable>::ZLIB);
      }

      // Write to file
      try {
         file.Name(cmd.param_s1() + BINARY_O_SUFFIX);
         file.Format(FileSingle<Serializiable>::stream);
         file.WriteSingleFileToStream(ser);
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw (e);
      }

      send_OK(false);

      if (op != NULL)
         DELETE_OP(op);
      else
         delete laser;
   }

   void CmdHandler::cmd_set_global_params(Command& cmd)
   {
      Response resp;

      if (!cmd.has_param_s1()){
         send_error_message("SET_GLOBAL_PARAMS is missing key/values");
      }

      ParamContainer pm;
      pm.Parse(cmd.param_s1());

      ParamContainer& gp = GlobalParams::Instance();
      gp += pm;  // Add to paraemters

      string s;
      gp.Write(s, false);

      send_OK(false);
   }

   void CmdHandler::cmd_get_global_params(Command& cmd)
   {
      ParamContainer& gp = GlobalParams::Instance();

      string s;
      gp.Write(s, false);

      Response resp;
      resp.set_msg(s);

      send_response(resp);
   }

   /**
    * Clear operator storage.
    */
   void CmdHandler::cmd_clear_objects()
   {
      GlobalOpList::Instance().Clear();
      Collector<QDClock>::Instance()->Delete();
      send_OK(false);
   }

   void CmdHandler::cmd_load_op(Command& cmd)
   {
      if (!cmd.has_xml())
         send_error_message("LOAD_OP is missing a XML operator definition");

      get_operator_from_command(cmd);

      send_OK(false);
   }

   void CmdHandler::cmd_load_U(Command& cmd)
   {
      if (!cmd.has_xml())
         send_error_message("LOAD_U is missing a XML operator definition");

      if (!cmd.has_param_d1())
         send_error_message("LOAD_U is missing a time step");

      if (!cmd.has_param_d2())
         send_error_message("LOAD_U is missing  Nt");

      try {
          XmlParser XMLfile;
          OPropagator* U;

          XMLfile.Parse(cmd.xml().c_str(), cmd.xml().size());

          XmlNode& root = XMLfile.Root();
          U = ChainLoader::LoadPropagator(&root, true);
          if (U == NULL)
             throw(EIncompatible("Propagator load from command failed"));

          // Keep track of the clock for later deletion.
          QDClock* cl = new QDClock(int(cmd.param_d2()), cmd.param_d1());
          Collector<QDClock>::Instance()->Register(cl, false);

          U->Clock( cl );

       } catch (Exception& e) {
          send_error_message(e.GetMessage());
          throw (e);
       }

      send_OK(false);
   }

   void CmdHandler::cmd_apply_op(Command& cmd)
   {
      // Load Operator
      Operator* Op = get_operator_from_command(cmd);

      // Chose time step
      if (cmd.has_param_d1()){
         QDClock* cl = Op->Clock();
         if (cl == NULL) {
            string err = "Operator has no clock";
            send_error_message(err);
            return;
         }

         int t = int(cmd.param_d1());
         if (t < 0 || t > cl->Steps()){
            string err = "Invalid time value chosen";
            send_error_message(err);
            return;
         }
         cl->TimeStep(t);
      }

      send_OK(true);

      // Receive WF
      FileWF file;

      file.SetInputStream(*sin_);

      WaveFunction* wf = get_wf(file);

      // Send Response
      try {
         if (!Op->Valid(wf)) Op->Init(wf);
         if (!Op->Valid(wf)) {
            send_error_message("Operator initialized with incompatible wfs");
            return;
         }
         Op->Apply(wf);
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw(e);
      }

      // Send back WF
      file.SetOutputStream(*sout_);
      put_wf_to_stream(file, wf);

      DELETE_WF(wf);
   }

   void CmdHandler::cmd_get_matel(Command& cmd)
   {
      // Load Operator
      Operator* Op = get_operator_from_command(cmd);

      // Get bra and ket
      FileWF file;
      file.SetInputStream(*sin_);

      WaveFunction* bra = get_wf(file);
      WaveFunction* ket = get_wf(file);

      // Send Response
      try {
         if (!Op->Valid(bra)) Op->Init(bra);
         if (!Op->Valid(bra) || !Op->Valid(ket)) {
            send_error_message("Operator initialized with incompatible wfs");
            return;
         }
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
         throw(e);
      }

      // Send response
      dcomplex res = Op->MatrixElement(bra, ket);

      Response resp;
      resp.set_response(Response_response_t_OK);
      resp.add_result(res.real());
      resp.add_result(res.imag());
      send_response(resp);

      DELETE_WF(bra);
      DELETE_WF(ket);
   }

   void CmdHandler::cmd_get_expec(Command& cmd)
   {
      // Load Operator
      Operator* Op = get_operator_from_command(cmd);

      send_OK(false);

      // Get bra/ket
      FileWF file;
      file.SetInputStream(*sin_);

      WaveFunction* bk = get_wf(file);

      // Send Response
      try {
         if (!Op->Valid(bk)) Op->Init(bk);
         if (!Op->Valid(bk)) {
            send_error_message("Operator initialized with incompatible wfs");
            return;
         }
      } catch (Exception& e) {
         send_error_message(e.GetMessage());
      }

      double res = Op->Expec(bk);
      Response resp;
      resp.set_response(Response_response_t_OK);
      resp.add_result(res);
      send_response(resp);
   }

   /**
    * Run commands.
    *
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
         log.coutdbg() << "msg_size: " << msg_size << endl;

         buffer = new char[msg_size];
         sin_->read(reinterpret_cast<char*>(buffer), msg_size);

         if ( !cmd.ParseFromArray(buffer, msg_size) )
            throw (EParamProblem("Parsing the command from input failed"));

         log.cout() << "cmd_type: "
                  << cmd.command_t_descriptor()->FindValueByNumber(cmd.cmd())->name() << endl;

         log.flush();

         delete[] buffer; buffer = NULL;

         switch (cmd.cmd()){
            case Command_command_t_RUN_PROG:
               cmd_run_prog(cmd, dir);
               break;
            case Command_command_t_READ_WF:
               cmd_read_wf(cmd);
               break;
            case Command_command_t_WRITE_WF:
               cmd_write_wf(cmd);
               break;
            case Command_command_t_GET_GLOBAL_PARAMS:
               cmd_get_global_params(cmd);
               break;
            case Command_command_t_SET_GLOBAL_PARAMS:
               cmd_set_global_params(cmd);
               break;
            case Command_command_t_CLEAR_OBJECTS:
               cmd_clear_objects();
               break;
            case Command_command_t_LOAD_OP:
               cmd_load_op(cmd);
               break;
            case Command_command_t_LOAD_U:
               cmd_load_U(cmd);
               break;
            case Command_command_t_READ_OP:
               cmd_read_op(cmd);
               break;
            case Command_command_t_WRITE_OP:
               cmd_write_op(cmd);
               break;
            case Command_command_t_QUIT:
               log.coutdbg() << "Received QUIT" << endl;
               log.flush();
               break;
            case Command_command_t_GET_EXPEC:
               cmd_get_expec(cmd);
               break;
            case Command_command_t_GET_MATEL:
               cmd_get_matel(cmd);
               break;
            case Command_command_t_APPLY_OP:
               cmd_apply_op(cmd);
               break;
            default:
               throw(EParamProblem("Unknown command code: ", cmd.cmd()));
            break;
         }

         log.cout() << "completed\n";
         log.flush();
      } while  (cmd.cmd() != Command_command_t_QUIT);

   }

} /* namespace QDLIB */

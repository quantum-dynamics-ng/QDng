/*
 * ProgFilter.cpp
 *
 *  Created on: 14.06.2011
 *      Author: mko
 */

#include "ProgFilter.h"
#include "GlobalOpList.h"
#include "Reporter.h"
#include "tools/QDGlobalClock.h"
#include "tools/fstools.h"
#include "tools/Logger.h"
#include "tools/KeyValFile.h"
#include "qdlib/FileWF.h"


namespace QDLIB
{

   ProgFilter::ProgFilter(XmlNode &FilterNode) : _FilterNode(FilterNode)
   {


   }

   ProgFilter::~ProgFilter()
   {
      /* remove the clock */
      QDGlobalClock::Destroy();
      GlobalOpList::Instance().Clear();
   }

   void ProgFilter::Init()
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      ParamContainer attr;
      Logger& log = Logger::InstanceRef();

      attr = _FilterNode.Attributes();

      /* Init propagation output dir */
      if (attr.isPresent("dir") && _dir.empty())
      {
         attr.GetValue("dir", _dir);
         if (_dir[_dir.length() - 1] != '/' && !_dir.empty())
            _dir += "/";
      }
      FS::CreateDir(_dir);

      /* path to propagation */
      if ( attr.isPresent("path") )
         attr.GetValue("path", _path);
      else
         throw (EParamProblem("Path is Missing"));


      /* Read the Propagation.meta */
      KeyValFile propmetafile(_path + "/Propagation.meta");
      ParamContainer p;
      string s;
      int i;
      double d;

      if ( !propmetafile.Parse(p) )
         throw(EIncompatible("Can't parse Propgation.meta"));


      p.GetValue("CLASS", s);
      if ( s != "Propagation" )
         throw(EIncompatible("Can't parse Propgation.meta: Wrong class"));

      p.GetValue("Nt", i );
      clock->Steps(i);

      p.GetValue("dt", d);
      clock->Dt(d);

      /* Rescale the by write cycle */
      p.GetValue("Wcycle", i );
      clock->Dt(clock->Dt() * double(i));
      clock->Steps(clock->Steps() / double(i));

      p.GetValue("WFBaseName", _fname );


      /* Summarize the configuration */
      log.cout() << "Number of time steps: " << clock->Steps() << endl;
      log.cout() << "Time step : " << clock->Dt() << endl;
      log.coutdbg() << "Write cycle was : " << i << endl;
      log.cout() << "WF base name: " << _fname << endl;
      log.cout() << "Directory for input: " << _path << endl;
      if (! _dir.empty())
         log.cout() << "Directory for output: " << _dir << endl;

      log.cout() << "\n\n";
      log.flush();
   }

   void ProgFilter::Run()
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      Logger& log = Logger::InstanceRef();

      log.Header( "Initializion", Logger::Section);

      Init();

      /* Try to open first wave function */
      WaveFunction* WF;
      FileWF wffiles;
      wffiles.ActivateSequence();
      wffiles.Name(_path + "/" + _fname);
      wffiles.Suffix(BINARY_WF_SUFFIX);

      wffiles >> &WF;

      XmlNode *ContentNodes = _FilterNode.NextChild();

      /* Post step filters */
      XmlNode *section = ContentNodes->FindNode( "filterdef" );
      if (section != NULL)
      {
         string s(_dir + DEFAULT_EXPEC_FILENAME);
         log.Header("Define filters", Logger::SubSection);
         log.IndentInc();
         _filter.SetDefaultName(s);
         _filter.Init(section);
         log.IndentDec();
         log.cout() << endl;
         log.flush();
         delete section;
      }

      log.Header("Run the filter chain", Logger::SubSection);

      /* Init Reporter */
      Reporter report;
      report.PsiInitial(WF);

      /* Do the first step */
      _filter.Apply(WF);
      report.Analyze(WF);
      ++(*clock);

      /* Loop file series */
      for(int i=0; i < clock->Steps(); i++){
         wffiles >> WF;
         _filter.Apply(WF);
         report.Analyze(WF);
         ++(*clock);
      }

      report.Finalize();
   }

}

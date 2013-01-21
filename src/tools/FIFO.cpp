/*
 * FIFO.cpp
 *
 *  Created on: Jan 16, 2013
 *      Author: markus
 */

#include "FIFO.h"
#include "GlobalParams.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>


#include "Exception.h"
#include "tools/fstools.h"

namespace QDLIB
{

   void FIFO::_Open()
   {
      /* Create two new FIFO channels */
      int ret = mkfifo((_fname + "rx").c_str(), S_IRUSR | S_IWUSR);

      if (ret != 0)
         throw(EIOError(ret, _fname));

      ret = mkfifo((_fname + "tx").c_str(), S_IRUSR | S_IWUSR);

      if (ret != 0)
         throw(EIOError(ret, _fname));

      /* Open the FIFO
       * Since a FIFO is unidirectional according to posix standard we have to open two.
       * Nevertheless both are openend as read AND write. This avoids the effect that
       * the open of the read-pipe is blocking until the other end is openend.
       */
      _istream.open((_fname + "rx").c_str(), fstream::in | fstream::out);

      if (! _istream.is_open())
         throw (EIOError("Failed to open FIFO", _fname+"rx") );

      _ostream.open((_fname + "tx").c_str(), fstream::in | fstream::out);

      if (! _ostream.is_open())
         throw (EIOError("Failed to open FIFO", _fname+"tx") );
   }

   /**
    * Opens a FIFO immedatly with temporary unique name.
    */
   FIFO::FIFO()
   {
      string tmpdir;
      ParamContainer& gp = GlobalParams::Instance();

      /* Look for the tmp path */
      if (gp.isPresent("tmpdir")){
         gp.GetValue("tmpdir", tmpdir);
      } else {
         char *c;
         c = getenv("TMPDIR");
         if (c != NULL)
            tmpdir = c;
      }

      /* Create a unique name */
      if ( tmpdir.length() > 0 )
         _fname = tmpdir + "/" + QDNG_FIFO_BASENAME;
      else
         _fname = QDNG_FIFO_BASENAME;

      _Open();
   }

   /**
    * Create a new FIFO by a distinct file name.
    */
   FIFO::FIFO(const string& path)
   {
      _fname = path;

      _Open();
   }

   FIFO::~FIFO()
   {
      /* Clean up */

      _istream.close();
      _ostream.close();
      FS::Remove(_fname+"rx");
      FS::Remove(_fname+"tx");
   }

} /* namespace QDLIB */

#ifndef QDLIBFILEWF_H
#define QDLIBFILEWF_H

#include "tools/FileSingle.h"

namespace QDLIB {

   /**
         @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class FileWF : public FileSingle<WaveFunction>
   {
      public:
         FileWF();
         ~FileWF();
         WaveFunction* LoadWaveFunctionByMeta();
         virtual void ReadFile (WaveFunction *data);
         virtual void WriteFile (WaveFunction *data);
         
         /** Stream operator for file writing. */
         virtual void operator<<(WaveFunction *data){ WriteFile(data); }
         
   };

   void operator>>(FileWF &file,  WaveFunction *data);
   
   void operator>>(FileWF &file,  WaveFunction **data);

}


#endif

#ifndef QDLIBFILEWF_H
#define QDLIBFILEWF_H

#include "tools/FileSingle.h"
#include "qdlib/WaveFunction.h"

#define FILEWF_COMPRESSION false
#define FILEWF_COMPRESSION_TOLERANCE 1e-4

namespace QDLIB {

   /**
         @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class FileWF : public FileSingle<WaveFunction>
   {
      private:
         bool _compress;         /* Use compression on write */
         int _compLevel;         /* Compression level 0..9; 0 = no compression, 9 = best */
         double _compTolerance;   /* Cut off factor for lossy compression */
         cVec *_buf;
         
         void CheckBuf(WaveFunction *data);
         void _ReadFromFile(WaveFunction *data);
      public:
         FileWF();
         ~FileWF();
         WaveFunction* LoadWaveFunctionByMeta();
         virtual void ReadFile (WaveFunction *data);
         virtual void WriteFile (WaveFunction *data);
         
         /** Stream operator for file writing. */
         virtual void operator<<(WaveFunction *data){ WriteFile(data); }
         
         /** Get file compression mode. */
         bool Compress() const { return _compress; }
         
         /** Set file compression. */
         void Compress(bool on) { _compress = on; }
         
         /** Get file compression level */
         int CompressionLevel() const { return _compLevel ; }
         
         /**
          * Set file compression.
          * 
          * This is same as for gzip: 0 means no compression,  9 means best.
          */
         void CompressionLevel(int level) {
            if (level < 0 || level > 9)
               throw (EParamProblem("Invalid compression level: ", level) ) ;
            _compLevel = level;
         }
         
         /** Get compression tolerance. */
         double CompressionTolerance() const { return _compTolerance; }
         
         /**
          * Set compression tolerance.
          * 
          * This defines a cut-off value for lossy compression.
          * The exact interpretation is up to the specific WF-class.
          */
         void CompressionTolerance(double tolerance) { _compTolerance = tolerance; }

   };

   void operator>>(FileWF &file,  WaveFunction *data);
   
   void operator>>(FileWF &file,  WaveFunction **data);

}


#endif

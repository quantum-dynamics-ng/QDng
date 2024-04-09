// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBFILEWF_H
#define QDLIBFILEWF_H

#include "tools/FileSingle.h"
#include "qdlib/WaveFunction.h"

#define FILEWF_COMPRESSION false
#define FILEWF_COMPRESSION_TOLERANCE 1e-6

namespace QDLIB {

   /**
         @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class FileWF : public FileSingle<WaveFunction>
   {
      private:
         int _compLevel;         /* Compression level 0..9; 0 = no compression, 9 = best */
         double _compTolerance;   /* Cut off factor for lossy compression */

      public:
         FileWF();
         virtual ~FileWF();
         WaveFunction* LoadWaveFunctionByMeta();
         virtual void ReadWaveFunction (WaveFunction *data);
         virtual void WriteWaveFunction (WaveFunction *data, bool more_files_follow = false);
         
         /** Stream operator for file writing. */
         virtual void operator<<(WaveFunction *data){ WriteWaveFunction(data); }

         /** Get file compression mode. */
         bool Compress() const { if ( _compMethod == UNCOMPRESSED) return false; else return true; }

         void CompressMethod(compMethod_t method){
            _compMethod = method;
         }

         /** Set file compression. */
         void Compress(bool on) { if (on) _compMethod = ZLIB; else _compMethod = UNCOMPRESSED; }
         
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

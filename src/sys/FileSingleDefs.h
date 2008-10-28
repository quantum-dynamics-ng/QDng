#ifndef FILESINGLEDEFS_H
#define FILESINGLEDEFS_H


#include "FileSingle.h"

#include "qdlib/WaveFunction.h"
#include "qdlib/OHerMat.h"



namespace QDLIB {
   typedef FileSingle<WFLevel> FileWF;
   typedef FileSingle<OHerMat> FileOHerMat;
}


#endif /* #ifndef FILESINGLEDEFS_H */

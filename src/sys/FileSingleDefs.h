#ifndef FILESINGLEDEFS_H
#define FILESINGLEDEFS_H


#include "FileSingle.h"

#include "qdlib/WaveFunction.h"
#include "qdlib/OGridsystem.h"

#include "qdlib/OHerMat.h"



namespace QDLIB {
   typedef FileSingle<WaveFunction> FileWF;
   
   typedef FileSingle<OGridSystem> FileOGrid;
	 
   typedef FileSingle<OHerMat> FileOHerMat;
}


#endif /* #ifndef FILESINGLEDEFS_H */

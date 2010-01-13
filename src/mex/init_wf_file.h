#ifndef INIT_WF_FILE_H
#define INIT_WF_FILE_H

#include "mex.h"

#include "mex/libs/ObjectHandle.h"
#include "mex/libs/Collector.h"
#include "mex/libs/convert_wf_mxArray.h"

#include <iostream>
#include <string>
#include <math.h>

#include "tools/Exception.h"
#include "tools/FileSingleDefs.h"
#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/WFMultistate.h"

extern void loadWF(char *string_File_name , char *string_WF_type);
extern void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] );



#endif
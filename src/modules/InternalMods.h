// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBINTERNALMODS_H
#define QDLIBINTERNALMODS_H

#include "qdlib/Operator.h"

namespace QDLIB
{

   typedef WaveFunction* (instWF)();
   typedef Operator* (instOP)();
   
   
   extern instWF* InternalWF(const string &name);
   extern instOP* InternalOP(const string &name);

}

#endif

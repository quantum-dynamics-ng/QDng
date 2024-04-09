// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef FILESINGLEDEFS_H
#define FILESINGLEDEFS_H


#include "FileSingle.h"

#include "qdlib/WaveFunction.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OHermitianMatrix.h"

namespace QDLIB {
   typedef FileSingle<OHermitianMatrix> FileOHerMat;
}


#endif /* #ifndef FILESINGLEDEFS_H */

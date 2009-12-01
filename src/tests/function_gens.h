#ifndef FUNCTION_GENS_H
#define FUNCTION_GENS_H

#include "math/typedefs.h"

using namespace QDLIB;

extern void fgen_sin(cVec &vec, double min, double max);
extern void fgen_sin_norm(cVec &vec, double min, double max);
extern void fgen_cos_norm(cVec &vec, double min, double max);

#endif
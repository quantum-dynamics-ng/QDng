#include "OGobbler.h"

namespace QDLIB {

OGobbler::OGobbler()
 : OKSpace(), _name(OGobbler)
{
}


OGobbler::~OGobbler()
{
}


}

Operator * QDLIB::OGobbler::NewInstance()
{
   OGobbler *r;
   
   r = new OGobbler;
   int _dims[MAX_DIMS];
   return r;
}

const string & QDLIB::OGobbler::Name()
{
   return _name;
}



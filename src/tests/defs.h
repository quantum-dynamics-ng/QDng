#ifndef DEFS_H
#define DEFS_H

#define TIGHT_EPS 1e-30
#define ROUNDOFF_EPS 1e-14
#define LOOSE_EPS 1e-10
#define ROUGH_EPS 1e-6

#define CATCH_EXCEPTION( LINE ) \
  try { LINE; } catch (Exception& e) { cout << __FILE__ << ": line " << __LINE__ <<": "<< e.GetMessage() << endl; }



#endif

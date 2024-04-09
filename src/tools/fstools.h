// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBFSTOOLS_H
#define QDLIBFSTOOLS_H

#include "Exception.h"



namespace QDLIB {
   

   /** Filesystem helper class. */
   class FS {
      public:
	 static void StripLast(string& dir);
	 static void CreateDir(const string& dir);
	 static void Remove(const string& name);
	 static bool IsDir(const string& name);
	 static bool IsFile(const string& name);
   };
   
}


#endif

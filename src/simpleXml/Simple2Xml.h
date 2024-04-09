// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDSXMLSIMPLE2XML_H
#define QDSXMLSIMPLE2XML_H
#include <string>

using namespace std;

namespace QDSXML {

   

   
   /**
    *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */

   typedef enum {SIMPLE, XML, UNKNOWN} FileType;
   
   FileType CheckType(const string &name);
   void Convert(const string &iname, const string &oname);
   void Parse(const string &iname, string &buf);
}

#endif

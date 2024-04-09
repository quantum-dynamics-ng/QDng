// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * qdng_info.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: markus
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>


#include "tools/Getopt.h"

#include "tools/FileSingle.h"


using namespace std;
using namespace QDLIB;

int main(int argc, char **argv)
{
   Getopt cmdline;

   cmdline.SetDescription("Show the meta data from a QDng file in pbuf format");
   cmdline.ReadArgs(argc, argv);
   cmdline.CheckOptions();

   ifstream file;

   try {
      string fname;

      cmdline.GetNonOption(0, fname);

      /* Open file */
      file.open(fname.c_str());
      if (!file.is_open())
         throw(EIOError("Can't open file", fname));

      /* Check for correct header */
      PbufHeader intro;

      file.read(reinterpret_cast<char*>(&intro), sizeof(PbufHeader));
      if (file.bad() || file.gcount() != sizeof(PbufHeader)){
         throw(EIOError("Can not read intro from stream"));
      }

      if (string(intro.magic) != PBUF_MAGIC)
         throw(EIOError("Malformated stream. No stream magic found."));

      if ((unsigned int) intro.version != PBUF_VERSION)
         throw(EIOError("Wrong stream version"));

      /* Print out header */
      int header_size;
      file.read((char*) &header_size, sizeof(header_size));

      char* buffer = new char[header_size];
      file.read((char*) buffer, header_size);

      FileSingleHeader MetaData;
      MetaData.ParseFromArray((void*) buffer, header_size);
//
//      google::protobuf::Descriptor* descriptor = MetaData.GetDescriptor();
//
//      google::protobuf::FieldDescriptor* =  descriptor->FindFieldByName("class");

      google::protobuf::io::OstreamOutputStream zcout(&cout);
      google::protobuf::TextFormat::Print(MetaData, &zcout);

   } catch (Exception& e) {
      cout << e.GetMessage() << endl;

      file.close();
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}

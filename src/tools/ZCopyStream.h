// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * ZCopyStream.h
 *
 *  Created on: Dec 28, 2013
 *      Author: markus
 */

#ifndef ZCOPYSTREAM_H_
#define ZCOPYSTREAM_H_

#include <google/protobuf/io/zero_copy_stream.h>

namespace QDLIB
{

   /**
    * Simple function which copies data to a ZeroCopyOutputStream.
    */
   void WriteToZeroCopyStream(::google::protobuf::io::ZeroCopyOutputStream& os, const char* buffer, size_t size);

   /**
    * Simple function which copies data from a ZeroCopyInputStream
    */
   void ReadFromZeroCopyStream(::google::protobuf::io::ZeroCopyInputStream& is, char* buffer, size_t size);

   /**
    * Map the buffer to a ZeroCopyInputStream.
    *
    * The data in the buffer is valid until is.Next() is called.
    * In case a buffer boundary is hit, the content is copied to location given by backup.
    *
    * \param is     The ZeroCopyInputStream to to read from
    * \param backup A fall back address which must provide size bytes storage on entry
    * \return
    */
   const char* MapToZeroCopyStream(::google::protobuf::io::ZeroCopyInputStream& is, char* backup, size_t size);

} /* namespace QDLIB */
#endif /* ZCOPYSTREAM_H_ */

/*
 * ZCopyStream.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: markus
 */

#include "ZCopyStream.h"
#include "tools/Exception.h"

#include <string.h>

namespace QDLIB
{
   void WriteToZeroCopyStream(::google::protobuf::io::ZeroCopyOutputStream& os, const char* buffer, size_t size)
   {
      char* buf;
      int buf_size;
      size_t len=0, size_written = 0;

      while (size_written < size){
         if(!os.Next(reinterpret_cast<void**>(&buf), &buf_size))
            throw(EIOError("WriteToZeroCopyStream failed!?"));

         len = buf_size;
         if ((size_t) buf_size > size-size_written) len = size-size_written;

         memcpy(reinterpret_cast<void*>(buf), reinterpret_cast<const void*>(buffer+size_written), len);

         size_written += len;
      }

      if (buf_size - len > 0) os.BackUp(buf_size - len);
   }

   void ReadFromZeroCopyStream(::google::protobuf::io::ZeroCopyInputStream& is, char* buffer, size_t size)
   {
      const char* buf;
      int buf_size = 0;
      size_t len=0, size_read = 0;

      while (size_read < size){
         if(!is.Next(reinterpret_cast<const void**>(&buf), &buf_size))
            throw(EIOError("ReadFromZeroCopyStream failed!?"));

         len = buf_size;
         if ((size_t) buf_size > size-size_read) len = size-size_read;

         memcpy(reinterpret_cast<void*>(buffer+size_read), reinterpret_cast<const void*>(buf), len);

         size_read += len;
      }

      if (buf_size - len > 0)
        is.BackUp(buf_size - len);
   }

   const char* MapToZeroCopyStream(::google::protobuf::io::ZeroCopyInputStream& is, char* backup, size_t size)
   {
      return backup;
   }

} /* namespace QDLIB */

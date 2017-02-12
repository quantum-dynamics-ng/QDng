/*
 * FileSingleTest.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: markus
 */

#include <cppunit/extensions/HelperMacros.h>

#include "FileSingleTest.h"
#include "tools/fstools.h"

#include "defs.h"
CPPUNIT_TEST_SUITE_REGISTRATION(FileSingleTest);


void FileSingleTest::Stream_Test()
{
   /* Write a file */
   DataClass data_out(true);
   FileSingle<DataClass> file_out;

   file_out.Format(FileSingle<DataClass>::stream);
   file_out.Name("DataClassTestFile");
   file_out.Suffix(".pbf");

   CPPUNIT_ASSERT_NO_THROW(file_out << &data_out);

   /* Read it back in */
   DataClass data_in;
   FileSingle<DataClass> file_in;

   file_in.Format(FileSingle<DataClass>::stream);
   file_in.Name("DataClassTestFile");
   file_in.Suffix(".pbf");

   CPPUNIT_ASSERT_NO_THROW(file_in >> &data_in);
   for (int i=0; i < data_in.size(); i++)
      CPPUNIT_ASSERT( data_in[i] == 2*i);


   /* Autodetection */
   FileSingle<DataClass> file_detect;
   file_detect.Name("DataClassTestFile");
   file_detect.Suffix(".pbf");

   //CATCH_EXCEPTION(file_detect.DetectFormat())
   CPPUNIT_ASSERT_NO_THROW(file_detect.DetectFormat());

   CPPUNIT_ASSERT(file_detect.Format() == FileSingle<DataClass>::stream );

   //CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFile.pbf"));
}


void FileSingleTest::Raw_Test()
{
   /* Write a file */
   DataClass data_out(true);
   FileSingle<DataClass> file_out;

   file_out.FormatOut(FileSingle<DataClass>::binary);
   file_out.Name("DataClassTestFile");
   file_out.Suffix(".raw");

   file_out << &data_out;
   CPPUNIT_ASSERT_NO_THROW(file_out << &data_out);

   /* Read it back in */
   DataClass data_in;
   FileSingle<DataClass> file_in;

   file_in.Format(FileSingle<DataClass>::binary);
   file_in.Name("DataClassTestFile");
   file_in.Suffix(".raw");

   CATCH_EXCEPTION(file_in >> &data_in);
   CPPUNIT_ASSERT_NO_THROW(file_in >> &data_in);
   for (int i=0; i < data_in.size(); i++)
      CPPUNIT_ASSERT( data_in[i] == 2*i);


   /* Autodetection */
   FileSingle<DataClass> file_detect;
   file_detect.Name("DataClassTestFile");
   file_detect.Suffix(".raw");

   //CATCH_EXCEPTION(file_detect.DetectFormat())
   CPPUNIT_ASSERT_NO_THROW(file_detect.DetectFormat());

   CPPUNIT_ASSERT(file_detect.Format() == FileSingle<DataClass>::binary );

   CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFile.meta"));
   CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFile.raw"));
}

void FileSingleTest::Compression_Test_ZLIB()
{
   DataClass data_out(true);
   FileSingle<DataClass> file_out;

   file_out.FormatOut(FileSingle<DataClass>::binary);
   file_out.Name("DataClassTestFileC");
   file_out.Suffix(".raw");
   file_out.Compression(FileSingle<DataClass>::ZLIB);

   CPPUNIT_ASSERT_NO_THROW(file_out << &data_out);

   /* Read it back in, detect compression automatically */
   DataClass data_in;
   FileSingle<DataClass> file_in;

   file_in.Format(FileSingle<DataClass>::binary);
   file_in.Name("DataClassTestFileC");
   file_in.Suffix(".raw");

   CATCH_EXCEPTION(file_in >> &data_in);

   for (int i=0; i < 20; i++){
     CPPUNIT_ASSERT(data_in[i] == 2*i);
   }

   CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFileC.meta"));
   CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFileC.raw"));
}


void FileSingleTest::Compression_Test_ZLIB_Stream()
{
   DataClass data_out(true);
   FileSingle<DataClass> file_out;

   file_out.Format(FileSingle<DataClass>::stream);
   file_out.Name("DataClassTestFileC");
   file_out.Suffix(".pbuf");
   file_out.Compression(FileSingle<DataClass>::ZLIB);

   CPPUNIT_ASSERT_NO_THROW(file_out << &data_out);

   /* Read it back in, detect compression automatically */
   DataClass data_in;
   FileSingle<DataClass> file_in;

   file_in.Format(FileSingle<DataClass>::stream);
   file_in.Name("DataClassTestFileC");
   file_in.Suffix(".pbuf");

   //CATCH_EXCEPTION(file_in >> &data_in);
   CPPUNIT_ASSERT_NO_THROW(file_in >> &data_in);

   for (int i=0; i < 20; i++){
      CPPUNIT_ASSERT(data_in[i] == 2*i);
   }


   CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFileC.pbuf"));
}

void FileSingleTest::Compression_Test_BZIP()
{
   DataClass data_out(true);
   FileSingle<DataClass> file_out;

   file_out.FormatOut(FileSingle<DataClass>::binary);
   file_out.Name("DataClassTestFileC");
   file_out.Suffix(".raw");
   file_out.Compression(FileSingle<DataClass>::BZIP);

   CPPUNIT_ASSERT_NO_THROW(file_out << &data_out);

   /* Read it back in, detect compression automatically */
   DataClass data_in;
   FileSingle<DataClass> file_in;

   file_in.Format(FileSingle<DataClass>::binary);
   file_in.Name("DataClassTestFileC");
   file_in.Suffix(".raw");

   CATCH_EXCEPTION(file_in >> &data_in);

   for (int i=0; i < 20; i++){
     CPPUNIT_ASSERT(data_in[i] == 2*i);
   }

   CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFileC.meta"));
   CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFileC.raw"));
}


void FileSingleTest::Compression_Test_BZIP_Stream()
{
   DataClass data_out(true);
   FileSingle<DataClass> file_out;

   file_out.Format(FileSingle<DataClass>::stream);
   file_out.Name("DataClassTestFileC");
   file_out.Suffix(".pbuf");
   file_out.Compression(FileSingle<DataClass>::BZIP);

   CATCH_EXCEPTION(file_out << &data_out);
   CPPUNIT_ASSERT_NO_THROW(file_out << &data_out);

   /* Read it back in, detect compression automatically */
   DataClass data_in;
   FileSingle<DataClass> file_in;

   file_in.Format(FileSingle<DataClass>::stream);
   file_in.Name("DataClassTestFileC");
   file_in.Suffix(".pbuf");

   //CATCH_EXCEPTION(file_in >> &data_in);
   CPPUNIT_ASSERT_NO_THROW(file_in >> &data_in);

   for (int i=0; i < 20; i++){
      CPPUNIT_ASSERT(data_in[i] == 2*i);
   }


   CPPUNIT_ASSERT_NO_THROW(FS::Remove("DataClassTestFileC.pbuf"));
}

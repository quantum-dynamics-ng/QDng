/*
 * FileSingleTest.h
 *
 *  Created on: Feb 13, 2013
 *      Author: markus
 */

#ifndef FILESINGLETEST_H_
#define FILESINGLETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "tools/FileSingle.h"

using namespace QDLIB;


class FileSingleTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( FileSingleTest );
   CPPUNIT_TEST( Stream_Test );
   CPPUNIT_TEST( Raw_Test );
   CPPUNIT_TEST( Compression_Test_ZLIB );
   CPPUNIT_TEST( Compression_Test_ZLIB_Stream );
   CPPUNIT_TEST( Compression_Test_BZIP );
   CPPUNIT_TEST( Compression_Test_BZIP_Stream );
   CPPUNIT_TEST_SUITE_END();

   protected:

      void Stream_Test();
      void Raw_Test();
      void Sequence_Test();
      void Compression_Test_ZLIB();
      void Compression_Test_ZLIB_Stream();
      void Compression_Test_BZIP();
      void Compression_Test_BZIP_Stream();



   private:

      /* Dummy class which implements minimal interface for FileSingle*/
      class DataClass {
         private:

            bool _init;
            int _size;
            int _data[20];
            ParamContainer _params;

         public:
            DataClass() : _init(false), _size(20)
            {
            }

            DataClass(bool init) : _init(init), _size(20)
            {
               _params.SetValue("size", _size);
               for (int i=0; i < _size; i++)
                  _data[i] = 2*i;
            }

            int* begin(int i) { CPPUNIT_ASSERT(_init); return _data; }
            string Name() { return "DataClass"; }
            size_t sizeBytes() { CPPUNIT_ASSERT(_init); return 20 * sizeof(int); }
            int size() { return 20; }
            int& operator[](int index) { return _data[index]; }
            int strides() { return 1; }
            ParamContainer Params() { return _params; }

            void Init (ParamContainer& pm)
            {
               _params = pm;

               int size;
               _params.GetValue("size", size);

               CPPUNIT_ASSERT(size == _size);

               _init = true;
            }
      };

};

#endif /* FILESINGLETEST_H_ */

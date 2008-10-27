#ifndef FILESINGLEC_H
#define FILESINGLEC_H

#include <fstream>

#include "Exception.h"

#include "FileSingle.h"
#include "ParamContainer.h"
#include "KeyValFile.h"

#include "Wavefunction.h"
#include "Operator.h"

using namespace std;

namespace QDLIB {
   
   /** 
    * I/O class for Binary data.
    * 
    * This is a template class which can handle all general
    * classes which support a basic interface, with at least
    * the folowing methods:
    * 
    * - T* C.begin()
    * - size_t C.sizeByte()
    * - ParamContainer& C.GetParams()
    * - C.Init(ParamContainer)
    * 
    * \todo implement ASCII+HDF5
    * \todo merge this back to FileSingle
    */
   template <class C>
   class FileSingleC: public FileSingle
   {
      private:
	 bool _drop_meta;      // Ignore metadata
	 bool _WriteFileBinary(C *data);
// 	 bool _WriteFileASCII(WaveFunction &wf);
// 	 bool _WriteFileHDF(WaveFunction &wf);
	 
	 bool _ReadFileBinary(C *data);
// 	 bool _ReadFileASCII(WaveFunction &wf);
// 	 bool _ReadFileHDF(WaveFunction &wf);	 
	 
      public:
         
         
         void SetDropMeta(bool drop);
         bool GetDropMeta();
         
	 void ReadFile(C *data);
	 void WriteFile(C *data);
	
	 FileSingle* operator<<(C *out);
   };
    
   template <class C>
   void operator>>(FileSingleC<C> *file, C *in)
   {
      FileSingleC->WriteFile(in);
   }
   
   /**
    * Write raw binary data.
    */
   template <class C>
   bool FileSingleC::_WriteFileBinary(C &data)
   {
      ofstream file;
      string s;
      
      /* Write meta file */
      if (!_drop_meta){
	 KeyValFile file(_name + METAFILE_SUFFIX);
         if ( !file.Write(data.GetParams()) ) EIOError("Can not write meta file");
      }
      
      p = wf.GetParams();
      
      
      /* Determine the suffix */
      if ( typeid(WaveFunction) == typeid(data) ) {
         s = _name + BINARY_WF_SUFFIX;
      } else if ( typeid(Operator) == typeid(data) ) {
         s = _name + BINARY_O_SUFFIX;
      }
      
      /* Write binary data */
      if( !file.open(_name.c_str(), ofstream::binary) ) throw( EIOError("Can not open binary file for writing") );
      file.write(wf.begin(), data.size() * sizeof( *(data.begin()) ) )
      if( file.bad() ) throw( EIOError("Can not write binary file") );
      return true;
   
   }
   
   /**
    * Read raw binary data.
    */
   template <class C>
   bool FileSingleC::_ReadFileBinary(C &data)
   {
     
      
      /* Read meta file */
      if (!_drop_meta){
         KeyValFile file(_name + METAFILE_SUFFIX);
         ParamContainer p;
         if ( !file.Parse(p) ) throw( EIOError("Can not read meta file") );
	 data.Init(p);
      }
      
      
      /* We need some parameters for reading */
      if (data.sizeByte <= 0) throw( EParamProblem("Wrong size") ) ;
      
      /* Determine the suffix */
      if ( typeid(WaveFunction) == typeid(data) ) {
         s = _name + BINARY_WF_SUFFIX;
      } else if ( typeid(Operator) == typeid(data) ) {
         s = _name + BINARY_O_SUFFIX;
      }      
      
      
      if( !file.open(s.c_str(), ifstream::binary) ) throw ( EIOError("Can not open binary file for reading") );
      file.read(data.begin(), data.sizeByte());
      if( file.bad() ) throw( EIOError("Can not read binary file") );
      
   
   }

   /**
    * File writer method.
    * 
    * \param data Data to save.
    * 
    */
   template <class C>
   bool FileSingleC::WriteFile(C &data)
   {
      switch(_type){
         binary:
	       return WriteFileBinary(data);
	       break;
	 ascii:
               throw(Exception("ASCII Unsupported"));
	 hdf5:
               throw(Exception("HDF5 Unsupported"));
      }
   }

   /**
    * File reader method.
    * 
    * \param data Data to save.
    * 
    */
   template <class C>
   bool FileSingleC::ReadFile(C &data)
   {
      switch(_type){
         binary:
	       return ReadFileBinary(data);
	       break;
	 ascii:
	       throw(Exception("ASCII Unsupported"));
	 hdf5:
               throw(Exception("HDF5 Unsupported"));
      }
   }   
   
   /**
    * Write data into file.
    */
   template <class C>
   void FileSingleC::operator<<(C *out)
   {
      WriteFile(out);
   }
   

   
   
} /* namespace QDLIB */


#endif /* #ifndef FILESINGLEC_H */


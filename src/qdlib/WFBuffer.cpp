#include "WFBuffer.h"
#include "tools/GlobalParams.h"

#include <unistd.h>
#include <sys/mman.h>




namespace QDLIB {

   WFBuffer::WFBuffer() : _size(0), _inmem(0), _maxmem(WFBUFFER_DEFAULT_MEM), _MaxBuf(0) ,_ondisk(0), _diskbuf(NULL)
   {
      ParamContainer& gp = GlobalParams::Instance();
            
      if (gp.isPresent("MaxBufferSize"))
         gp.GetValue("MaxBufferSize", (long int&) _MaxBuf);
   }


   WFBuffer::~WFBuffer()
   {
      Clear();
   }   
   
   void WFBuffer::Size(size_t size)
   {
      if (_size == 0) _size = size;
      else if (size < 0) Clear();
   }

   void WFBuffer::Init(WaveFunction* Psi)
   {
      if (_MaxBuf > 0){
         _maxmem = _MaxBuf / Psi->sizeBytes();
         
         if (_maxmem < 1) _maxmem = 0;
         if (_maxmem < _size)            
            _diskbuf = Open(_size - maxmem, true);
         
      }
   }
   
   
   void WFBuffer::SetBuf(size_t pos, WaveFunction *Psi)
   {
      if (_maxmem > 0 && _MaxBuf == 0)
         _maxmem = _buf.size()+1;
         
      
      if (_inmem < _maxmem){ /* All free, copy to to buffer location */
         _buf[pos].BufPos = -1;
         if (_buf[pos].Psi == NULL) {
            _buf[pos].Psi = Psi->NewInstance();
            _inmem++;
         }
         *(_buf[pos].Psi) = Psi;
      } else { /* Move an element to disk storage */
         for (size_t i = _buf.size()-1; i > 0; i--){
            if (i != pos && _buf[i].BufPos == -1){ /* This is an Element in memory */
                for (_diskmap) /* Look for free disk buffer element */
            }
         }
      }
 
   }
   
   void WFBuffer::Clear()
   {
      _File.Close();
      
      while (! _buf.empty() ){
         DELETE_WF(_buf.back().Psi);
         _buf.pop_back();
      }
      
      while (! _LastAccess.empty())
         _LastAccess.pop();
      
      _diskmap.clear();
      
      _size = 0;
      _inmem = 0;
      _maxmem = 0;
      _ondisk = 0;
      _diskbuf = NULL;
   }


}

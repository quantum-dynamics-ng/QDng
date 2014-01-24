#include "OProjection.h"
#include "WFMultistate.h"
#include "FileWF.h"
#include "tools/Logger.h"

namespace QDLIB {
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OProjection)
	 
   OProjection::OProjection() : _name("OProjection"), _sign(1), _inv(false), _buf(NULL)
   {
   }
   
   
   OProjection::~OProjection()
   {
      _destroy();
   }
 

   void OProjection::_destroy()
   {
      DELETE_WF(_buf);
      _buf = NULL;
   }
   
   void QDLIB::OProjection::Init( WaveFunction * Psi )
   {
      if (Psi == NULL)
         throw(EIncompatible("Projector got a void wave function"));

      if (Size() > 0)
         return;

      _buf = Psi->NewInstance();
      _buf->Retire();

      /* Read a WF sequence from disk (Has to be here because we need to know the WF type) */
      if (_params.isPresent("files")) {
         string files;
         FileWF wfs;
         uint num = 1;
         int start = 0;
         int step = 1;

         _params.GetValue("files", files);

         if (_params.isPresent("num"))
            _params.GetValue("num", num);

         _params.GetValue("start", start);

         if (_params.isPresent("step")) {
            _params.GetValue("step", step);
         }

         /*         Logger& log = Logger::InstanceRef();
          log.cout() << "Projector:\n";
          log.cout() << "Reading from files: " << files << endl;
          log.cout() << "from " << start;
          if (num > 1)
          log.cout() << " to " << num-start-1;
          log.cout() << endl << endl;*/

         ReadFromFiles(files, start, step, num);

         if (uint(num) != Size()){
            Logger& log = Logger::InstanceRef();
            log.cout() << "wanted " << num << ", got " << Size() << endl;
            throw (EIOError("Projector: The Projection couldn't be read completely. Something is missing."));
         }

         WFBuffer::Init(Psi);
      }
   }
   
   void QDLIB::OProjection::Init( ParamContainer & params )
   {
      _params = params;
      
      bool pos;
      _params.GetValue("positive", pos,true);
      Sign(pos);

      _params.GetValue("inv", _inv, false);
   }

     
   Operator * QDLIB::OProjection::operator =( Operator * O )
   {
      Copy(O);
      return this;
   }
   
   Operator * OProjection::Copy(Operator * O)
   {
      OProjection* o;
      
      o = dynamic_cast<OProjection*>(O);
      if( o == NULL)
	 throw(EIncompatible("Projection operator tried to copy wrong operator", O->Name()));
      
      if ( Size()>0){
         _destroy();
         Clear();
      }
      
      _inv = o->_inv;
      _sign = o->_sign; 
      

      for (uint i=0; i < Size(); i++){
         Add((*o)[i]);
      }


      return this;
   }
      
   void OProjection::Apply( WaveFunction * Psi )
   {
      _buf->Reaquire();

      *_buf = Psi;
      Apply( Psi, _buf );

      _buf->Retire();
   }
   
   void OProjection::Apply( WaveFunction * destPsi, WaveFunction * sourcePsi )
   {
      *destPsi = dcomplex(0, 0);


      for (uint i = 0; i < Size(); i++) {
         WaveFunction* wfi = Get(i);
         MultElementsAdd(destPsi, wfi, *wfi * sourcePsi * _sign);
      }
      if (_inv) {
         _buf->Reaquire();
         *_buf = sourcePsi;
         AddElements(_buf, destPsi, -1);
         *destPsi = _buf;
         _buf->Retire();
      }
   }


   bool OProjection::Valid(WaveFunction * Psi)
   {
      if ( Psi == NULL ) return false;
      
      if (_buf->size() != Psi->size() ) return false;
      
      return true;
   }
   
}



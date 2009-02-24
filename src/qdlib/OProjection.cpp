#include "OProjection.h"

namespace QDLIB {

   OProjection::OProjection() : _size(0), _name("OProjection"), _buf(NULL)
   {
      
   }
   
   
   OProjection::~OProjection()
   {
      _destroy();
   }
   
   void OProjection::_destroy()
   {
      for (int i=0; i < _size; i++){
	 delete _wfbuf[i];
      }
      
      if (_buf != NULL) delete _buf;
      
      _buf = NULL;
      _size = 0;
   }
   
   void QDLIB::OProjection::Add( WaveFunction * Psi )
   {
      if (_size == MAX_WFSPACE)
	 throw( EOverflow("Projector has reached max capaticty: MAX_WFSPACE"));
	 
      if (Psi != NULL){
	 if (_size == 0)
	    _buf = Psi->NewInstance();
		  
	 _wfbuf[_size] = Psi;
	 _size++;
      }
   }
   
   string & QDLIB::OProjection::Name( )
   {
      return _name;
   }
      
   Operator * QDLIB::OProjection::NewInstance( )
   {
      return new OProjection();
   }
   
   
   void QDLIB::OProjection::Init( WaveFunction * Psi )
   {
      if (Psi == NULL)
	 throw ( EIncompatible("Projector got a void wave function") );
   }
   
   void QDLIB::OProjection::Init( ParamContainer & params )
   {
      _params = params;
   }

  
   Operator * QDLIB::OProjection::Scale( const double d )
   {
      for (int i=0; i < _size; i++){
	 *(_wfbuf[i]) *= d;
      }
   }
      
   Operator * QDLIB::OProjection::operator *( Operator * O )
   {
      throw(EIncompatible("Projection operator not appliable to other operators"));
      
      return this;
   }
   
   Operator * QDLIB::OProjection::operator =( Operator * O )
   {
      OProjection* o;
      
      o = dynamic_cast<OProjection*>(O);
      if( o == NULL)
	 throw(EIncompatible("Projection operator tried to copy wrong operator", O->Name()));
      
      if (_size>0) _destroy();
      
      _size = o->_size;
      
      for (int i=0; i < _size; i++){
	 *(_wfbuf[i]) = o->_wfbuf[i];
      }
   }
   
   WaveFunction * QDLIB::OProjection::Apply( WaveFunction * Psi )
   {
      WaveFunction* in = Psi->NewInstance();
      *in = Psi;
      Apply( Psi, in );
      delete in;
      return Psi;
   }
   
   /**
    * \todo replace with a muladd low level func.
    */
   WaveFunction * QDLIB::OProjection::Apply( WaveFunction * destPsi, WaveFunction * sourcePsi )
   {
      *destPsi = dcomplex(0,0);
      
      for (int i=0; i < _size; i++){
	 *_buf = _wfbuf[i];
	 *_buf *= *(_wfbuf[i]) * sourcePsi;
	 *destPsi += _buf;
      }
      
      return destPsi;
   }


}
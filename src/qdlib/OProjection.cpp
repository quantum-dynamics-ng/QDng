#include "OProjection.h"
#include "WFMultistate.h"
#include "FileWF.h"

namespace QDLIB {

   OProjection::OProjection() : _name("OProjection"), _sign(1), _size(0),  _buf(NULL)
   {
      for (int i=0; i < _size; i++){
         _wfbuf[MAX_WFSPACE] = NULL;
      }
   }
   
   
   OProjection::~OProjection()
   {
      _destroy();
   }
 

   void OProjection::_destroy()
   {
      for (int i=0; i < _size; i++){
         if (_wfbuf[i] != NULL) delete _wfbuf[i];
      }
      
      if (_buf != NULL) delete _buf;
      
      _buf = NULL;
      _size = 0;
   }
   
   /**
    * Add a new wave function to the container.
    * 
    * The Operator takes responsibility for the
    * WF object (get destructed along with the projector).
    * 
    */
   void QDLIB::OProjection::Add( WaveFunction * Psi )
   {
      if (_size == MAX_WFSPACE)
	 throw( EOverflow("Projector has reached max capaticity: MAX_WFSPACE"));
	 
      if (Psi != NULL){
         if (_buf == NULL)
	    _buf = Psi->NewInstance();
		  
	 _wfbuf[_size] = Psi->NewInstance();
	 *(_wfbuf[_size]) = Psi;
	 _size++;
      }
      
   }
   
   /**
    * Extract a wave function from the operator.
    */
   WaveFunction* QDLIB::OProjection::Get(int n)
   {
      if (n < _size)
	 return _wfbuf[n];
      else
	 throw ( EParamProblem("Larger index requested than size of the projector"));
      
      return NULL;
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
      
      /** \todo dirty hack to handle Multistate WFs */
      if (_size == 0){
         WFMultistate* wfm;
         wfm = dynamic_cast<WFMultistate*>(Psi);
         if (_buf != NULL) delete _buf;
         if (wfm == NULL)
            _buf = Psi->NewInstance();
         else
            _buf = wfm->State(0)->NewInstance();
      }
            
      /* Read a WF sequence from disk (Has to be here because we need to know the WF type) */
      if (_params.isPresent("files")){
         string files;
         FileWF wfs;
         int num;
         int start;
         int step=1;
         
         _params.GetValue("files", files);
         _params.GetValue("num", num);
         _params.GetValue("start", start);
         if (_params.isPresent("step")){
            _params.GetValue("step", step);
         }
         if (num < 1)
            throw(EParamProblem("No file count specified for Projector"));
         
         wfs.Suffix(BINARY_WF_SUFFIX);
         wfs.Name(files);
         wfs.ActivateSequence(step);
         wfs.Counter(start);
         for (int i=0; i < num; i+=step){ /* Read the sequence */
            if (_size == MAX_WFSPACE)
               throw( EOverflow("Projector has reached max capaticity: MAX_WFSPACE"));
            wfs >> _buf;
            _wfbuf[_size] = _buf->NewInstance();
            *(_wfbuf[_size]) = _buf;
            _size++;
         }
      }
   }
   
   void QDLIB::OProjection::Init( ParamContainer & params )
   {
      _params = params;
      
      bool pos;
      _params.GetValue("positive", pos,true);
      Sign(pos);

   }

  
   Operator * QDLIB::OProjection::Scale( const double d )
   {
      for (int i=0; i < _size; i++){
	 *(_wfbuf[i]) *= d;
      }
      return this;
   }
      
   Operator * QDLIB::OProjection::operator *( Operator * O )
   {
      throw(EIncompatible("Projection operator not appliable to other operators"));
      
      return this;
   }
   
   Operator * QDLIB::OProjection::operator =( Operator * O )
   {
      Copy(O);
      return this;
   }
   
   Operator * QDLIB::OProjection::Copy(Operator * O)
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
      return this;
   }
   
   dcomplex OProjection::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      dcomplex c;
      WaveFunction *OpPsi;
      
      OpPsi = PsiKet->NewInstance();
      
      Apply(OpPsi, PsiKet);
      c = *PsiBra * OpPsi;
      delete OpPsi;
      return c;
   }
   
   double OProjection::Expec(WaveFunction *Psi)
   {
      return MatrixElement( Psi, Psi).real();
   }
	    
   
   WaveFunction * QDLIB::OProjection::Apply( WaveFunction * Psi )
   {
      *_buf = Psi;
      Apply( Psi, _buf );

      return Psi;
   }
   
   WaveFunction * QDLIB::OProjection::Apply( WaveFunction * destPsi, WaveFunction * sourcePsi )
   {
      *destPsi = dcomplex(0,0);
      
      int i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
      for (i=0; i < _size; i++){
	 MultElementsAdd(destPsi, _wfbuf[i], *(_wfbuf[i]) * sourcePsi * _sign);
      }
      
      return destPsi;
   }


   bool OProjection::Valid(WaveFunction * Psi)
   {
      if ( Psi == NULL ) return false;
      
      if (_buf->size() != Psi->size() ) return false;
      
      return true;
   }
   
}



#include "OProjection.h"
#include "WFMultistate.h"
#include "FileWF.h"
#include "tools/Logger.h"

namespace QDLIB {
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OProjection)
	 
   OProjection::OProjection() : _name("OProjection"), _sign(1), _inv(false), _size(0),  _buf(NULL)
   {
      for (int i=0; i < MAX_WFSPACE; i++){
         _wfbuf[i] = NULL;
      }
   }
   
   
   OProjection::~OProjection()
   {
      _destroy();
   }
 

   void OProjection::_destroy()
   {
      for (int i=0; i < _size; i++){
	 DELETE_WF(_wfbuf[i]);
      }
      
      DELETE_WF(_buf);
      
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
   
   void QDLIB::OProjection::Init( WaveFunction * Psi )
   {
      if (Psi == NULL)
	 throw ( EIncompatible("Projector got a void wave function") );
      
      /** \todo dirty hack to handle Multistate WFs */
      if (_size == 0){
         WFMultistate* wfm;
         wfm = dynamic_cast<WFMultistate*>(Psi);
	 DELETE_WF(_buf);
         if (wfm == NULL)
            _buf = Psi->NewInstance();
         else
            _buf = wfm->State(0)->NewInstance();
      }
            
      /* Read a WF sequence from disk (Has to be here because we need to know the WF type) */
      if (_params.isPresent("files")){
// 	 Logger& log = Logger::InstanceRef();
         string files;
         FileWF wfs;
         int num=1;
         int start=0;
         int step=1;
         
         _params.GetValue("files", files);
	 if(_params.isPresent("num")) _params.GetValue("num", num);
	 _params.GetValue("start", start);
         if (_params.isPresent("step")){
            _params.GetValue("step", step);
         }
         
// 	 log.cout() << "Projector:\n";
// 	 log.cout() << "Reading from files: " << files << endl;
// 	 log.cout() << "from " << start;
// 	 if (num > 1)
// 	    log.cout() << " to " << num-start-1;
// 	 log.cout() << endl << endl;
	 
         wfs.Suffix(BINARY_WF_SUFFIX);
         wfs.Name(files);
         wfs.ActivateSequence(step);
         wfs.Counter(start);
         for (int i=0; i < num; i+=step){ /* Read the sequence */
            if (_size == MAX_WFSPACE)
               throw( EOverflow("Projector has reached max capaticity: MAX_WFSPACE"));
// 	    log.coutdbg() << "Read " << i << endl;
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

      _params.GetValue("inv", _inv, false);
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
      _inv = o->_inv;
      _sign = o->_sign; 
      
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
      DELETE_WF(OpPsi);
      return c;
   }
   
   double OProjection::Expec(WaveFunction *Psi)
   {
      return MatrixElement( Psi, Psi).real();
   }
	    
   
   void QDLIB::OProjection::Apply( WaveFunction * Psi )
   {
      *_buf = Psi;
      Apply( Psi, _buf );
   }
   
   void QDLIB::OProjection::Apply( WaveFunction * destPsi, WaveFunction * sourcePsi )
   {
      *destPsi = dcomplex(0,0);
      
      int i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
      for (i=0; i < _size; i++){
	 MultElementsAdd(destPsi, _wfbuf[i], *(_wfbuf[i]) * sourcePsi * _sign);
      }
      if (_inv){
         *_buf = 1;
         AddElements(_buf, destPsi, -1);
         *destPsi = _buf;
      }
   }


   bool OProjection::Valid(WaveFunction * Psi)
   {
      if ( Psi == NULL ) return false;
      
      if (_buf->size() != Psi->size() ) return false;
      
      return true;
   }
   
}



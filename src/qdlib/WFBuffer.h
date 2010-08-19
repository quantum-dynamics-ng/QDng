#ifndef QDLIBWFBUFFER_H
#define QDLIBWFBUFFER_H

namespace QDLIB {

   /**
    *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class WFBuffer {
      private:
      public:
	 WFBuffer();
	 ~WFBuffer();
	 
	 void Size(sizt_t size);
	 size_t Size() const;
	 
	 WaveFunction* GetBuf(size_t pos);
	 void SetBuf(size_t pos, WaveFunction *Psi);
	 
	 void Clear();
   };

}

#endif

#ifndef QDLIBBUTTERWORTH_H
#define QDLIBBUTTERWORTH_H

namespace QDLIB
{

   /**
    * Butterworth filter generator.
    * 
    * The template works with every vector type.
    * 
    * For mathematical details \see Wikipedia
    * 
    * @author Markus Kowalewski
    */
   template <class VECT>
   class Butterworth
   {
      public:
	 static void Highpass(VECT &vec, double center, int order);
	 static void Lowpass(VECT &vec, double center, int order);
   };

   
   
      /**
    * Generates a butterworth highpass transfer function.
    * 
    * \param vec     The vector which should be filled
    * \param center  Center gridpoint 
    * \param order   filter order
       */
   template <class VECT>
	 void Butterworth<VECT>::Highpass(VECT & vec, double center, int order)
   {
      double k = double(order);
      int size = vec.size();
      
      for (int i=0; i < size; i++){
	 vec[i] = 1 / (1 + pow( center / double(i) , 2 * k) );
      }
      
   }

   /**
    * Generates a butterworth highpass transfer function.
    * 
    * \param vec     The vector which should be filled
    * \param center  Center gridpoint 
    * \param order   filter order
    */   
   template <class VECT>
	 void Butterworth<VECT>::Lowpass(VECT & vec, double center, int order)
   {
      double k = double(order);
      int size = vec.size();
      
      for (int i=0; i < size; i++){
	 vec[i] = 1 / (1 + pow(  double(i) / center, 2 * k) );
      }
      
   }
   
}


#endif

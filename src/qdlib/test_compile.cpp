#include <iostream>


#include "WFLevel.h"
#include "OHerMat.h"


using namespace  std;
using namespace  QDLIB;



int main(int argc, char **argv)
{
   WFLevel WF, WF2;
   ParamContainer p;
   int s;
   
   dcomplex c(0.7,0.7);

   
   cout << c << endl;
   cout << c.conj() << endl;
   cout << WF.Name() <<endl;
   
   WF.Size(3);
   
   WF[0] = dcomplex(0.707107,0.707107);
   WF[1] = dcomplex(0,-1);
   WF[2] = dcomplex(0,1);
   
   p =WF.Params();
   
   p.GetValue("size", s);
   
   for (int i=0; i < s; i++){
      cout << "(" << WF[i] << ")" << endl;
   }
   
   cout << "Norm: " << WF.Norm()  << endl;
   
   WF.Normalize();
   
   cout << "Norm: " << WF.Norm()  << endl;
   
//    for (int i=0; i < s; i++){
//       cout << "(" << WF[i] << ")" << endl;
//    }
//    WF2 = WF;
//    
//    for (int i=0; i < s; i++){
//       cout << "(" << WF2[i] << ")" << endl;
//    }

   try {
      WaveFunction *psi1, *psi2;
      Operator *o,*o2;
      
      OHerMat Matrix, Matrix2;
      
      Matrix.Init(p);
      
      p = Matrix.Params();
   
      p.GetValue("size", s);
      
      
      cout << "Matrix initialized, size : " << s << endl;
      
      psi1 = &WF;
      o = &Matrix;
      
      Matrix[0][0] = Matrix[1][1] = Matrix[2][2] =1;
      Matrix[1][0] = Matrix[0][1] = 1;
      Matrix[2][0] = Matrix[0][2] = 0.5;
      
      psi2 = (*o) * psi1;
      
       o2 = (*o) * o;
     // o2 = o;
   
      for (int i=0; i < s; i++){
         cout << "(" << (*psi2)[i] << ")" << endl;
      }
      
      for (int i=0; i < s; i++){
	 for (int j=0; j < s; j++){
            cout << "(" << (*(dynamic_cast<OHerMat*>(o2)))[i][j] << ")  ";
	 }
	 cout << endl;
      }
      
      
   } catch (Exception e){
      cout << e.GetMessage() << endl;
   }
}



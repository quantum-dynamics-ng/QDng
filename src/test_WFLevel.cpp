#include <iostream>


#include "qdlib/WFLevel.h"
#include "qdlib/OHerMat.h"

#include "sys/FileSingleDefs.h"

using namespace  std;
using namespace  QDLIB;

int main(int argc, char **argv)
{
   WFLevel start_wf(3);
   
   OHerMat mat_op (3);

   
   /* Interface  test */
   cout << "WF: " << start_wf.Name() << endl;
   cout << "Operator: " << mat_op.Name() << endl;
   
   start_wf[0] = dcomplex(1,1);
   start_wf[1] = dcomplex(2,2);
   start_wf[2] = dcomplex(3,3);
   
   cout << "\nVector init: \n" << start_wf << endl;
   
   mat_op[0][0] = 0.1;
   mat_op[1][1] = 0.15;
   mat_op[2][2] = 0.4;
   
   mat_op[1][0] = mat_op[0][1] = 0.01;
   mat_op[1][2] = mat_op[2][1] = 0.015;
   mat_op[2][0] = mat_op[0][2] = 0.04;
   
   cout << "\nMatrix init: \n" << mat_op << endl;
   
   /* Test pure WF methods */
   cout << "\nNorm: " << start_wf.Norm() << endl;
   start_wf.Normalize();
   cout << "\nNorm + vector after normalize: " << start_wf.Norm() << endl << start_wf << endl;
   
   WFLevel wf_copy;
   
   wf_copy = start_wf;
   cout << "\nNative copy operator: \n" << wf_copy << endl;
   
   WaveFunction *wf;
   wf = start_wf.NewInstance();
   cout << "Size of NewInstance: " << wf->size() << endl;
   
   Operator *O;
   
   O = mat_op * (Operator*)(&mat_op);
   /* Test Pure Matrix functions */
   cout << "\nSquare of matrix: " <<  mat_op * mat_op  << endl;
   
   delete O;
   
   delete wf;
   wf = mat_op * (WaveFunction*)(&start_wf);
   
   cout << "Matrix * Vector (pointer): " << *wf << endl;
   cout << "Matrix * Vector (native types): " << mat_op * start_wf << endl;
   
   delete wf;
   
   /* File Writer test */
   cout << "Write Vector" << endl;
   FileWF file(binary, "Vector", BINARY_WF_SUFFIX);
   file.ActivateSequence();
   
   file << &start_wf;
   file << &start_wf;
   cout << "Writing Completed" << endl;
   
   WFLevel read_wf;
      
   file.ResetCounter();
   try{
//       file >>  &read_wf;
//       cout << "\nRead from file: \n" << read_wf << endl;
//       file >>  &read_wf;
//       cout << "\nRead from file: \n" << read_wf << endl;
      
   } catch (Exception e) {
      cout <<  e.GetMessage() << endl;
   }

   
   /* Operator Writer test */
   cout << "Write Matrix" << endl;
   
   FileOHerMat fileO(binary, "Matrix", BINARY_O_SUFFIX);;
   
   fileO << &mat_op;
	 
   cout << "Writing Completed" << endl;
   
   OHerMat mat_in;
   
   fileO >> &mat_in;
   cout << "\nRead from file: \n" << mat_in << endl;
   
   /* Test the sum operator */
   
}

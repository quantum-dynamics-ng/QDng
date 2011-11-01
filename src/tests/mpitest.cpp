/*
 * mpitest.cpp
 *
 * Simple MPI check routine. CPPUnit can't handle it (?)
 *
 *  Created on: 01.11.2011
 *      Author: markus
 */

#include <mpi.h>
#include "config.h"
#include "tools/Logger.h"
#include "qdlib/WFLevel.h"
#include "qdlib/WFMultistate.h"
#include "qdlib/FileWF.h"

using namespace std;
using namespace QDLIB;

#define SIZE 6
#define STRIDES 3

int main(int argc, char** argv)
{
#ifdef HAVE_MPI
   Logger& log = Logger::InstanceRef();
//   int rank = MPI::COMM_WORLD.Get_rank();
//   int size = MPI::COMM_WORLD.Get_size();

   MPI::Init();



   log.Header("MPI Test", Logger::Chapter);
   log.cout() << "MPI test starting up - You should read this only once." << endl;
   log.flush();

   /* Simple sync test */
   dVec vec1;
   vec1.newsize(SIZE, STRIDES, &MPI::COMM_WORLD);

   vec1 = 0;

   for (int s=vec1.MPIrank(); s < vec1.strides(); s += vec1.MPIsize())
      for (int j=0; j < vec1.lsize(); j++){
         vec1.begin(s)[j] = double(s*SIZE/STRIDES+j);
      }

   log.cout() << "without sync - local portion\n";
   log.cout() << vec1 << endl;

   log.cout() << "with sync - global portion\n";
   vec1.SyncStrides();
   log.cout() << vec1 << endl;

   log.flush();

   /*
    * Test basic multistate.
    */
   CollectorWF *Cwf = CollectorWF::Instance();
   WFLevel *wf[STRIDES];
   ParamContainer pm;
   WFMultistate *wfm = new WFMultistate();
   Cwf->Register(wfm);

   /* init */
   pm.SetValue("size", SIZE/STRIDES);
   for (int i=0; i < STRIDES; i++){
      wf[i] = new WFLevel();
      Cwf->Register(wf[i]);
      wf[i]->Init(pm);
      wfm->Add(wf[i], i);
   }

   pm.clear();
   wfm->Init(pm);

   /** Init the local stride with numbers */
   for (int s=wfm->MPIrank(); s < STRIDES; s += wfm->MPIsize())
      for (int i=0; i < SIZE/STRIDES; i++)
         ((cVec*) wf[s])[i] = dcomplex(wfm->MPIrank()+1);

   wfm->SyncStrides();

   log.cout() << "MS - WF: " << *wfm << endl;
   log.flush();

   /*
    * File writer test.
    */
   FileWF file;
   WaveFunction *lwf;

   file.Name("TESTWF");

   try {
   file << wfm;

   lwf = file.LoadWaveFunctionByMeta();

   lwf->SyncStrides();

   log.cout() << "MS - WF after reading: " << *lwf << endl;

   } catch (Exception e) {
      cout << e.GetMessage() << endl;
   }

   /*
    * Clean up
    */
   log.flush();
   DELETE_ALL_WF();
   MPI::Finalize();
#else
   cout << "\nApplication is not compiled with MPI support\n\n";
#endif
}

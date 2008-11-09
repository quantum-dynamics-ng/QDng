#include "OGridNablaSq.h"
#include "WFGridSystem.h"


namespace QDLIB {

   OGridNablaSq::OGridNablaSq()
   : OKSpace(), _name("OGridNablaSq"), _ndims(0), _dims(NULL)
   {
   }
   
   
   OGridNablaSq::~OGridNablaSq()
   {
   }


}

/**
 * Init K-Space in one dimension.
 * All factors are included in the kspace representation except FFT normalization.
 * 
 * \param mass reduced mass
 */
dVec * QDLIB::OGridNablaSq::InitKspace1D(const double mass, const double length, const int Nx)
{

   dVec *kspace;
   
   kspace = new dVec(Nx);
   
   double dp = (2*PI) / length;    // Setup dp for kspace
   dp *= dp;
   
   /* We include all the factors in the k-space function => Do it only once */
   /* The minus cancels with minus from -kx^2. */
   dp *= 0.5 /  mass ;
     
   int i;
	 
   for (i=0; i < Nx / 2; i++){ //run from [-p..+p]
      (*kspace)[i] = (double(i) * double(i)) * dp;
      (*kspace)[Nx - i - 1] = (double(i+1)* double(i+1)) * dp;
   }
   
   return kspace;
}


Operator * QDLIB::OGridNablaSq::NewInstance()
{
   OGridNablaSq *r;
   
   r = new OGridNablaSq();
   
   return r;
}

void QDLIB::OGridNablaSq::Init(ParamContainer & params)
{
}

const string & QDLIB::OGridNablaSq::Name()
{
   return _name;
}

void QDLIB::OGridNablaSq::UpdateTime()
{
}

dcomplex QDLIB::OGridNablaSq::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
{
}

double QDLIB::OGridNablaSq::Expec(WaveFunction * Psi)
{
}

WaveFunction * QDLIB::OGridNablaSq::operator *(WaveFunction * Psi)
{
}

WaveFunction * QDLIB::OGridNablaSq::operator *=(WaveFunction * Psi)
{
}

Operator * QDLIB::OGridNablaSq::operator =(Operator * O)
{
}

Operator * QDLIB::OGridNablaSq::operator *(Operator * O)
{
}

void QDLIB::OGridNablaSq::InitKspace()
{
   dVec *kspace1;
   
   
   dVecView view(*_kspace, _ndims, dims);
   
   for (int i=0; i < _ndims; i++){
      if (_mass[i] != 0) {
	 
         kspace1 = InitKspace1D(mass[i], );
      }
      
   }
}



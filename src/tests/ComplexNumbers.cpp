#include "ComplexNumbers.h"
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ComplexNumbers);

using namespace QDLIB;

ComplexNumbers::ComplexNumbers()
 : CppUnit::TestFixture()
{
}


ComplexNumbers::~ComplexNumbers()
{
}

void ComplexNumbers::API_Test()
{
  /* Constructors */
   dcomplex c1(1.5);
   dcomplex c2(2.5,3.25);
   
   CPPUNIT_ASSERT(c1._real == 1.5);
   CPPUNIT_ASSERT(c1._imag == 0);
   
   CPPUNIT_ASSERT(c2._real == 2.5);
   CPPUNIT_ASSERT(c2._imag == 3.25);
   
   /* Accessor */
   CPPUNIT_ASSERT(c2._real == c2.real());
   CPPUNIT_ASSERT(c2._imag == c2.imag());
   
   c1.real(1.2);
   c1.imag(3.4);
   CPPUNIT_ASSERT(c1.real() == 1.2);
   CPPUNIT_ASSERT(c1.imag() == 3.4);
   
   /* Copy and assignment */
   c1 = dcomplex(5.6,6.7);
   CPPUNIT_ASSERT(c1.real() == 5.6);
   CPPUNIT_ASSERT(c1.imag() == 6.7);
   
   c1 = 8.9;
   CPPUNIT_ASSERT(c1.real() == 8.9);
   CPPUNIT_ASSERT(c1.imag() == 0);
   
   
   
   /* Complex Conjugation */
   c1 = c2.conj();
   CPPUNIT_ASSERT(c1._real == c2._real);
   CPPUNIT_ASSERT(c1._imag == -c2._imag);
   
   c1 = c2;
   c1.conjugate();
   CPPUNIT_ASSERT(c1._real == c2._real);
   CPPUNIT_ASSERT(c1._imag == -c2._imag);
   
   CPPUNIT_ASSERT(conj(c2).real() == c2._real);
   CPPUNIT_ASSERT(conj(c2).imag() == -c2._imag);

   
   c1 = I;
   CPPUNIT_ASSERT(c1._real == 0);
   CPPUNIT_ASSERT(c1._imag == 1);
   
   
}

void ComplexNumbers::NUMERIC_Test()
{
   
   dcomplex c1;
   dcomplex c2;
   
   std::cout.precision(20);
   
   /* Imaginary unit */
   CPPUNIT_ASSERT(I * I == -1);
   
   c1 = I * 2.1;
   CPPUNIT_ASSERT( c1.real() == 0 );
   CPPUNIT_ASSERT( c1.imag() == 2.1);
   
   c1 = 3.4 * I;
   CPPUNIT_ASSERT( c1.real() == 0 );
   CPPUNIT_ASSERT( c1.imag() == 3.4);

   c2.real(5.6);
   c2.imag(7.8);
   
   c1 = I * c2;
   CPPUNIT_ASSERT( c1.real() == -7.8);
   CPPUNIT_ASSERT( c1.imag() == 5.6);
   
   c1 = c2 * I;
   CPPUNIT_ASSERT( c1.real() == -7.8);
   CPPUNIT_ASSERT( c1.imag() == 5.6);

   /* Complex addition */
   c1=dcomplex(9.1,10.11) + 1;
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 10.1, TIGHT_EPS );
   CPPUNIT_ASSERT( c1.imag() == 10.11);
   
   c1=dcomplex(9.1,10.11);
   c1+=1;
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 10.1, TIGHT_EPS );
   CPPUNIT_ASSERT( c1.imag() == 10.11);
   
   c1=dcomplex(9.1,10.11) + dcomplex(11.12,13.14);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 20.22, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), 23.25, TIGHT_EPS );

   c1=dcomplex(9.1,10.11);
   c1+= dcomplex(11.12,13.14);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 20.22, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), 23.25, TIGHT_EPS );

   /* Complex Subtraction*/
   c1=dcomplex(9.1,10.11) - 1;
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 8.1, TIGHT_EPS );
   CPPUNIT_ASSERT( c1.imag() == 10.11);
   
   c1=dcomplex(9.1,10.11);
   c1-=1;
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 8.1, TIGHT_EPS );
   CPPUNIT_ASSERT( c1.imag() == 10.11);
   
   c1=dcomplex(9.1,10.11) - dcomplex(11.12,13.14);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), -2.02, LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), -3.03, LOOSE_EPS );

   c1=dcomplex(9.1,10.11);
   c1-= dcomplex(11.12,13.14);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), -2.02, LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), -3.03, LOOSE_EPS );
   
   
   /* Complex multiplication */
   c1=dcomplex(1,2) * 5;
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 5, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), 10, TIGHT_EPS );

   c1=dcomplex(1,2);
   c1*=5;
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 5, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), 10, TIGHT_EPS );

   
   c1=dcomplex(1,2)*dcomplex(3,4);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), -5, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), +10, TIGHT_EPS );

   
   c1=dcomplex(1,2);
   c1*= dcomplex(3,4);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), -5, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), +10, TIGHT_EPS );
   
   
   /* Complex Division */
   c1=dcomplex(1,2) / 5;
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 0.2, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), 0.4, TIGHT_EPS );

   c1=dcomplex(1,2);
   c1/=5;
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 0.2, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), 0.4, TIGHT_EPS );

   
   c1=dcomplex(1,2)/dcomplex(3,4);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 0.44, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), 0.08, TIGHT_EPS );

   
   c1=dcomplex(1,2);
   c1/= dcomplex(3,4);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.real(), 0.44, TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( c1.imag(), 0.08, TIGHT_EPS );
   
   /* complex abs */
   c1=dcomplex(1,2);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cabs(c1), 2.23606797749979, TIGHT_EPS );
   
   /* Exponentials */
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.13120438375681, cexp(c1).real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(2.47172667200482, cexp(c1).imag(), LOOSE_EPS );
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, cexpI(0).real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, cexpI(0).imag(), LOOSE_EPS );
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1/sqrt(2), cexpI(M_PI/4).real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1/sqrt(2), cexpI(M_PI/4).imag(), LOOSE_EPS );
      
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, cexpI(M_PI/2).real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, cexpI(M_PI/2).imag(), LOOSE_EPS );

   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1, cexpI(M_PI).real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, cexpI(M_PI).imag(), LOOSE_EPS );

   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, cexpI(2*M_PI).real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, cexpI(2*M_PI).imag(), LOOSE_EPS );
   
   /* phase */
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, phase(dcomplex(1,0)), LOOSE_EPS );
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL(M_PI/2, phase(dcomplex(0,1)), LOOSE_EPS );
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL(M_PI, phase(dcomplex(-1,0)), LOOSE_EPS );
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-M_PI/2, phase(dcomplex(0,-1)), LOOSE_EPS );
}

#ifdef HAVE_SSE2

#include "math/m128dc.h"

void ComplexNumbers::SSE2_Test()
{
   dcomplex val[32] __attribute__((__aligned__));

   val[0] = dcomplex(1,2);
   val[1] = dcomplex(5,6);

   m128dc va(val[0]), vb(val[1]), vc;

   /* Load & Store */
   vc = va;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val[0].real(), val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val[0].imag(), val[2].imag(), TIGHT_EPS );

   /* operator* */
   vc = va * vb;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-7, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(16, val[2].imag(), TIGHT_EPS );

   /* operator*= */
   vc = va;
   vc *= vb;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-7, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(16, val[2].imag(), TIGHT_EPS );

   /* operator+ */
   vc = va + vb;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(6, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(8, val[2].imag(), TIGHT_EPS );

   /* operator+= */
   vc = va;
   vc += vb;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(6, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(8, val[2].imag(), TIGHT_EPS );

   /* operator- */
   vc = va - vb;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-4, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-4, val[2].imag(), TIGHT_EPS );

   /* operator-= */
   vc = va;
   vc -= vb;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-4, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-4, val[2].imag(), TIGHT_EPS );

   /* dcomplex  * I * double */
   vc = va.MulImag(5.0);
   vc.Store(val[2]);

   CPPUNIT_ASSERT_DOUBLES_EQUAL(-10, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(5, val[2].imag(), TIGHT_EPS );

   /* Scalar multiplication */
   vc = va * 3.0;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(3, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(6, val[2].imag(), TIGHT_EPS );

   /* Scalar multiplication */
   vc = va;
   vc *= 3.0;
   val[2] = 0;
   vc.Store(val[2]);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(3, val[2].real(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(6, val[2].imag(), TIGHT_EPS );


}
#endif



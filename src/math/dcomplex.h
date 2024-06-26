#ifndef DCOMPLEX_H
#define DCOMPLEX_H 

#include <iostream>
#include <cmath>

#ifdef HAVE_MPI
#include <mpi.h>
#endif

namespace QDLIB {
   
   struct Imaginary;
   
   
  /**
   * Simple class for complex numbers.
   * Implementet for double precission only.
   * 
   * \todo: Add support for \f$ r \cdot e^\varphi \f$ storage format.
   */
  struct dcomplex
  { 
    
    double _real, _imag;
   
    /** Get real part. */ 
    inline double real(){return _real;}
    /** Get imaginary part. */
    inline double imag(){return _imag;}
    
    /** Set real part. */
    inline void real(double d) {_real = d;}
    /** Set imaginary part. */
    inline void imag(double d) {_imag = d;}
    
    inline dcomplex() {}
    
    dcomplex(const double real): _real(real), _imag(0)  {}
  
    dcomplex(const double real, const double imag): _real(real), _imag(imag)  {}
  
    /** Complex conjugate. */
    inline dcomplex conj() const  __attribute__((always_inline))
    {
      dcomplex c;
      c._real = _real;
      c._imag = -_imag;
      return c;
    }
	
    /**
     * Complex conjugate.
     * 
     * Is carried out in place.
     */
    inline void conjugate()  __attribute__((always_inline))
    {
      _imag = -_imag;
    }

    inline void operator=(const dcomplex &B)  __attribute__((always_inline))
    {
	_real = B._real;
	_imag = B._imag;
    }
    
    inline dcomplex& operator=(const double B)  __attribute__((always_inline))
    {
      _real = B;
      _imag = 0;
      return *this;
    }
  
    inline dcomplex& operator=(const int B)  __attribute__((always_inline))
    {
      _real = double(B);
      _imag = 0;
      return *this;
  
    }
    
    void operator=(Imaginary i);
    
  }; /*struct dcomplex*/
  
#ifdef HAVE_MPI
  /**
   * Own mapping from dcomplex to MPI datatypes.
   * This class is a singleton.
   */
  class MPIdcomplex
  {
     private:
        MPI::Datatype _type;
        MPIdcomplex(MPIdcomplex &c) {}
        MPIdcomplex()
        {
           _type = MPI::DOUBLE.Create_contiguous(2);
           _type.Commit();
        }
        ~MPIdcomplex()
        {
           _type.Free();
        }
     public:
        /**
         * Get an Instance of the datatype.
         */
        static MPI::Datatype& Instance()
        {
           static MPIdcomplex ref;
           return ref._type;
        }
  };
#endif

#ifdef __GNUC__
  /* Force inline forward declarations */
  inline double conj(double d) __attribute__((always_inline));
  inline dcomplex conj(dcomplex c)  __attribute__((always_inline));
  inline dcomplex operator+(const dcomplex &A, const dcomplex &B) __attribute__((always_inline));
  inline dcomplex operator+(const dcomplex &A, const double &B) __attribute__((always_inline));
  inline dcomplex operator-(const dcomplex &A, const dcomplex &B) __attribute__((always_inline));
  inline dcomplex operator-(const dcomplex &A, const double &B) __attribute__((always_inline));
  inline void operator+=(dcomplex &A, const dcomplex &B) __attribute__((always_inline));
  inline void operator-=(dcomplex &A, const dcomplex &B) __attribute__((always_inline));
  inline dcomplex operator*(const dcomplex &A, const dcomplex &B) __attribute__((always_inline));
  inline void operator*=(dcomplex &A, const dcomplex &B) __attribute__((always_inline));
  inline dcomplex operator*(const dcomplex &A, const double &B) __attribute__((always_inline));
  inline void operator*=(dcomplex &A, const double &B) __attribute__((always_inline));
  inline dcomplex operator*(const double &B, const dcomplex &A) __attribute__((always_inline));
  inline dcomplex operator/(const dcomplex &A, const dcomplex &B) __attribute__((always_inline));
  inline void operator/=(dcomplex &A, const dcomplex &B) __attribute__((always_inline));
  inline dcomplex operator/(const dcomplex &A, const double &B) __attribute__((always_inline));
#endif

  /* Dummies to allow carry out complex conjugation no matter if real or complex */
  inline double conj(double d) {return d;}
  inline dcomplex conj(dcomplex c)  {return c.conj();}
  
  /*
  Operator Definitions
  */
  
  std::ostream& operator<<(std::ostream &s, const dcomplex c);
    

  inline dcomplex operator+(const dcomplex &A, const dcomplex &B)
  {
    dcomplex c;
    
    c._real = A._real + B._real;
    c._imag = A._imag + B._imag;
    
    return c;
  }
  
  

  inline dcomplex operator+(const dcomplex &A, const double &B)
  {
    dcomplex c;
    
    c._real = A._real + B;
    c._imag = A._imag;
    
    return c;
  }
  
  inline dcomplex operator-(const dcomplex &A, const dcomplex &B)
  {
    dcomplex c;
    
    c._real = A._real - B._real;
    c._imag = A._imag - B._imag;
    
    return c;
  }
  
  
  inline dcomplex operator-(const dcomplex &A, const double &B)
  {
    dcomplex c;
    
    c._real = A._real - B;
    c._imag = A._imag;
    
    return c;
  }

  inline void operator+=(dcomplex &A, const dcomplex &B)
  {
    A = A + B;
  }
  
  inline void operator-=(dcomplex &A, const dcomplex &B)
  {
     A = A - B;
  }
  
  inline dcomplex operator*(const dcomplex &A, const dcomplex &B)
  {
    dcomplex c;
    
    c._real = A._real*B._real - A._imag * B._imag;
    c._imag = A._real*B._imag + A._imag * B._real;
	
    return c;
  }
  
  inline void operator*=(dcomplex &A, const dcomplex &B)
  {
    A=A*B;
  }
  
  
  inline dcomplex operator*(const dcomplex &A, const double &B)
  {
    dcomplex c;
      c._real = A._real * B;
      c._imag = A._imag * B;
    return c;
  }
  
  inline void operator*=(dcomplex &A, const double &B)
  {
    A=A*B;
  }
  
  
  inline dcomplex operator*(const double &B, const dcomplex &A)
  {
    return A*B;
  }

  inline dcomplex operator/(const double &A, const dcomplex &B)
  {
    dcomplex c;
    double d;

    d = B._real*B._real + B._imag*B._imag;
    c._real =   A * B._real;
    c._imag = - A * B._imag;
    c._real /= d;
    c._imag /= d;
    return c;
  }

  
  inline dcomplex operator/(const dcomplex &A, const dcomplex &B)
  {
    dcomplex c;
    double d;
    
    d = B._real*B._real + B._imag*B._imag;
    c._real = A._real * B._real + A._imag * B._imag;
    c._imag = A._imag * B._real - A._real * B._imag;
    c._real /= d;
    c._imag /= d;
    return c;
  }
  
  inline void operator/=(dcomplex &A, const dcomplex &B)
  {
    A = A / B;
  }
  
  inline dcomplex operator/(const dcomplex &A, const double &B)
  {
    dcomplex c;
    
    c._real = A._real / B;
    c._imag = A._imag / B;
    
    return c;
  }
  

  /*
  Functions
  */
  
  /** dcomplex(0,2.)
   * Calculates \f$ e^{i phi} \f$.
   * 
   * This ist faster than cexp(), which is more general
   */
  inline  dcomplex cexpI(double phi)
  {
    dcomplex c;
    c._real = cos(phi);
    c._imag = sin(phi);
    return c;
  }
  
  /**
   * Calculates \f$ e^{z} = e^{i Re(z)} e^{i Im(z)}\f$.
   */
  inline dcomplex cexp(dcomplex z)
  {
    dcomplex d;
    d = cexpI(z._imag);
    d *= exp(z._real);
    return d;
  }
  

  /** Absolute value of complex number.*/
  inline double cabs(dcomplex z)
  {
    return sqrt(z._real * z._real + z._imag * z._imag);
  }
 
  /** Phase angle of complex number.*/
  inline double phase(dcomplex z)
  {
     double r;
     r = cabs(z);
     if (r == 0) return NAN;
     if (z._imag >= 0)
        return acos(z._real / r);
     else 
        return -acos(z._real / r);
  }
  
  
  /**
   * Represents the imaginary unit \f$ \sqrt(-1) \f$.
   */
  struct Imaginary
  {
    inline dcomplex operator*(const dcomplex &c)
    {
      dcomplex d;
      d._real = -c._imag;
      d._imag = c._real;
      return d;
    }
    
    inline dcomplex operator*(const double d)
    {
      dcomplex c;
      
      c._real = 0;
      c._imag = d;
      
      return c;
    }
  }; /* struct Imaginary */
  
  inline dcomplex operator*(dcomplex c, Imaginary i)
  {
    dcomplex d;
    
    d._real = -c.imag();
    d._imag = c.real();
    
    return d;
  }
  
  inline dcomplex operator*(const double d, Imaginary i)
  {
    dcomplex c;
    
    c._real = 0;
    c._imag = d;
    
    return c;
  }
  
  inline double operator*(Imaginary i, Imaginary i1)
  {
    return -1;
  }
  
  /**
   * Power of imaginary unit.
   */
  inline dcomplex cpow(Imaginary i, int n)
  {
     int rem = n % 4;

     switch (rem){
        case 0: return dcomplex(1);
        case 1: return dcomplex(0,1);
        case 2: return dcomplex(-1);
        case 3: return dcomplex(0,-1);
     }
  }

  /**
   * Complex square root
   */
  inline dcomplex csqrt(dcomplex c)
  {
     double r  = cabs(c);
     double phi = phase(c);

     return sqrt(r) * cexpI(phi/2);
  }

  extern Imaginary I;
}

#endif


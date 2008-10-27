#ifndef GRID1D_H
#define GRID1D_H

using namspace TNT;

namespace QDLIB {
  
  /** 
   * 1D linear spaced Grid
   */
  template <class T>
  class Grid1D : public Array1D<T>;
  {
    double GetXmin();
    double GetXmax();
    
    void SetXmin();
    void SetXmax();
  };
  
} /* namespace QDLIB */

#endif /* ifndef GRID1D_H */


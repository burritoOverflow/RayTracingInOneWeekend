#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class Vector3 {
   public:
    Vector3() : e_{0, 0, 0} {}
    Vector3(double e0, double e1, double e2) : e_{e0, e1, e2} {}

    double x() const { return e_[0]; }
    double y() const { return e_[1]; }
    double z() const { return e_[2]; }

   private:
    double e_[3];
};

#endif  // VEC3_H
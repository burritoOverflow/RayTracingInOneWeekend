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

    Vector3 operator-() const { return Vector3(-e_[0], -e_[1], -e_[2]); }
    double operator[](int i) const { return e_[i]; }
    double& operator[](int i) { return e_[i]; }

    Vector3& operator+=(const Vector3& v) {
        e_[0] += v.x();
        e_[1] += v.y();
        e_[2] += v.z();
        return *this;
    }

    Vector3& operator*=(double d) {
        e_[0] *= d;
        e_[1] *= d;
        e_[2] *= d;
        return *this;
    }

    Vector3& operator/=(double d) { return *this *= 1 / d; }

    double length() const { return std::sqrt(length_squared()); }

    double length_squared() const { return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2]; }

   private:
    double e_[3];
};

#endif  // VEC3_H
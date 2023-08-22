#ifndef RAYTRACINGINONEWEEKEND_VEC3_H
#define RAYTRACINGINONEWEEKEND_VEC3_H

#include <cmath>
#include <iostream>

class Vector3 {
   public:
    Vector3() : e_{0, 0, 0} {}
    Vector3(double e0, double e1, double e2) : e_{e0, e1, e2} {}

    double x() const { return e_[0]; }
    double y() const { return e_[1]; }
    double z() const { return e_[2]; }

    Vector3 operator-() const { return {-e_[0], -e_[1], -e_[2]}; }
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

    double Length() const { return std::sqrt(LengthSquared()); }

    double LengthSquared() const { return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2]; }

   private:
    double e_[3];
};

inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vector3 operator+(const Vector3& u, const Vector3& v) {
    return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z()};
}

inline Vector3 operator-(const Vector3& u, const Vector3& v) {
    return {u.x() - v.x(), u.y() - v.y(), u.z() - v.z()};
}

inline Vector3 operator*(const Vector3& u, const Vector3& v) {
    return {u.x() * v.x(), u.y() * v.y(), u.z() * v.z()};
}

inline Vector3 operator*(double d, const Vector3& v) {
    return {d * v.x(), d * v.y(), d * v.z()};
}

inline Vector3 operator*(const Vector3& v, double d) {
    return d * v;
}

inline Vector3 operator/(Vector3 v, double d) {
    return (1 / d) * v;
}

inline double Dot(const Vector3& u, const Vector3& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline Vector3 Cross(const Vector3& u, const Vector3& v) {
    return {u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
            u.x() * v.y() - u.y() * v.x()};
}

inline Vector3 UnitVector(const Vector3 v) {
    return v / v.Length();
}

using Point3 = Vector3;

#endif  // RAYTRACINGINONEWEEKEND_VEC3_H

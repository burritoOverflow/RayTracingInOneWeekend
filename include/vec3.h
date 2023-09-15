#ifndef RAYTRACINGINONEWEEKEND_VEC3_H
#define RAYTRACINGINONEWEEKEND_VEC3_H

#include <cmath>
#include <iostream>
#include "config.h"

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

    // length (or norm) of a vector is the square root of the sum of the squares of
    // its entries, i.e sqrt(x^2 + y^2 + z^2)
    double Length() const { return std::sqrt(LengthSquared()); }

    double LengthSquared() const { return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2]; }

    bool IsNearZero() const {
        auto s = 1e-8;
        return (fabs(e_[0]) < s) && (fabs(e_[1]) < s) && (fabs(e_[2]) < s);
    }

    static inline Vector3 GenerateRandomVector() {
        return {config::GetRandomDouble(), config::GetRandomDouble(), config::GetRandomDouble()};
    }

    static inline Vector3 GenerateRandomVector(const double min, const double max) {
        return {config::GetRandomDouble(min, max), config::GetRandomDouble(min, max),
                config::GetRandomDouble(min, max)};
    }

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

/*
 *  | Uy*Vz - Uz*Vy |
 *  | Uz*Vx - UxVz |
 *  | UxVy - UyVx |
 */
inline Vector3 Cross(const Vector3& u, const Vector3& v) {
    return {u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(), u.x() * v.y() - u.y() * v.x()};
}

// unit vector - vector whose length is 1
inline Vector3 UnitVector(const Vector3 v) {
    // normalizing a vector - convert nonzero vector V to
    // a unit vector by dividing by the vector length
    return v / v.Length();
}

// repeatedly generate random samples until a valid one is generated (inside the unit
// sphere) (see 9.1)
static inline Vector3 VectorInUnitSphere() {
    while (true) {
        const auto p = Vector3::GenerateRandomVector(-1, 1);
        if (p.LengthSquared() < 1)
            return p;
    }
}

static inline Vector3 RandomInUnitDisk() {
    while (true) {
        const auto p = Vector3(config::GetRandomDouble(-1, 1), config::GetRandomDouble(-1, 1), 0);

        if (p.LengthSquared() < 1)
            return p;
    }
}

// normalize to get a vector in the unit sphere
static inline Vector3 RandomUnitVector() {
    return UnitVector(VectorInUnitSphere());
}

static inline Vector3 RandomVectorOnHemisphere(const Vector3& normal_vector) {
    const Vector3 on_unit_sphere = RandomUnitVector();

    // in the same hemisphere as the normal
    if (Dot(on_unit_sphere, normal_vector) > 0.0) {
        return on_unit_sphere;
    } else {
        // otherwise, invert the vector
        return -on_unit_sphere;
    }
}

static inline Vector3 Reflect(const Vector3& v, const Vector3& n) {
    // see 10.4
    return v - 2 * Dot(v, n) * n;
}

// see 11.2
static inline Vector3 Refract(const Vector3& uv, const Vector3& n, const double etai_over_etat) {
    auto cos_theta = fmin(Dot(-uv, n), 1.0);
    Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.LengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}

using Point3 = Vector3;

#endif  // RAYTRACINGINONEWEEKEND_VEC3_H

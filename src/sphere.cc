#include "sphere.h"
#include "interval.h"
#include "vec3.h"

bool Sphere::Hit(const Ray& ray, Interval ray_t, HitRecord& hit_record) const {
    // see walkthrough in section 5.1 (and the subsequent changes in section 6.2)
    const Point3 center = this->is_moving_ ? GetSphereCenter(ray.time()) : this->center_;
    // TODO - why is this subtraction backwards? (see original:
    // https://github.com/RayTracing/raytracing.github.io/blob/release/src/InOneWeekend/sphere.h#L23)
    const Vector3 oc = ray.origin() - center;
    const auto a = ray.direction().LengthSquared();
    const auto half_b = Dot(oc, ray.direction());
    const auto c = oc.LengthSquared() - radius_ * radius_;
    const auto discriminant = half_b * half_b - a * c;

    // point not on the surface (in the sphere)
    if (discriminant < 0) {
        return false;
    }

    const auto sqrtd = sqrt(discriminant);

    // find the nearest root in the range (see 6.3)
    auto root = (-half_b - sqrtd) / a;

    if (!ray_t.Surrounds(root)) {
        root = (-half_b + sqrtd) / a;

        if (!ray_t.Surrounds(root))
            return false;
    }

    hit_record.t_ = root;
    hit_record.point_ = ray.at(hit_record.t_);

    // 6.4 front vs back faces
    const Vector3 outward_normal = (hit_record.point_ - center_) / radius_;
    hit_record.SetFaceNormal(ray, outward_normal);
    SetSphereUVCoordinates(outward_normal, hit_record.u_, hit_record.v_);
    hit_record.SetMaterial(material_);

    return true;
}

/*
    Section 4.4
    Description lifted directly from:
    https://raytracing.github.io/books/RayTracingTheNextWeek.html#texturemapping/texturecoordinatesforspheres

    point: a given point on the sphere of radius one, centered at the origin.
    u: returned value [0,1] of angle around the Y axis from X=-1.
    v: returned value [0,1] of angle from Y=-1 to Y=+1.
        <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
*/
void Sphere::SetSphereUVCoordinates(const Point3& point, double& u, double& v) {
    const auto phi = atan2(-point.z(), point.x()) + config::PI;
    const auto theta = acos(-point.y());
    u = phi / (2 * config::PI);
    v = theta / config::PI;
}
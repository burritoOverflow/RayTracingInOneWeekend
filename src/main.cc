#include "camera.h"
#include "dielectric.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "sphere.h"
#include "vec3.h"

static HittableList GenerateRandomWorld() {
    HittableList world;

    const auto ground_material = std::make_shared<Lambertian>(color::Color(0.5, 0.5, 0.5));
    world.AddObject(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int i = -11; i <= 11; ++i) {
        for (int j = -11; j <= 11; ++j) {
            const auto choose_material = config::GetRandomDouble();
            const Point3 center{i + 0.9 * config::GetRandomDouble(), 0.2, j + 0.9 * config::GetRandomDouble()};

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                if (choose_material < 0.8) {
                    // diffuse
                    auto albedo = Vector3::GenerateRandomVector() * Vector3::GenerateRandomVector();
                    world.AddObject(
                        std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(albedo)));
                } else if (choose_material < 0.95) {
                    // metal
                    auto albedo = Vector3::GenerateRandomVector(0.5, 1);
                    auto fuzz = config::GetRandomDouble(0, 0.5);
                    world.AddObject(
                        std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                } else {
                    // glass
                    world.AddObject(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }  // end inner loop
    }      // end outer loop

    world.AddObject(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, std::make_shared<Dielectric>(1.5)));

    world.AddObject(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0,
                                             std::make_shared<Lambertian>(color::Color(0.4, 0.2, 0.1))));

    world.AddObject(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0,
                                             std::make_shared<Metal>(color::Color(0.7, 0.6, 0.5), 0.0)));

    return world;
}

int main() {
    Camera camera;
    camera.SetVerticalFieldOfView(20);

    Point3 look_from = Point3(13, 2, 3);
    Point3 look_at = Point3(0, 0, 0);

    camera.SetLookFrom(look_from);
    camera.SetLookAt(look_at);

    Vector3 v_up = Vector3(0, 1, 0);
    camera.SetViewUpVector(v_up);

    camera.SetMaxRecursionDepth(50);

    camera.SetDefocusAngle(0.6);
    camera.SetFocusDistance(10.0);

    const auto world = GenerateRandomWorld();
    camera.Render(world);
}

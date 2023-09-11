#include "camera.h"
#include "dielectric.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "sphere.h"
#include "vec3.h"

int main() {
    // add a "world"
    HittableList world;

    auto material_ground = std::make_shared<Lambertian>(color::Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(color::Color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<Dielectric>(1.5);
    auto material_right = std::make_shared<Metal>(color::Color(0.8, 0.6, 0.2), 0.0);

    world.AddObject(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.AddObject(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.AddObject(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.AddObject(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.4, material_left));
    world.AddObject(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    Camera camera;
    camera.SetVerticalFieldOfView(90);  // 90 degree field of view
    camera.SetMaxRecursionDepth(50);
    camera.Render(world);
}

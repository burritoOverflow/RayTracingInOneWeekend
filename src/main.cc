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

    const auto R = cos(config::PI / 4);

    auto material_left = std::make_shared<Lambertian>(color::Color(0, 0, 1));
    auto material_right = std::make_shared<Lambertian>(color::Color(1, 0, 0));

    world.AddObject(std::make_shared<Sphere>(Point3(-R, 0, -1), R, material_left));
    world.AddObject(std::make_shared<Sphere>(Point3(R, 0, -1), R, material_right));

    Camera camera;
    camera.SetVerticalFieldOfView(90);  // 90 degree field of view
    camera.SetMaxRecursionDepth(50);
    camera.Render(world);
}

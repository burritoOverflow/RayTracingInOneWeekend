#if defined(__APPLE__)
#include <sstream>
#endif
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

int main() {
    // add a "world"
    HittableList world;

    world.AddObject(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.AddObject(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera camera;
    camera.Render(world);
}

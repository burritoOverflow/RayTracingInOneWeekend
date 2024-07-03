#include <getopt.h>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include "bvhnode.h"
#include "camera.h"
#include "color.h"
#include "config.h"
#include "dielectric.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "sphere.h"
#include "texture.h"
#include "vec3.h"

enum RenderOption {
    kBouncingSpheres,
    kCheckeredSpheres,
};

Camera ConfigureCameraForRender(const RenderOption render_option) {
    Camera camera;

    switch (render_option) {
        case kBouncingSpheres: {
            camera.SetVerticalFieldOfView(20);

            const Point3 look_from = Point3(13, 2, 3);
            const Point3 look_at = Point3(0, 0, 0);

            camera.SetLookFrom(look_from);
            camera.SetLookAt(look_at);

            const Vector3 v_up = Vector3(0, 1, 0);

            camera.SetViewUpVector(v_up);
            camera.SetMaxRecursionDepth(50);
            camera.SetDefocusAngle(0.6);
            camera.SetFocusDistance(10.0);
            break;
        }

            // section 4.3 example
        case kCheckeredSpheres: {
            camera.SetAspectRatio(16.0 / 9.0);
            camera.SetSamplesPerPixel(100);
            camera.SetMaxRecursionDepth(50);
            camera.SetVerticalFieldOfView(20.0);
            camera.SetLookFrom(Point3(13, 2, 3));
            camera.SetLookAt(Point3(0, 0, 0));
            camera.SetViewUpVector(Vector3(0, 1, 0));
            camera.SetDefocusAngle(0.0);
            break;
        }
    }

    return camera;
}

static void RenderCheckeredSpheresWorld() {
    HittableList world{};
    const auto checker =
        std::make_shared<CheckerTexture>(0.32, color::Color(.2, .3, .1), color::Color(.9, .9, .9));

    const double radius = 10;
    const std::shared_ptr<Lambertian> checker_mat_ptr = std::make_shared<Lambertian>(checker);

    auto sphere = std::make_shared<Sphere>(Point3(0, -10, 0), radius, checker_mat_ptr);
    auto sphere2 = std::make_shared<Sphere>(Point3(0, 10, 0), radius, checker_mat_ptr);

    world.AddObject(sphere);
    world.AddObject(sphere2);

    auto camera = ConfigureCameraForRender(kCheckeredSpheres);
    camera.Render(world);
}

static void RenderBouncingSpheresWorld() {
    HittableList world{};

    const auto checker =
        std::make_shared<CheckerTexture>(0.32, color::Color(.2, .3, .1), color::Color(.9, .9, .9));
    const auto lambertian = std::make_shared<Lambertian>(checker);
    const auto sphere = std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, lambertian);
    world.AddObject(sphere);

    std::shared_ptr<Material> ground_material = std::make_shared<Lambertian>(color::Color(0.5, 0.5, 0.5));
    auto center_point = Point3(0, -1000, 0);
    world.AddObject(std::make_shared<Sphere>(center_point, 1000, ground_material));

    for (int i = -11; i <= 11; ++i) {
        for (int j = -11; j <= 11; ++j) {
            const auto choose_material = config::GetRandomDouble();
            const Point3 center{i + 0.9 * config::GetRandomDouble(), 0.2, j + 0.9 * config::GetRandomDouble()};

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                if (choose_material < 0.8) {
                    // diffuse
                    const auto albedo = Vector3::GenerateRandomVector() * Vector3::GenerateRandomVector();
                    const std::shared_ptr<Material> sphere_material = std::make_shared<Lambertian>(albedo);
                    world.AddObject(std::make_shared<Sphere>(center, 0.2, sphere_material));

                    // see section 2.6 in `TheNextWeek`
                    // each Sphere moves from center C at time t==0 to C + (0, r/2, 0) at time t==1
                    const auto center2 = center + Vector3(0, config::GetRandomDouble(0, 0.5), 0);
                    world.AddObject(std::make_shared<Sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_material < 0.95) {
                    // metal
                    const auto albedo = Vector3::GenerateRandomVector(0.5, 1);
                    const auto fuzz = config::GetRandomDouble(0, 0.5);
                    const std::shared_ptr<Material> metal_ptr = std::make_shared<Metal>(albedo, fuzz);
                    world.AddObject(std::make_shared<Sphere>(center, 0.2, metal_ptr));
                } else {
                    // glass
                    const std::shared_ptr<Material> dielectric_ptr = std::make_shared<Dielectric>(1.5);
                    world.AddObject(std::make_shared<Sphere>(center, 0.2, dielectric_ptr));
                }
            }
        }  // end inner loop
    }      // end outer loop

    const std::shared_ptr<Material> dielectric_material_ptr = std::make_shared<Dielectric>(1.5);
    const auto point1 = Point3(0, 1, 0);
    world.AddObject(std::make_shared<Sphere>(point1, 1.0, dielectric_material_ptr));

    const std::shared_ptr<Material> lambertian_ptr = std::make_shared<Lambertian>(color::Color(0.4, 0.2, 0.1));
    const auto point2 = Point3(-4, 1, 0);
    world.AddObject(std::make_shared<Sphere>(point2, 1.0, lambertian_ptr));

    const std::shared_ptr<Material> metal_ptr = std::make_shared<Metal>(color::Color(0.7, 0.6, 0.5), 0.0);
    const auto point3 = Point3(4, 1, 0);
    world.AddObject(std::make_shared<Sphere>(point3, 1.0, metal_ptr));

    // addition of BVHNode
    world = HittableList{std::make_shared<BoundingVolumeHierarchyNode>(world)};

    Camera camera = ConfigureCameraForRender(kBouncingSpheres);
    camera.Render(world);
}

static void ShowHelp() {
    std::cerr << "--render <arg>        The target to render {checkered-spheres, bouncing-spheres};" << '\n';
    exit(EXIT_FAILURE);
}

static std::optional<std::string> ParseArgs(int argc, char** argv) {
    // i dont fully understand why this work for short opts? (as opposed to adding "r")
    const char* const short_opts = "";
    const option long_opts[] = {{"render", required_argument, nullptr, 'r'}};
    std::optional<std::string> render_option{};

    while (true) {
        const auto option = getopt_long_only(argc, argv, short_opts, long_opts, nullptr);
        if (option == -1) {
            break;
        }

        switch (option) {
            case 'r': {
                render_option = std::string(optarg);
                break;
            }
            case 'h':
            default:
                ShowHelp();
                break;
        }
    }
    return render_option;
}

static void RunRender(const std::optional<std::string>& maybe_render_opt_str) {
    if (maybe_render_opt_str->empty()) {
        ShowHelp();
    }

    std::string render_opt_str = *maybe_render_opt_str;
    const std::map<std::string, RenderOption> option_map{{"checkered-spheres", kCheckeredSpheres},
                                                         {"bouncing-spheres", kBouncingSpheres}};

    const auto result = option_map.find(render_opt_str);
    if (result == option_map.end()) {
        ShowHelp();
    }

    switch (result->second) {
        case kCheckeredSpheres:
            RenderCheckeredSpheresWorld();
            break;
        case kBouncingSpheres:
            RenderBouncingSpheresWorld();
            break;
    }
}

int main(int argc, char** argv) {
    RunRender(ParseArgs(argc, argv));
    return 0;
}

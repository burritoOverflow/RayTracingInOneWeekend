#include <getopt.h>
#include <sys/types.h>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include "bvhnode.h"
#include "camera.h"
#include "config.h"
#include "dielectric.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "sphere.h"
#include "texture.h"
#include "vec3.h"

// choice of pre-defined rendered scene
enum RenderedSceneOption { kBouncingSpheres, kCheckeredSpheres, kEarthTexture };

// contains the args provided via the longopts.
struct Args {
    std::optional<std::string> render_option_;  // string corresponding to the rendered scene
    std::optional<u_int16_t> image_width_;      // desired width of the image
};

// set Camera state for a given render, via the RenderSceneOption provided
// set the camera's image_width, if the parameter is present.
Camera ConfigureCameraForRender(const RenderedSceneOption render_option,
                                const std::optional<u_int16_t>& image_width = std::nullopt) {
    Camera camera{};

    // TODO - many of these are "default options" or otherwise shared
    // should be refactored to reflect this clutter here
    switch (render_option) {
        case kBouncingSpheres: {
            camera.SetVerticalFieldOfView(20.0);

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

        case kEarthTexture:
            camera.SetAspectRatio(16.0 / 9.0);
            camera.SetSamplesPerPixel(100);
            camera.SetMaxRecursionDepth(50);
            camera.SetVerticalFieldOfView(20.0);
            camera.SetLookFrom(Point3(0, 0, 12));
            camera.SetLookAt(Point3(0, 0, 0));
            camera.SetViewUpVector(Vector3(0, 1, 0));
            camera.SetDefocusAngle(0.0);
            break;
    }

    if (image_width.has_value()) {
        camera.SetImageWidth(image_width.value());
    }
    return camera;
}

static void RenderCheckeredSpheresWorld(const std::optional<u_int16_t>& image_width) {
    HittableList world{};
    const auto checker =
        std::make_shared<CheckerTexture>(0.32, color::Color(.2, .3, .1), color::Color(.9, .9, .9));

    const double radius = 10;
    const std::shared_ptr<Lambertian> checker_mat_ptr = std::make_shared<Lambertian>(checker);

    auto sphere = std::make_shared<Sphere>(Point3(0, -10, 0), radius, checker_mat_ptr);
    auto sphere2 = std::make_shared<Sphere>(Point3(0, 10, 0), radius, checker_mat_ptr);

    world.AddObject(sphere);
    world.AddObject(sphere2);

    auto camera = ConfigureCameraForRender(kCheckeredSpheres, image_width);
    camera.Render(world);
}

static void RenderBouncingSpheresWorld(const std::optional<u_int16_t>& image_width) {
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
    }  // end outer loop

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

    Camera camera = ConfigureCameraForRender(kBouncingSpheres, image_width);
    camera.Render(world);
}

static void RenderEarthTextureWorld(const std::optional<u_int16_t>& image_width) {
    const auto earth_image = "earthmap.jpg";
    const auto earth_texture = std::make_shared<ImageTexture>(earth_image);
    const auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    const auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

    auto world = HittableList(globe);
    Camera camera = ConfigureCameraForRender(kEarthTexture, image_width);
    camera.Render(world);
}

static void ShowHelp() {
    std::cerr << "--render <arg>        The target to render {checkered-spheres, bouncing-spheres}" << '\n';
    std::cerr << "--image-width <arg>        The width of the rendered scene" << '\n';
    exit(EXIT_FAILURE);
}

static Args ParseArgs(int argc, char** argv) {
    const char* const short_opts = "r:i:";
    const option long_opts[] = {{"render", required_argument, nullptr, 'r'},
                                {"image-width", required_argument, nullptr, 'i'}};

    std::optional<std::string> render_option{};
    std::optional<u_int16_t> image_width{};

    while (true) {
        const auto option = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (option == -1) {
            break;
        }

        switch (option) {
            case 'r': {
                render_option = std::string(optarg);
                break;
            }
            case 'i': {
                // this makes no sense why this is "required"
                // I'm clearly not using this API correctly.
                if (optarg) {
                    image_width = std::stoi(optarg);
                }
                break;
            }
            case 'h':
            default:
                ShowHelp();
                break;
        }
    }
    return {render_option, image_width};
}

static void RunRender(const Args& args) {
    // requires the "render" arg
    const std::optional<std::string> maybe_render_opt_str = args.render_option_;
    if (maybe_render_opt_str->empty()) {
        ShowHelp();
    }

    const std::string& render_opt_str = *maybe_render_opt_str;
    const std::map<std::string, RenderedSceneOption> option_map{{"checkered-spheres", kCheckeredSpheres},
                                                                {"bouncing-spheres", kBouncingSpheres},
                                                                {"earth-texture", kEarthTexture}};

    const auto result = option_map.find(render_opt_str);
    if (result == option_map.end()) {
        ShowHelp();
    }

    switch (result->second) {
        case kCheckeredSpheres:
            RenderCheckeredSpheresWorld(args.image_width_);
            break;
        case kBouncingSpheres:
            RenderBouncingSpheresWorld(args.image_width_);
            break;
            // TODO
        case kEarthTexture:
            RenderEarthTextureWorld(args.image_width_);
            break;
    }
}

int main(int argc, char** argv) {
    RunRender(ParseArgs(argc, argv));
    return 0;
}

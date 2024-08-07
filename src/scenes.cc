#include "scenes.h"

// set Camera state for a given render, via the RenderSceneOption provided
// set the camera's image_width, if the parameter is present.
static Camera ConfigureCameraForRender(const args::RenderedSceneOption render_option,
                                       const std::optional<u_int16_t>& image_width = std::nullopt) {
    const color::Color DEFAULT_RENDER_BACKGROUND_COLOR{0.7, 0.8, 1.0};
    Camera camera{};
    camera.SetBackgroundColor(DEFAULT_RENDER_BACKGROUND_COLOR);

    // TODO - many of these are "default options" or otherwise shared
    // should be refactored to reflect this clutter here
    switch (render_option) {
        case args::kBouncingSpheres: {
            camera.SetVerticalFieldOfView(20.0);
            camera.SetLookFrom(Point3(13, 2, 3));
            camera.SetLookAt(Point3(0, 0, 0));
            camera.SetViewUpVector(Vector3(0, 1, 0));
            camera.SetMaxRecursionDepth(50);
            camera.SetDefocusAngle(0.6);
            camera.SetFocusDistance(10.0);
            break;
        }

            // section 4.3 example
        case args::kCheckeredSpheres: {
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

        case args::kEarthTexture:
            camera.SetAspectRatio(16.0 / 9.0);
            camera.SetSamplesPerPixel(100);
            camera.SetMaxRecursionDepth(50);
            camera.SetVerticalFieldOfView(20.0);
            camera.SetLookFrom(Point3(0, 0, 12));
            camera.SetLookAt(Point3(0, 0, 0));
            camera.SetViewUpVector(Vector3(0, 1, 0));
            camera.SetDefocusAngle(0.0);
            break;

        case args::kPerlinSpheres:
            camera.SetAspectRatio(16.0 / 9.0);
            camera.SetSamplesPerPixel(100);
            camera.SetMaxRecursionDepth(50);
            camera.SetVerticalFieldOfView(20.0);
            camera.SetLookFrom(Point3(13, 2, 3));
            camera.SetLookAt(Point3(0, 0, 0));
            camera.SetViewUpVector(Vector3(0, 1, 0));
            camera.SetDefocusAngle(0.0);
            break;

        case args::kQuads:
            camera.SetAspectRatio(1.0);
            camera.SetSamplesPerPixel(100);
            camera.SetMaxRecursionDepth(50);
            camera.SetVerticalFieldOfView(80);
            camera.SetLookFrom(Point3(0, 0, 9));
            camera.SetLookAt(Point3(0, 0, 0));
            camera.SetDefocusAngle(0.0);
            break;

        case args::kSimpleLight:
            camera.SetAspectRatio(16.0 / 9.0);
            camera.SetSamplesPerPixel(100);
            camera.SetMaxRecursionDepth(50);
            camera.SetVerticalFieldOfView(20.0);
            camera.SetDefocusAngle(0.0);
            camera.SetLookFrom(Point3(26, 3, 6));
            camera.SetLookAt(Point3(0, 2, 0));
            camera.SetViewUpVector(Vector3(0, 1, 0));
            camera.SetBackgroundColor(color::Color(0, 0, 0));
            break;

        case args::kCornellBox:
            camera.SetAspectRatio(1.0);
            camera.SetVerticalFieldOfView(40);
            camera.SetSamplesPerPixel(200);
            camera.SetMaxRecursionDepth(50);
            camera.SetBackgroundColor(color::Color(0, 0, 0));
            camera.SetLookFrom(Point3(278, 278, -800));
            camera.SetLookAt(Point3(278, 278, 0));
            camera.SetViewUpVector(Vector3(0, 1, 0));
            camera.SetDefocusAngle(0.0);
            break;
    }

    if (image_width.has_value()) {
        camera.SetImageWidth(image_width.value());
    }
    return camera;
}

void RayTracingScenes::RenderCheckeredSpheresWorld(const std::optional<u_int16_t>& image_width) {
    HittableList world{};
    const auto checker =
        std::make_shared<CheckerTexture>(0.32, color::Color(.2, .3, .1), color::Color(.9, .9, .9));

    const double radius = 10;
    const std::shared_ptr<Lambertian> checker_mat_ptr = std::make_shared<Lambertian>(checker);

    const auto sphere = std::make_shared<Sphere>(Point3(0, -10, 0), radius, checker_mat_ptr);
    const auto sphere2 = std::make_shared<Sphere>(Point3(0, 10, 0), radius, checker_mat_ptr);

    world.AddObject(sphere);
    world.AddObject(sphere2);

    auto camera = ConfigureCameraForRender(args::kCheckeredSpheres, image_width);
    camera.Render(world);
}

void RayTracingScenes::RenderBouncingSpheresWorld(const std::optional<u_int16_t>& image_width) {
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

    Camera camera = ConfigureCameraForRender(args::kBouncingSpheres, image_width);
    camera.Render(world);
}

void RayTracingScenes::RenderEarthTextureWorld(const std::optional<u_int16_t>& image_width) {
    const auto earth_image = "earthmap.jpg";
    const auto earth_texture = std::make_shared<ImageTexture>(earth_image);
    const auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    const auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

    auto world = HittableList(globe);
    Camera camera = ConfigureCameraForRender(args::kEarthTexture, image_width);
    camera.Render(world);
}

void RayTracingScenes::RenderPerlinSpheres(const std::optional<u_int16_t>& image_width) {
    auto world = HittableList();
    const double scale{4};
    auto pertext = std::make_shared<NoiseTexture>(scale);
    world.AddObject(
        std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    world.AddObject(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    Camera camera = ConfigureCameraForRender(args::kPerlinSpheres, image_width);
    camera.Render(world);
}

void RayTracingScenes::RenderQuads(const std::optional<u_int16_t>& image_width) {
    auto left_red = std::make_shared<Lambertian>(color::Color(1.0, 0.2, 0.2));
    auto back_green = std::make_shared<Lambertian>(color::Color(0.2, 1.0, 0.2));
    auto right_blue = std::make_shared<Lambertian>(color::Color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<Lambertian>(color::Color(1.0, 0.5, 0.0));
    auto lower_teal = std::make_shared<Lambertian>(color::Color(0.2, 0.8, 0.8));

    auto world = HittableList();

    world.AddObject(std::make_shared<Quad>(Point3(-3, -2, 5), Vector3(0, 0, -4), Vector3(0, 4, 0), left_red));
    world.AddObject(std::make_shared<Quad>(Point3(-2, -2, 0), Vector3(4, 0, 0), Vector3(0, 4, 0), back_green));
    world.AddObject(std::make_shared<Quad>(Point3(3, -2, 1), Vector3(0, 0, 4), Vector3(0, 4, 0), right_blue));
    world.AddObject(
        std::make_shared<Quad>(Point3(-2, 3, 1), Vector3(4, 0, 0), Vector3(0, 0, 4), upper_orange));
    world.AddObject(
        std::make_shared<Quad>(Point3(-2, -3, 5), Vector3(4, 0, 0), Vector3(0, 0, -4), lower_teal));

    Camera camera = ConfigureCameraForRender(args::kQuads, image_width);
    camera.Render(world);
}

void RayTracingScenes::RenderSimpleLight(const std::optional<u_int16_t>& image_width) {
    auto world = HittableList();
    auto pertext = std::make_shared<NoiseTexture>(4);

    world.AddObject(
        std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    world.AddObject(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    const auto difflight_color = color::Color(4, 4, 4);
    const auto difflight = std::make_shared<DiffuseLight>(difflight_color);

    world.AddObject(std::make_shared<Sphere>(Point3(0, 7, 0), 2, difflight));
    world.AddObject(std::make_shared<Quad>(Point3(3, 1, -2), Vector3(2, 0, 0), Vector3(0, 2, 0), difflight));

    Camera camera = ConfigureCameraForRender(args::kSimpleLight, image_width);
    camera.Render(world);
}

// section 7.4 && section 8
void RayTracingScenes::RenderCornellBox(const std::optional<u_int16_t>& image_width) {
    auto world = HittableList();

    const auto red = std::make_shared<Lambertian>(color::Color(.65, .05, .05));
    const auto white = std::make_shared<Lambertian>(color::Color(.73, .73, .73));
    const auto green = std::make_shared<Lambertian>(color::Color(.12, .45, .15));
    const auto light = std::make_shared<DiffuseLight>(color::Color(15, 15, 15));

    world.AddObject(std::make_shared<Quad>(Point3(555, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), green));
    world.AddObject(std::make_shared<Quad>(Point3(0, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), red));
    world.AddObject(
        std::make_shared<Quad>(Point3(343, 554, 332), Vector3(-130, 0, 0), Vector3(0, 0, -105), light));
    world.AddObject(std::make_shared<Quad>(Point3(0, 0, 0), Vector3(555, 0, 0), Vector3(0, 0, 555), white));
    world.AddObject(
        std::make_shared<Quad>(Point3(555, 555, 555), Vector3(-555, 0, 0), Vector3(0, 0, -555), white));
    world.AddObject(std::make_shared<Quad>(Point3(0, 0, 555), Vector3(555, 0, 0), Vector3(0, 555, 0), white));

    // rotation set in section 8.2
    std::shared_ptr<Hittable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(std::move(box1), 15);
    box1 = std::make_shared<Translate>(std::move(box1), Vector3(265, 0, 295));
    world.AddObject(box1);

    std::shared_ptr<Hittable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(std::move(box2), -18);
    box2 = std::make_shared<Translate>(std::move(box2), Vector3(130, 0, 65));
    world.AddObject(box2);

    Camera camera = ConfigureCameraForRender(args::kCornellBox, image_width);
    camera.Render(world);
}
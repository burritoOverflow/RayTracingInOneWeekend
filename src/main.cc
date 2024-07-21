#include <map>
#include <string>

#include "args.h"
#include "scenes.h"

static void RunRender(const args::Args& args) {
    if (args.render_option_->empty()) {
        args::ShowHelp();
    }

    // validate provided render arg
    const std::string& render_opt_str = args.render_option_.value();
    const auto result = args::RENDER_ARG_OPTION_MAP.find(render_opt_str);
    if (result == args::RENDER_ARG_OPTION_MAP.end()) {
        args::ShowHelp();
    }

    switch (result->second) {
        case args::kCheckeredSpheres:
            RayTracingScenes::RenderCheckeredSpheresWorld(args.image_width_);
            break;
        case args::kBouncingSpheres:
            RayTracingScenes::RenderBouncingSpheresWorld(args.image_width_);
            break;
        case args::kEarthTexture:
            RayTracingScenes::RenderEarthTextureWorld(args.image_width_);
            break;
        case args::kPerlinSpheres:
            RayTracingScenes::RenderPerlinSpheres(args.image_width_);
            break;
        case args::kQuads:
            RayTracingScenes::RenderQuads(args.image_width_);
            break;
        case args::kSimpleLight:
            RayTracingScenes::RenderSimpleLight(args.image_width_);
            break;
        case args::kCornellBox:
            RayTracingScenes::RenderCornellBox(args.image_width_);
            break;
    }
}

int main(int argc, char** argv) {
    RunRender(args::ParseArgs(argc, argv));
    return 0;
}

#ifndef ARGS_H
#define ARGS_H

#include <getopt.h>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>

// contains the args provided via the longopts.
namespace args {
struct Args {
    std::optional<std::string> render_option_;  // string corresponding to the rendered scene
    std::optional<u_int16_t> image_width_;      // desired width of the image
};

inline static void ShowHelp() {
    std::cerr << "--render <arg>        The target to render {checkered-spheres, bouncing-spheres, "
                 "earth-texture, perlin-spheres, quads, simple-light, cornell-box}"
              << '\n';
    std::cerr << "--image-width <arg>        The width of the rendered scene" << '\n';
    exit(EXIT_FAILURE);
}

// choice of pre-defined rendered scene
enum RenderedSceneOption {
    kBouncingSpheres,
    kCheckeredSpheres,
    kEarthTexture,
    kPerlinSpheres,
    kQuads,
    kSimpleLight,
    kCornellBox
};

const std::map<std::string, RenderedSceneOption> RENDER_ARG_OPTION_MAP{
    {"checkered-spheres", kCheckeredSpheres},
    {"bouncing-spheres", kBouncingSpheres},
    {"earth-texture", kEarthTexture},
    {"perlin-spheres", kPerlinSpheres},
    {"quads", kQuads},
    {"simple-light", kSimpleLight},
    {"cornell-box", kCornellBox}};

Args ParseArgs(int argc, char** argv);
}  // namespace args

#endif  // ARGS_H
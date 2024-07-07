#ifndef ARGS_H
#define ARGS_H

#include <getopt.h>
#include <functional>
#include <iostream>
#include <optional>
#include <string>

// contains the args provided via the longopts.
struct Args {
    std::optional<std::string> render_option_;  // string corresponding to the rendered scene
    std::optional<u_int16_t> image_width_;      // desired width of the image
};

inline static void ShowHelp() {
    std::cerr
        << "--render <arg>        The target to render {checkered-spheres, bouncing-spheres, earth-texture}"
        << '\n';
    std::cerr << "--image-width <arg>        The width of the rendered scene" << '\n';
    exit(EXIT_FAILURE);
}

Args ParseArgs(int argc, char** argv);

#endif  // ARGS_H
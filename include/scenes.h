#ifndef SCENES_H
#define SCENES_H

#include <optional>
#include "args.h"
#include "bvhnode.h"
#include "camera.h"
#include "dielectric.h"
#include "diffuse_light.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "quad.h"
#include "sphere.h"

namespace RayTracingScenes {
void RenderCheckeredSpheresWorld(const std::optional<u_int16_t>& image_width);

void RenderBouncingSpheresWorld(const std::optional<u_int16_t>& image_width);

void RenderEarthTextureWorld(const std::optional<u_int16_t>& image_width);

void RenderPerlinSpheres(const std::optional<u_int16_t>& image_width);

void RenderQuads(const std::optional<u_int16_t>& image_width);

void RenderSimpleLight(const std::optional<u_int16_t>& image_width);

void RenderCornellBox(const std::optional<u_int16_t>& image_width);
}  // namespace RayTracingScenes

#endif
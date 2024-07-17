#ifndef DIFFUSE_LIGHT_H
#define DIFFUSE_LIGHT_H

#include "material.h"
#include "texture.h"

class DiffuseLight : public Material {
   public:
    DiffuseLight(std::shared_ptr<Texture>& texture) : texture_(texture){};

    DiffuseLight(const color::Color& emit) : texture_(std::make_shared<SolidColor>(emit)){};

    color::Color Emitted(const double u, const double v, const Point3& point) const override {
        return this->texture_->Value(u, v, point);
    }

   private:
    std::shared_ptr<Texture> texture_;
};

#endif  // DIFFUSE_LIGHT_H

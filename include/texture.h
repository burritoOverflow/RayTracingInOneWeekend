#ifndef TEXTURE_H
#define TEXTURE_H

#include <cmath>
#include <memory>
#include "color.h"
#include "image_utils.h"
#include "perlin.h"
#include "vec3.h"

class Texture {
   public:
    virtual ~Texture() = default;

    // return the texture color given the input coordinates
    virtual color::Color Value(double u, double v, const Point3& point) const = 0;
};

class SolidColor : public Texture {
   public:
    SolidColor(const color::Color& albedo) : albedo_(albedo){};

    SolidColor(const double red, const double green, const double blue)
        : SolidColor(color::Color(red, green, blue)){};

    color::Color Value(double u, double v, const Point3& point) const override { return this->albedo_; }

   private:
    color::Color albedo_;
};

class CheckerTexture : public Texture {
   public:
    CheckerTexture(const double scale,
                   const std::shared_ptr<Texture>& odd,
                   const std::shared_ptr<Texture>& even)
        : inv_scale_(1.0 / scale), odd_(odd), even_(even){};

    CheckerTexture(const double scale, const color::Color& color1, const color::Color& color2)
        : inv_scale_(1.0 / scale),
          odd_(std::make_shared<SolidColor>(color2)),
          even_(std::make_shared<SolidColor>(color1)) {}

    color::Color Value(double u, double v, const Point3& point) const override;

   private:
    double inv_scale_;
    std::shared_ptr<Texture> odd_;
    std::shared_ptr<Texture> even_;
};

class ImageTexture : public Texture {
   public:
    explicit ImageTexture(const char* filename) : image_(filename){};

    color::Color Value(double u, double v, const Point3& point) const override;

   private:
    Image image_;
};

class NoiseTexture : public Texture {
   public:
    NoiseTexture(const double scale) : scale_(scale){};

    color::Color Value(double u, double v, const Point3& point) const override;

   private:
    Perlin noise_;
    const double scale_;
};

#endif

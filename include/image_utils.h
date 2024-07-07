#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <iostream>
#include <string>

class Image {
   public:
    Image()
        : fdata_(nullptr),
          bdata_(nullptr),
          image_width_(0),
          image_height_(0),
          bytes_per_scanline_(0),
          image_dir_(DEFAULT_IMAGE_DIR){};

    explicit Image(const char* image_filename)
        : fdata_(nullptr),
          bdata_(nullptr),
          image_width_(0),
          image_height_(0),
          bytes_per_scanline_(0),
          image_dir_(DEFAULT_IMAGE_DIR) {
        const auto filename = std::string{image_filename};
        const auto filepath = this->image_dir_ + '/' + image_filename;
        const bool result = this->LoadImageData(filepath);
        if (!result) {
            std::cerr << "Unable to load file at path: " << filepath << '\n';
        }
    }

    // impl for the dtor in source file to work with STB single inclusion.
    ~Image();

    int Width() const { return this->fdata_ == nullptr ? 0 : this->image_width_; }
    int Height() const { return this->fdata_ == nullptr ? 0 : this->image_height_; }

    // get the 8-bit RGB values for each pixel
    const unsigned char* PixelData(int x, int y) const;

   private:
    const int bytes_per_pixel_{3};
    float* fdata_;          // Linear floating point pixel data
    unsigned char* bdata_;  // Linear 8-bit pixel data
    int image_width_;       // Loaded image width
    int image_height_;      // Loaded image height
    int bytes_per_scanline_ = 0;

    const std::string image_dir_;  // default directory for image textures

    // Loads the linear (gamma=1) image data from the given file name
    bool LoadImageData(const std::string& filepath);

    static inline int Clamp(const int x, const int min, const int max) {
        if (x < min) {
            return min;
        }
        if (x < max) {
            return x;
        }
        return max - 1;
    }

    static inline unsigned char FloatToByte(const float value) {
        if (value <= 0.0f) {
            return 0;
        }
        if (1.0f <= value) {
            return 255;
        }
        return static_cast<unsigned char>(256.0f * value);
    }

    // Convert the linear floating point pixel data to bytes, storing the resulting byte
    // data in the `bdata` member.
    void ConvertToBytes();
};

#endif  // IMAGE_UTILS_H

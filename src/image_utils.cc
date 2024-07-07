#include "image_utils.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

Image::~Image() {
    delete[] this->bdata_;
    STBI_FREE(this->fdata_);
}

bool Image::LoadImageData(const std::string& filepath) {
    // dummy out param
    auto n = this->bytes_per_pixel_;
    this->fdata_ =
        stbi_loadf(filepath.c_str(), &this->image_width_, &this->image_height_, &n, this->bytes_per_pixel_);
    if (this->fdata_ == nullptr) {
        return false;
    }

    this->bytes_per_scanline_ = this->image_width_ * this->bytes_per_pixel_;
    this->ConvertToBytes();
    return true;
}

void Image::ConvertToBytes() {
    const int total_bytes = this->image_width_ * this->image_height_ * this->bytes_per_pixel_;
    this->bdata_ = new unsigned char[total_bytes];

    // convert all values from [0.0, 1.0] floats to unsigned [0, 255] values.
    auto* bytes_ptr = this->bdata_;
    auto* float_pointer = this->fdata_;

    for (int i = 0; i < total_bytes; i++, float_pointer++, bytes_ptr++) {
        *bytes_ptr = Image::FloatToByte(*float_pointer);
    }
}

// Return the address of the three RGB bytes of the pixel at x,y. If there is no image
// data, returns magenta.
const unsigned char* Image::PixelData(int x, int y) const {
    static const unsigned char MAGENTA[] = {255, 0, 255};
    if (this->bdata_ == nullptr) {
        return MAGENTA;
    }

    x = Image::Clamp(x, 0, this->image_width_);
    y = Image::Clamp(y, 0, this->image_height_);

    return this->bdata_ + y * this->bytes_per_scanline_ + x * this->bytes_per_pixel_;
}
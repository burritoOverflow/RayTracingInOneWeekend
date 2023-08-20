#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>

const int IMAGE_WIDTH = 1024;
const int IMAGE_HEIGHT = 768;
const std::string DIRNAME = "images";

void CreateImageOutdir() {
  if (!std::filesystem::exists(DIRNAME)) {
    std::filesystem::create_directory(DIRNAME);
  }
}

std::string GetCurrentDateStr() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%m-%d-%Y_%H-%M-%S");
    return oss.str();
}

int main() {
  CreateImageOutdir();
  
  const auto image_out_filename = GetCurrentDateStr() + "image.ppm";
  auto image_path = std::filesystem::path(DIRNAME) / image_out_filename;

  std::ofstream out_stream(image_path.string().c_str());

  out_stream << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

  for (int j = 0; j < IMAGE_HEIGHT; ++j) {
    for (int i = 0; i < IMAGE_WIDTH; ++i) {
      auto r = double(i) / (IMAGE_WIDTH - 1);
      auto g = double(j) / (IMAGE_HEIGHT - 1);
      auto b = 0;

      int ir = static_cast<int>(255.999 * r);
      int ig = static_cast<int>(255.999 * g);
      int ib = static_cast<int>(255.999 * b);

      out_stream << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }
}

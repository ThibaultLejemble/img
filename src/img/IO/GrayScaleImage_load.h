#pragma once

#include <string>

namespace img {

class GrayScaleImage;

bool load(const std::string& filename, GrayScaleImage& gray, bool flip = false);

} // namespace img

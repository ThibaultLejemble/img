#pragma once

#include <string>

namespace img {

class GrayScaleImage;

bool save(const std::string& filename, const GrayScaleImage& gray, bool flip = false);

} // namespace img

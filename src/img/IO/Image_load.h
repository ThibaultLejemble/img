#pragma once

#include <string>

namespace img {

class Image;

bool load(const std::string& filename, Image& rgb, bool flip = false);

} // namespace img

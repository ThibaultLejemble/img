#pragma once

#include <string>

namespace img {

class Image;

bool save(const std::string& filename, const Image& rgb, bool flip = false);

} // namespace img

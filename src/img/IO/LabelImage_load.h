#pragma once

#include <string>

namespace img {

class LabelImage;

bool load(const std::string& filename, LabelImage& labels);

} // namespace img

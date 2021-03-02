#pragma once

#include <string>

namespace img {

class LabelImage;

bool save(const std::string& filename, const LabelImage& labels);

} // namespace img

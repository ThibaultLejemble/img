#pragma once

#include <img/IO/STB.h>

#include <fstream>
#include <cmath>
#include <vector>

namespace img {
namespace io {

template<class ImageT>
inline bool load(const std::string& filename, ImageT& image, bool flip = false);

template<class ImageT>
inline bool save(const std::string& filename, const ImageT& image, bool flip = false);

} // namespace io
} // namespace img

// =============================================================================

namespace img {
namespace io {

template<class ImageT>
bool load(const std::string& filename, ImageT& image, bool flip)
{
    std::abort(); //TODO

    return true;
}

namespace internal {
template<typename T> char cast(T val);
template<> inline char cast(int val)   {return char(val);}
template<> inline char cast(float val) {return char(int(std::round(255.f * val)));}
template<> inline char cast(double val){return char(int(std::round(255.  * val)));}
} // namespace internal

template<class ImageT>
bool save(const std::string& filename, const ImageT& image, bool flip)
{
    std::vector<char> data(image.capacity());
    for(int i = 0; i < image.capacity(); ++i)
    {
        data[i] = internal::cast(image.data()[i]);
    }

    stbi_flip_vertically_on_write(flip);
    const auto ok = stbi_write_png(filename.c_str(),
                                   image.width(),
                                   image.height(),
                                   image.depth(),
                                   data.data(), 0);

    return ok;
}

} // namespace io
} // namespace img



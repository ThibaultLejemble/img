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

namespace internal {

template<typename T> char cast(T val);
template<> inline char cast(int val)    {return char(val);}
template<> inline char cast(float val)  {return char(int(std::round(255.f * val)));}
template<> inline char cast(double val) {return char(int(std::round(255.  * val)));}

template<typename T> T cast(char val);
template<> inline int    cast(char val) {return int(val);}
template<> inline float  cast(char val) {return float(int(val))  * 255.f;}
template<> inline double cast(char val) {return double(int(val)) * 255. ;}

template<typename T, int C> struct Caster {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to);
};

template<> struct Caster<int,1> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<int,2> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<int,3> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<int,4> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};

template<> struct Caster<float,1> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<float,2> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<float,3> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<float,4> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};

template<> struct Caster<double,1> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<double,2> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<double,3> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};
template<> struct Caster<double,4> {
    template<class Map> inline static void cast(const std::vector<unsigned char>& from,
                                                Map to)
    {
        std::abort(); //TODO
    }
};

} // namespace internal

template<class ImageT>
bool load(const std::string& filename, ImageT& image, bool flip)
{
    using          T = typename ImageT::Type;
    constexpr auto C = ImageT::depth();

    image.clear();

    int width   = 0;
    int height  = 0;
    int channel = 0;
    constexpr auto desired_channels = 0;
    stbi_set_flip_vertically_on_load(flip);
    auto data = stbi_load(filename.c_str(),
                          &width,
                          &height,
                          &channel,
                          desired_channels);

    if(data == nullptr) return false;

    image.resize(height, width);

    for(int k = 0; k < width * height; ++k)
    {
        std::vector<unsigned char> pixel(channel);
        for(int c = 0; c < channel; ++c)
            pixel[c] = data[channel * k + c];

        internal::Caster<T,C>::cast(pixel, image(k));
    }

    return true;
}


template<class ImageT>
bool save(const std::string& filename, const ImageT& image, bool flip)
{
    std::vector<char> data(image.capacity());
    for(int k = 0; k < image.capacity(); ++k)
    {
        data[k] = internal::cast(image.data()[k]);
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



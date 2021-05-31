#include <img/IO/GrayScaleImage_load.h>
#include <img/IO/STB.h>
#include <img/Image/GrayScaleImage.h>

namespace img {

bool load(const std::string& filename, GrayScaleImage& gray, bool flip)
{
    gray.clear();

    int width   = 0;
    int height  = 0;
    int channel = 0;
    stb::stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stb::stbi_load(filename.c_str(), &width, &height, &channel, 1);

    if(data == nullptr) return false;

    gray.resize(height, width);

    for(int k=0; k<gray.capacity(); ++k)
    {
        gray.data()[k] = float(data[k]) / 255;
    }

    free(data);

    return true;
}

} // namespace img

#include <img/IO/Image_load.h>
#include <img/IO/STB.h>
#include <img/Image/Image.h>

namespace img {

bool load(const std::string& filename, Image& rgb, bool flip)
{
    rgb.clear();

    int width   = 0;
    int height  = 0;
    int channel = 0;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channel, 4);

    if(data == nullptr) return false;

    rgb.resize(height, width);

    for(int k=0; k<rgb.capacity(); ++k)
    {
        rgb.data()[k] = float(data[k]) / 255;
    }

    free(data);

    return true;
}

} // namespace img

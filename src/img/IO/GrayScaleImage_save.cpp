#include <img/IO/GrayScaleImage_save.h>
#include <img/IO/STB.h>
#include <img/Image/GrayScaleImage.h>

namespace img {

bool save(const std::string& filename, const GrayScaleImage& gray, bool flip)
{
    char* data = (char*)malloc(gray.capacity() * sizeof(char));

    for(int k=0; k<gray.capacity(); ++k)
    {
        data[k] = 255 * gray.data()[k];
    }

    stbi_flip_vertically_on_write(flip);
    int ok = stbi_write_png(filename.c_str(), gray.width(), gray.height(), 1, data, 0);

    free(data);

    return ok;
}

} // namespace img

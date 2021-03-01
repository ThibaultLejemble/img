#include <img/IO/Image_save.h>
#include <img/IO/STB.h>
#include <img/Image/Image.h>

namespace img {

bool save(const std::string& filename, const Image& rgb, bool flip)
{
    char* data = (char*)malloc(rgb.capacity() * sizeof(char));

    for(int k=0; k<rgb.capacity(); ++k)
    {
        data[k] = 255 * rgb.data()[k];
    }

    stbi_flip_vertically_on_write(flip);
    int ok = stbi_write_png(filename.c_str(), rgb.width(), rgb.height(), 4, data, 0);

    free(data);

    return ok;
}

} // namespace img

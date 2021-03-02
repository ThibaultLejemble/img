#include <img/Image/Image.h>
#include <img/Image/BinaryImage.h>
#include <img/Operations/Convert.h>
#include <img/IO/ImageIO.h>

#include <iostream>

using namespace img;

int main()
{
    Image rgb;
    bool ok = load("example1_fractal.png", rgb);
    if(!ok)
    {
        std::cout << "Failed to load image 'example1_fractal.png'" << std::endl;
        return 1;
    }

    BinaryImage binary;

    // RGB to binary
    convert(rgb, binary);

    // binary to RGB (black and white)
    convert(binary, rgb);

    ok = save("example2_binary.png", rgb);

    return !ok;
}

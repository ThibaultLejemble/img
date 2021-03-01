#include <img/Image/Image.h>
#include <img/Image/BinaryImage.h>

#include <iostream>

using namespace img;

int main()
{
    Image image;
    bool ok = image.load("example1_fractal.png");
    if(!ok)
    {
        std::cout << "Failed to load image 'example1_fractal.png'" << std::endl;
        return 1;
    }

    // RGB to binary
    BinaryImage binary = image;

    // binary to RGB (black and white)
    image = binary;

    ok = image.save("example2_binary.png");
    return !ok;
}

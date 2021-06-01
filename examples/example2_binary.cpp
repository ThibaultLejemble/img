#include <img/Image.h>

#include <iostream>

using namespace img;

int main()
{
    ImageRGBf rgb;
    const auto ok = load("example1_fractal.png", rgb);
    if(!ok)
    {
        std::cout << "Failed to load image 'example1_fractal.png'" << std::endl;
        return 1;
    }

    // rgb to gray scale
    const auto gray = ImageGf(rgb);

    // binarize
    ImageGf binary;
    cast(gray, binary, [](auto c)
    {
        return ImageGf::Color(c[0] > 0.5);
    });

    save("example2_gray.png",   gray);
    save("example2_binary.png", binary);

    return 0;
}

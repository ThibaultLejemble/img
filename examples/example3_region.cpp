#include <img/Image.h>
#include <img/RegionGrowing.h>

#include <iostream>

using namespace img;

int main()
{
    ImageGf gray;
    const auto ok = load("example1_fractal.png", gray);
    if(!ok)
    {
        std::cout << "Failed to load image 'example1_fractal.png'" << std::endl;
        return 1;
    }

    ImageGi labels;
    auto n = region_growing(gray, labels, [&gray](int i,int j, int k,int l) -> bool
    {
        const auto diff = std::abs(gray(i,j) - gray(k,l));
        return diff < 0.01;
    });

    std::cout << n << " regions created" << std::endl;

    cast(labels, gray, [](auto label)
    {
         return ImageGf::Color(label % 2);
    });

    save("example3_regions.png", gray);

    return 0;
}

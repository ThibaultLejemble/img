#include <img/Operations/Convert.h>
#include <img/Image/Image.h>
#include <img/Image/BinaryImage.h>
#include <img/Image/GrayScaleImage.h>

namespace img {

void convert(const Image& rgb, BinaryImage& binary)
{
    binary.resize(rgb.height(), rgb.width());

    for(int k=0; k<rgb.size(); ++k)
    {
        binary(k) = (rgb(k).head<3>().sum() / 3 > 0.5);
    }
}

void convert(const Image& rgb, GrayScaleImage& gray)
{
    gray.resize(rgb.height(), rgb.width());

    for(int k=0; k<rgb.size(); ++k)
    {
        gray(k) = rgb(k).head<3>().sum() / 3;
    }
}

void convert(const BinaryImage& binary, Image& rgb)
{
    rgb.resize(binary.height(), binary.width());

    for(int k=0; k<binary.size(); ++k)
    {
        rgb(k) = binary(k) ? Image::Color(1,1,1,1) : Image::Color(0,0,0,1);
    }
}

void convert(const BinaryImage& binary, GrayScaleImage& gray)
{
    gray.resize(binary.height(), binary.width());

    for(int k=0; k<binary.size(); ++k)
    {
        gray(k) = binary(k) ? 1 : 0;
    }
}

void convert(const GrayScaleImage& gray, Image& rgb)
{
    rgb.resize(gray.height(), gray.width());

    for(int k=0; k<gray.size(); ++k)
    {
        rgb(k) = Image::Color(gray(k), gray(k), gray(k), 1);
    }
}

void convert(const GrayScaleImage& gray, BinaryImage& binary)
{
    binary.resize(gray.height(), gray.width());

    for(int k=0; k<gray.size(); ++k)
    {
        binary(k) = gray(k) > 0.5 ? 1 : 0;
    }
}

} // namespace img

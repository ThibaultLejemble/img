#include <img/Operations/Convert.h>
#include <img/Image/Image.h>
#include <img/Image/BinaryImage.h>
#include <img/Image/GrayScaleImage.h>
#include <img/Image/LabelImage.h>

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

void convert(const Image& rgb, LabelImage& labels)
{
    labels.resize(rgb.height(), rgb.width());

    for(int k=0; k<rgb.size(); ++k)
    {
        labels(k) = (rgb(k).head<3>().sum() / 3 > 0.5);
    }
}

// -----------------------------------------------------------------------------

void convert(const BinaryImage& binary, Image& rgb)
{
    rgb.resize(binary.height(), binary.width());

    for(int k=0; k<binary.size(); ++k)
    {
        rgb(k) = Image::Color(binary(k),binary(k),binary(k),1);
    }
}

void convert(const BinaryImage& binary, GrayScaleImage& gray)
{
    gray.resize(binary.height(), binary.width());

    for(int k=0; k<binary.size(); ++k)
    {
        gray(k) = binary(k);
    }
}

void convert(const BinaryImage& binary, LabelImage& labels)
{
    labels.resize(binary.height(), binary.width());

    for(int k=0; k<binary.size(); ++k)
    {
        labels(k) = binary(k);
    }
}

// -----------------------------------------------------------------------------

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
        binary(k) = gray(k) > 0.5;
    }
}

void convert(const GrayScaleImage& gray, LabelImage& labels)
{
    labels.resize(gray.height(), gray.width());

    for(int k=0; k<gray.size(); ++k)
    {
        labels(k) = gray(k) > 0.5;
    }
}

// -----------------------------------------------------------------------------

void convert(const LabelImage& labels, Image& rgb)
{
    rgb.resize(labels.height(), labels.width());

    int label_min = std::numeric_limits<int>::max();
    int label_max = std::numeric_limits<int>::min();
    for(int k=0; k<labels.size(); ++k)
    {
        label_min = std::min(label_min, labels(k));
        label_max = std::max(label_max, labels(k));
    }

    for(int k=0; k<labels.size(); ++k)
    {
        const float t = float(labels(k)-label_min)/(label_max-label_min);
        rgb(k) = Image::Color(t,t,t,1);
    }
}

void convert(const LabelImage& labels, BinaryImage& binary)
{
    binary.resize(labels.height(), labels.width());

    for(int k=0; k<labels.size(); ++k)
    {
        binary(k) = labels(k) >= 0;
    }
}

void convert(const LabelImage& labels, GrayScaleImage& gray)
{
    gray.resize(labels.height(), labels.width());

    int label_min = std::numeric_limits<int>::max();
    int label_max = std::numeric_limits<int>::min();
    for(int k=0; k<labels.size(); ++k)
    {
        label_min = std::min(label_min, labels(k));
        label_max = std::max(label_max, labels(k));
    }

    for(int k=0; k<labels.size(); ++k)
    {
        gray(k) = float(labels(k)-label_min)/(label_max-label_min);
    }
}

} // namespace img

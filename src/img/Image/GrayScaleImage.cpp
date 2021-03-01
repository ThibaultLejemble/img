#include <img/Image/GrayScaleImage.h>
#include <img/Image/Image.h>
#include <img/Image/BinaryImage.h>

namespace img {

// GrayScaleImage --------------------------------------------------------------

GrayScaleImage::GrayScaleImage() :
    m_height(0),
    m_width(0),
    m_data(nullptr)
{
}

GrayScaleImage::GrayScaleImage(int height, int width) : GrayScaleImage()
{
    this->resize(height, width);
}

GrayScaleImage::GrayScaleImage(const GrayScaleImage& other) : GrayScaleImage()
{
    this->resize(other.height(), other.width());
    std::copy(other.data(), other.data() + other.capacity(), data());
}

GrayScaleImage::GrayScaleImage(GrayScaleImage&& other) : GrayScaleImage()
{
    m_height = other.m_height;
    m_width  = other.m_width;
    m_data   = other.m_data;
    other.m_height = 0;
    other.m_width  = 0;
    other.m_data   = nullptr;
}

GrayScaleImage& GrayScaleImage::operator = (const GrayScaleImage& other)
{
    this->resize(other.height(), other.width());
    std::copy(other.data(), other.data() + other.capacity(), data());
    return *this;
}

GrayScaleImage& GrayScaleImage::operator = (GrayScaleImage&& other)
{
    this->clear();
    m_height = other.m_height;
    m_width  = other.m_width;
    m_data   = other.m_data;
    other.m_height = 0;
    other.m_width  = 0;
    other.m_data   = nullptr;
    return *this;
}

GrayScaleImage::~GrayScaleImage()
{
    this->clear();
}

// Accessors -------------------------------------------------------------------

const float* GrayScaleImage::data() const
{
    return m_data;
}

float* GrayScaleImage::data()
{
    return m_data;
}

GrayScaleImage::ConstColorAccess GrayScaleImage::eval(float u, float v) const
{
    const int i = std::floor(u * (m_height-1));
    const int j = std::floor(v * (m_width-1));
    return this->operator()(i,j);
}

// Modifiers -------------------------------------------------------------------

void GrayScaleImage::clear()
{
    free(m_data);
    m_height = 0;
    m_width  = 0;
    m_data   = nullptr;
}

void GrayScaleImage::resize(int height, int width)
{
    if(empty())
    {
        m_data = (float*)malloc(4 * width * height * sizeof(float));
    }
    else
    {
        m_data = (float*)realloc(m_data, 4 * width * height * sizeof(float));
    }
    m_height = height;
    m_width  = width;
    fill(0);
}

void GrayScaleImage::fill(float value)
{
    for(int i=0; i<height(); ++i)
    {
        for(int j=0; j<width(); ++j)
        {
            this->operator()(i,j) = value;
        }
    }
}

} // namespace img

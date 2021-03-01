#include <img/Image/Image.h>
#include <img/Image/BinaryImage.h>
#include <img/Image/GrayScaleImage.h>

namespace img {

// Image -----------------------------------------------------------------------

Image::Image() :
    m_height(0),
    m_width(0),
    m_data(nullptr)
{
}

Image::Image(int height, int width) : Image()
{
    this->resize(height, width);
}

Image::Image(const Image& other) : Image()
{
    this->resize(other.height(), other.width());
    std::copy(other.data(), other.data() + other.capacity(), data());
}

Image::Image(Image&& other) : Image()
{
    m_height = other.m_height;
    m_width  = other.m_width;
    m_data   = other.m_data;
    other.m_height = 0;
    other.m_width  = 0;
    other.m_data   = nullptr;
}

Image& Image::operator = (const Image& other)
{
    this->resize(other.height(), other.width());
    std::copy(other.data(), other.data() + other.capacity(), data());
    return *this;
}

Image& Image::operator = (Image&& other)
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

Image::~Image()
{
    this->clear();
}

// Accessors -------------------------------------------------------------------

const float* Image::data() const
{
    return m_data;
}

float* Image::data()
{
    return m_data;
}

Image::ConstColorAccess Image::eval(float u, float v) const
{
    const int i = std::floor(u * (m_height-1));
    const int j = std::floor(v * (m_width-1));
    return this->operator()(i,j);
}

// Modifiers -------------------------------------------------------------------

void Image::clear()
{
    free(m_data);
    m_height = 0;
    m_width  = 0;
    m_data   = nullptr;
}

void Image::resize(int height, int width)
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
    fill(0,0);
}

void Image::set_alpha(float alpha)
{
    for(int i=0; i<height(); ++i)
    {
        for(int j=0; j<width(); ++j)
        {
            this->operator()(i,j)[3] = alpha;
        }
    }
}

void Image::fill(float r, float g, float b, float a)
{
    for(int i=0; i<height(); ++i)
    {
        for(int j=0; j<width(); ++j)
        {
            this->operator()(i,j)[0] = r;
            this->operator()(i,j)[1] = g;
            this->operator()(i,j)[2] = b;
            this->operator()(i,j)[3] = a;
        }
    }
}

void Image::fill(float gray, float a)
{
    this->fill(gray, gray, gray, a);
}

} // namespace img

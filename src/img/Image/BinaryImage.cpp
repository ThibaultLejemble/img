#include <img/Image/BinaryImage.h>

namespace img {

// BinaryImage -----------------------------------------------------------------

BinaryImage::BinaryImage() :
    m_height(0),
    m_width(0),
    m_data(0)
{
}

BinaryImage::BinaryImage(int height, int width) :
    m_height(height),
    m_width(width),
    m_data(height * width)
{
}

BinaryImage::BinaryImage(const BinaryImage& other) :
    m_height(other.m_height),
    m_width(other.m_width),
    m_data(other.m_data)
{
}

BinaryImage::BinaryImage(BinaryImage&& other) :
    m_height(other.m_height),
    m_width(other.m_width),
    m_data(other.m_data)
{
}

BinaryImage& BinaryImage::operator = (const BinaryImage& other)
{
    m_height = other.m_height;
    m_width  = other.m_width;
    m_data   = other.m_data;
    return *this;
}

BinaryImage& BinaryImage::operator = (BinaryImage&& other)
{
    m_height = other.m_height;
    m_width  = other.m_width;
    m_data   = std::move(other.m_data);
    return *this;
}

BinaryImage::~BinaryImage()
{
    this->clear();
}

// Modifiers -------------------------------------------------------------------

void BinaryImage::clear()
{
    m_height = 0;
    m_width  = 0;
    m_data.clear();
}

void BinaryImage::resize(int height, int width)
{
    m_height = height;
    m_width  = width;
    m_data.resize(height * width);
    fill(false);
}

void BinaryImage::fill(bool b)
{
    std::fill(m_data.begin(), m_data.end(), b);
}

} // namespace img

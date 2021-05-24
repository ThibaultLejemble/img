#include <img/Image/LabelImage.h>

namespace img {

// LabelImage ------------------------------------------------------------------

LabelImage::LabelImage() :
    m_height(0),
    m_width(0),
    m_data(0)
{
}

LabelImage::LabelImage(int height, int width) :
    m_height(height),
    m_width(width),
    m_data(height * width)
{
}

LabelImage::LabelImage(const LabelImage& other) :
    m_height(other.m_height),
    m_width(other.m_width),
    m_data(other.m_data)
{
}

LabelImage::LabelImage(LabelImage&& other) :
    m_height(other.m_height),
    m_width(other.m_width),
    m_data(other.m_data)
{
}

LabelImage& LabelImage::operator = (const LabelImage& other)
{
    m_height = other.m_height;
    m_width  = other.m_width;
    m_data   = other.m_data;
    return *this;
}

LabelImage& LabelImage::operator = (LabelImage&& other)
{
    m_height = other.m_height;
    m_width  = other.m_width;
    m_data   = std::move(other.m_data);
    return *this;
}

LabelImage::~LabelImage()
{
    this->clear();
}

// Modifiers -------------------------------------------------------------------

void LabelImage::clear()
{
    m_height = 0;
    m_width  = 0;
    m_data.clear();
}

void LabelImage::resize(int height, int width)
{
    m_height = height;
    m_width  = width;
    m_data.resize(height * width);
    fill(false);
}

void LabelImage::fill(int value)
{
    std::fill(m_data.begin(), m_data.end(), value);
}

} // namespace img

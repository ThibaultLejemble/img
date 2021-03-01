#include <img/BinaryImage.h>
#include <img/Image.h>

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

BinaryImage::BinaryImage(const Image& other) :
    m_height(other.height()),
    m_width(other.width()),
    m_data(other.height() * other.width())
{
    for(int k=0; k<capacity(); ++k)
    {
        m_data[k] = (other(k).head<3>().sum() / 3 > 0.5);
    }
}

BinaryImage& BinaryImage::operator = (const Image& other)
{
    m_height = other.height();
    m_width  = other.width();
    m_data.resize(m_height * m_width);
    for(int k=0; k<int(m_data.size()); ++k)
    {
        m_data[k] = (other(k).head<3>().sum() / 3 > 0.5);
    }
    return *this;
}

BinaryImage::~BinaryImage()
{
    this->clear();
}

// IO --------------------------------------------------------------------------

bool BinaryImage::load(const std::string& filename, bool flip)
{
    clear();

    Image rgb;
    const bool ok = rgb.load(filename, flip);
    if(!ok) return false;

    *this = rgb;

    return true;
}

bool BinaryImage::save(const std::string& filename, bool flip) const
{
    Image rgb = *this;
    return rgb.save(filename, flip);
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

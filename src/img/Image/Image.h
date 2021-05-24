#pragma once

#include <Eigen/Core>

#include <string>
#include <assert.h>

namespace img {

//!
//! \brief The Image class represents a 2D RGBA single floating point image.
//!
//! Image can be loaded from and saved to png files.
//! The top-left pixel is at coordinate (0,0).
//! The storage is in row-major order.
//! Pixel access is made through an Eigen::Map of Eigen::Vector4f.
//! Resizing operations are not conservative.
//!
class Image
{
    // Types -------------------------------------------------------------------
public:
    using Color            = Eigen::Vector4f;
    using ColorAccess      = Eigen::Map<Color>;
    using ConstColorAccess = Eigen::Map<const Color>;

    // Image -------------------------------------------------------------------
public:
    inline Image();
    inline Image(int height, int width);
    inline Image(const Image& other);
    inline Image(Image&& other);

    inline Image& operator = (const Image& other);
    inline Image& operator = (Image&& other);

    inline ~Image();

    // Capacity ----------------------------------------------------------------
public:
    inline bool empty();
    inline int height() const;
    inline int width() const;
    inline int size() const;
    inline int capacity() const;

    // Accessors ---------------------------------------------------------------
public:
    inline ConstColorAccess operator() (int i, int j) const;
    inline ColorAccess      operator() (int i, int j);
    inline ConstColorAccess operator() (int k) const;
    inline ColorAccess      operator() (int k);

    inline const float* data() const;
    inline       float* data();

    inline ConstColorAccess eval(float u, float v) const;

    // Modifiers ---------------------------------------------------------------
public:
    inline void clear();
    inline void resize(int height, int width);
    inline void set_alpha(float alpha);
    inline void fill(float r, float g, float b, float a = 1.f);
    inline void fill(float gray, float a = 1.f);

    // Internal ----------------------------------------------------------------
protected:
    inline const float* at(int i, int j) const;
    inline       float* at(int i, int j);

    inline const float* at(int k) const;
    inline       float* at(int k);

    inline int index(int i, int j) const;

    // Data --------------------------------------------------------------------
protected:
    int     m_height;
    int     m_width;
    float*  m_data;

}; // class Image

// =============================================================================
// =============================================================================
// =============================================================================

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

// Capacity --------------------------------------------------------------------

bool Image::empty()
{
    return size() == 0;
}

int Image::height() const
{
    return m_height;
}

int Image::width() const
{
    return m_width;
}

int Image::size() const
{
    return width() * height();
}

int Image::capacity() const
{
    return 4 * size();
}

// Accessors -------------------------------------------------------------------

Image::ConstColorAccess Image::operator()(int i, int j) const
{
    return ConstColorAccess(at(i,j));
}

Image::ColorAccess Image::operator()(int i, int j)
{
    return ColorAccess(at(i,j));
}

Image::ConstColorAccess Image::operator()(int k) const
{
    return ConstColorAccess(at(k));
}

Image::ColorAccess Image::operator()(int k)
{
    return ColorAccess(at(k));
}


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

// Internal --------------------------------------------------------------------

const float* Image::at(int i, int j) const
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return &m_data[index(i,j)];
}

float* Image::at(int i, int j)
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return &m_data[index(i,j)];
}

const float* Image::at(int k) const
{
    assert(0 <= k && k < height() * width());
    return &m_data[4 * k];
}

float* Image::at(int k)
{
    assert(0 <= k && k < height() * width());
    return &m_data[4 * k];
}

int Image::index(int i, int j) const
{
    return 4 * (i * width() + j);
}

} // namespace img

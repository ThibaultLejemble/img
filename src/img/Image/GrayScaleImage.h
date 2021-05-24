#pragma once

#include <string>
#include <assert.h>
#include <cmath>

namespace img {

//!
//! \brief The Image class represents a 2D gray-scale single floating point image.
//!
//! Image can be loaded from and saved to png files.
//! The top-left pixel is at coordinate (0,0).
//! The storage is in row-major order.
//! Resizing operations are not conservative.
//!
class GrayScaleImage
{
    // Types -------------------------------------------------------------------
public:
    using Color            = float;
    using ColorAccess      = Color&;
    using ConstColorAccess = Color;

    // GrayScale ---------------------------------------------------------------
public:
    inline GrayScaleImage();
    inline GrayScaleImage(int height, int width);
    inline GrayScaleImage(const GrayScaleImage& other);
    inline GrayScaleImage(GrayScaleImage&& other);

    inline GrayScaleImage& operator = (const GrayScaleImage& other);
    inline GrayScaleImage& operator = (GrayScaleImage&& other);

    inline ~GrayScaleImage();

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
    inline void fill(float value);

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

}; // class GrayScale

// =============================================================================
// =============================================================================
// =============================================================================

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

// Capacity --------------------------------------------------------------------

bool GrayScaleImage::empty()
{
    return size() == 0;
}

int GrayScaleImage::height() const
{
    return m_height;
}

int GrayScaleImage::width() const
{
    return m_width;
}

int GrayScaleImage::size() const
{
    return width() * height();
}

int GrayScaleImage::capacity() const
{
    return 1 * size();
}

// Accessors -------------------------------------------------------------------

GrayScaleImage::ConstColorAccess GrayScaleImage::operator()(int i, int j) const
{
    return *(at(i,j));
}

GrayScaleImage::ColorAccess GrayScaleImage::operator()(int i, int j)
{
    return *(at(i,j));
}

GrayScaleImage::ConstColorAccess GrayScaleImage::operator()(int k) const
{
    return *(at(k));
}

GrayScaleImage::ColorAccess GrayScaleImage::operator()(int k)
{
    return *(at(k));
}

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

// Internal --------------------------------------------------------------------

const float* GrayScaleImage::at(int i, int j) const
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return &m_data[index(i,j)];
}

float* GrayScaleImage::at(int i, int j)
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return &m_data[index(i,j)];
}

const float* GrayScaleImage::at(int k) const
{
    assert(0 <= k && k < height() * width());
    return &m_data[1 * k];
}

float* GrayScaleImage::at(int k)
{
    assert(0 <= k && k < height() * width());
    return &m_data[1 * k];
}

int GrayScaleImage::index(int i, int j) const
{
    return 1 * (i * width() + j);
}

} // namespace img

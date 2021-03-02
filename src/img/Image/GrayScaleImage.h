#pragma once

#include <string>
#include <assert.h>

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
    GrayScaleImage();
    GrayScaleImage(int height, int width);
    GrayScaleImage(const GrayScaleImage& other);
    GrayScaleImage(GrayScaleImage&& other);

    GrayScaleImage& operator = (const GrayScaleImage& other);
    GrayScaleImage& operator = (GrayScaleImage&& other);

    ~GrayScaleImage();

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

    const float* data() const;
          float* data();

    ConstColorAccess eval(float u, float v) const;

    // Modifiers ---------------------------------------------------------------
public:
    void clear();
    void resize(int height, int width);
    void fill(float value);

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

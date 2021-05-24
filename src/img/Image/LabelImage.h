#pragma once

#include <vector>
#include <assert.h>

namespace img {

//!
//! \brief The LabelImage class represents a 2D array of integer labels.
//!
//! The top-left pixel is at coordinate (0,0).
//! The storage is in row-major order.
//! Resizing operations are not conservative.
//!
class LabelImage
{
    // Types -------------------------------------------------------------------
public:
    using Color            = int;
    using ColorAccess      = int&;
    using ConstColorAccess = int;

    // LabelImage -------------------------------------------------------------
public:
    inline LabelImage();
    inline LabelImage(int height, int width);
    inline LabelImage(const LabelImage& other);
    inline LabelImage(LabelImage&& other);

    inline LabelImage& operator = (const LabelImage& other);
    inline LabelImage& operator = (LabelImage&& other);

    inline ~LabelImage();

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

    // Modifiers ---------------------------------------------------------------
public:
    inline void clear();
    inline void resize(int height, int width);
    inline void fill(int value);

    // Internal ----------------------------------------------------------------
protected:
    inline int index(int i, int j) const;

    // Data --------------------------------------------------------------------
protected:
    int m_height;
    int m_width;
    std::vector<int> m_data;

}; // class LabelImage

// =============================================================================
// =============================================================================
// =============================================================================

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

// Capacity --------------------------------------------------------------------

bool LabelImage::empty()
{
    return size() == 0;
}

int LabelImage::height() const
{
    return m_height;
}

int LabelImage::width() const
{
    return m_width;
}

int LabelImage::size() const
{
    return width() * height();
}

int LabelImage::capacity() const
{
    return m_data.size(); // = size()
}

// Accessors -------------------------------------------------------------------

LabelImage::ConstColorAccess LabelImage::operator()(int i, int j) const
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return m_data[index(i,j)];
}

LabelImage::ColorAccess LabelImage::operator()(int i, int j)
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return m_data[index(i,j)];
}

LabelImage::ConstColorAccess LabelImage::operator()(int k) const
{
    assert(0 <= k && k < size());
    return m_data[k];
}

LabelImage::ColorAccess LabelImage::operator()(int k)
{
    assert(0 <= k && k < size());
    return m_data[k];
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

// Internal --------------------------------------------------------------------

int LabelImage::index(int i, int j) const
{
    return (i * width() + j);
}

} // namespace img

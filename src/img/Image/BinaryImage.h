#pragma once

#include <vector>
#include <assert.h>

namespace img {

//!
//! \brief The BinaryImage class represents a 2D binary image.
//!
//! Values are boolean with the following general rule:
//!   True  = 1 = white = foreground
//!   False = 0 = black = background
//!
//! The top-left pixel is at coordinate (0,0).
//! The storage is in row-major order.
//! Pixels values are stored in a std::vector<bool>.
//! Non-const pixel access is made through a std::vector<bool>::const_reference.
//! Resizing operations are not conservative.
//!
class BinaryImage
{
    // Types -------------------------------------------------------------------
public:
    using Color            = bool;
    using ColorAccess      = std::vector<bool>::reference;
    using ConstColorAccess = std::vector<bool>::const_reference; // = bool

    // BinaryImage -------------------------------------------------------------
public:
    inline BinaryImage();
    inline BinaryImage(int height, int width);
    inline BinaryImage(const BinaryImage& other);
    inline BinaryImage(BinaryImage&& other);

    inline BinaryImage& operator = (const BinaryImage& other);
    inline BinaryImage& operator = (BinaryImage&& other);

    inline ~BinaryImage();

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
    inline void fill(bool b);

    // Internal ----------------------------------------------------------------
protected:
    inline int index(int i, int j) const;

    // Data --------------------------------------------------------------------
protected:
    int m_height;
    int m_width;
    std::vector<bool> m_data;

}; // class BinaryImage

// =============================================================================
// =============================================================================
// =============================================================================

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

// Capacity --------------------------------------------------------------------

bool BinaryImage::empty()
{
    return size() == 0;
}

int BinaryImage::height() const
{
    return m_height;
}

int BinaryImage::width() const
{
    return m_width;
}

int BinaryImage::size() const
{
    return width() * height();
}

int BinaryImage::capacity() const
{
    return m_data.size(); // = size()
}

// Accessors -------------------------------------------------------------------

BinaryImage::ConstColorAccess BinaryImage::operator()(int i, int j) const
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return m_data[index(i,j)];
}

BinaryImage::ColorAccess BinaryImage::operator()(int i, int j)
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return m_data[index(i,j)];
}

BinaryImage::ConstColorAccess BinaryImage::operator()(int k) const
{
    assert(0 <= k && k < size());
    return m_data[k];
}

BinaryImage::ColorAccess BinaryImage::operator()(int k)
{
    assert(0 <= k && k < size());
    return m_data[k];
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

// Internal --------------------------------------------------------------------

int BinaryImage::index(int i, int j) const
{
    return (i * width() + j);
}

} // namespace img

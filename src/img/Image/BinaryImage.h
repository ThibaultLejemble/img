#pragma once

#include <vector>
#include <assert.h>

namespace img {

//!
//! \brief The BinaryImage class represents a 2D binary image.
//!
//! Values are boolean with the following rules:
//!   True  = 1 = white
//!   False = 0 = black
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
    BinaryImage();
    BinaryImage(int height, int width);
    BinaryImage(const BinaryImage& other);
    BinaryImage(BinaryImage&& other);

    BinaryImage& operator = (const BinaryImage& other);
    BinaryImage& operator = (BinaryImage&& other);

    ~BinaryImage();

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
    void clear();
    void resize(int height, int width);
    void fill(bool b);

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

// Internal --------------------------------------------------------------------

int BinaryImage::index(int i, int j) const
{
    return (i * width() + j);
}

} // namespace img

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
    Image();
    Image(int height, int width);
    Image(const Image& other);
    Image(Image&& other);

    Image& operator = (const Image& other);
    Image& operator = (Image&& other);

    ~Image();

    // IO ----------------------------------------------------------------------
public:
    bool load(const std::string& filename, bool flip = false);
    bool save(const std::string& filename, bool flip = false) const;

    bool load_bin(const std::string& filename);
    bool save_bin(const std::string& filename) const;

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

    const float* data() const;
          float* data();

    ConstColorAccess eval(float u, float v) const;

    // Modifiers ---------------------------------------------------------------
public:
    void clear();
    void resize(int height, int width);
    void set_alpha(float alpha);
    void fill(float r, float g, float b, float a = 1.f);
    void fill(float gray, float a = 1.f);

    // Internal ----------------------------------------------------------------
protected:
    inline const float* at(int i, int j) const;
    inline       float* at(int i, int j);

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

int Image::index(int i, int j) const
{
    return 4 * (i * width() + j);
}

} // namespace img

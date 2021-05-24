#pragma once

#include <Eigen/Core>

#include <vector>

namespace img {

template<typename T = float, int C = 4>
class ImageT;

using ImageGi    = ImageT<int,   1>;
using ImageGf    = ImageT<float, 1>;
using ImageGd    = ImageT<double,1>;
using ImageGAi   = ImageT<int,   2>;
using ImageGAf   = ImageT<float, 2>;
using ImageGAd   = ImageT<double,2>;
using ImageRGBi  = ImageT<int,   3>;
using ImageRGBf  = ImageT<float, 3>;
using ImageRGBd  = ImageT<double,3>;
using ImageRGBAi = ImageT<int,   4>;
using ImageRGBAf = ImageT<float, 4>;
using ImageRGBAd = ImageT<double,4>;

template<typename T, int C>
class ImageT
{
    // Types -------------------------------------------------------------------
public:
    using Color            = Eigen::Matrix<T, C, 1>;
    using ColorAccess      = Eigen::Map<Color>;
    using ConstColorAccess = Eigen::Map<const Color>;

    // Image -------------------------------------------------------------------
public:
    inline ImageT();
    inline ImageT(int height, int width);

    // Capacity ----------------------------------------------------------------
public:
    inline bool empty();
    inline int height() const;
    inline int width() const;
    inline int depth() const;
    inline int size() const;
    inline int capacity() const;

    // Accessors ---------------------------------------------------------------
public:
    inline      ColorAccess operator()(int i, int j);
    inline ConstColorAccess operator()(int i, int j) const;

    inline const std::vector<T>& data() const;
    inline       std::vector<T>& data();

    inline const T* raw() const;
    inline       T* raw();

    inline ConstColorAccess eval(float u, float v) const;

    // Modifiers ---------------------------------------------------------------
public:
    inline void clear();
    inline void resize(int height, int width);
    inline void fill(const T& value);
    inline void fill(const Color& color);

    // Internal ----------------------------------------------------------------
protected:
    inline const float* at(int i, int j) const;
    inline       float* at(int i, int j);

    inline const float* at(int k) const;
    inline       float* at(int k);

    inline int index(int i, int j) const;

    // Data --------------------------------------------------------------------
protected:
    int            m_height;
    int            m_width;
    std::vector<T> m_data;
};

} // namespace img

// =============================================================================

namespace img {

// Image -----------------------------------------------------------------------

template<typename T, int C>
ImageT<T,C>::ImageT() : ImageT(0, 0)
{
}

template<typename T, int C>
ImageT<T,C>::ImageT(int height, int width) :
    m_height(height),
    m_width(width),
    m_data(C * width * height)
{
}

// Capacity --------------------------------------------------------------------

template<typename T, int C>
bool ImageT<T,C>::empty()
{
    return m_data.empty();
}

template<typename T, int C>
int ImageT<T,C>::height() const
{
    return m_height;
}

template<typename T, int C>
int ImageT<T,C>::width() const
{
    return m_width;
}

template<typename T, int C>
int ImageT<T,C>::depth() const
{
    return C;
}

template<typename T, int C>
int ImageT<T,C>::size() const
{
    return m_height * m_width;
}

template<typename T, int C>
int ImageT<T,C>::capacity() const
{
    return m_height * m_width * C;
}

// Accessors -------------------------------------------------------------------

template<typename T, int C>
typename ImageT<T,C>::ColorAccess ImageT<T,C>::operator()(int i, int j)
{
    return ColorAccess(at(i,j));
}

template<typename T, int C>
typename ImageT<T,C>::ConstColorAccess ImageT<T,C>::operator()(int i, int j) const
{
    return ConstColorAccess(at(i,j));
}

template<typename T, int C>
const std::vector<T>& ImageT<T,C>::data() const
{
    return m_data;
}

template<typename T, int C>
std::vector<T>& ImageT<T,C>::data()
{
    return m_data;
}

template<typename T, int C>
const T* ImageT<T,C>::raw() const
{
    return m_data.data();
}

template<typename T, int C>
T* ImageT<T,C>::raw()
{
    return m_data.data();
}

template<typename T, int C>
typename ImageT<T,C>::ConstColorAccess ImageT<T,C>::eval(float u, float v) const
{
    const int i = std::floor(u * (m_height-1));
    const int j = std::floor(v * (m_width-1));
    return this->operator()(i,j);
}

// Modifiers -------------------------------------------------------------------

template<typename T, int C>
void ImageT<T,C>::clear()
{
    m_height = 0;
    m_width  = 0;
    m_data.clear();
}

template<typename T, int C>
void ImageT<T,C>::resize(int height, int width)
{
    m_height = height;
    m_width  = width;
    m_data.resize(height * width * C);
}

template<typename T, int C>
void ImageT<T,C>::fill(const T& value)
{
    std::fill(m_data.begin(), m_data.end(), value);
}

template<typename T, int C>
void ImageT<T,C>::fill(const Color& color)
{
    for(int i=0; i<m_height; ++i)
        for(int j=0; j<m_width; ++j)
            this->operator()(i,j) = color;
}

// Internal --------------------------------------------------------------------

template<typename T, int C>
const float* ImageT<T,C>::at(int i, int j) const
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return &m_data[index(i,j)];
}

template<typename T, int C>
float* ImageT<T,C>::at(int i, int j)
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return &m_data[index(i,j)];
}

template<typename T, int C>
const float* ImageT<T,C>::at(int k) const
{
    assert(0 <= k && k < height() * width());
    return &m_data[C * k];
}

template<typename T, int C>
float* ImageT<T,C>::at(int k)
{
    assert(0 <= k && k < height() * width());
    return &m_data[C * k];
}

template<typename T, int C>
int ImageT<T,C>::index(int i, int j) const
{
    return C * (i * width() + j);
}

} // namespace img

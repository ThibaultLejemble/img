#pragma once

#include <Eigen/Core>

#include <vector>
#include <string>
#include <fstream>

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

// cast ------------------------------------------------------------------------

template<typename TFrom, int CFrom, typename TTo, int CTo, class Caster>
inline void cast(const ImageT<TFrom, CFrom>& from, ImageT<TTo, CTo>& to, Caster&& caster);

template<typename TFrom, int CFrom, typename TTo, int CTo>
inline void cast(const ImageT<TFrom, CFrom>& from, ImageT<TTo, CTo>& to);

// io --------------------------------------------------------------------------

template<typename T, int C>
inline bool save(const std::string& filename, const ImageT<T,C>& image);

// -----------------------------------------------------------------------------

template<typename T, int C>
class ImageT
{
    // Types -------------------------------------------------------------------
public:
    using Type             = T;
    using Color            = Eigen::Matrix<T, C, 1>;
    using ColorAccess      = Eigen::Map<Color>;
    using ConstColorAccess = Eigen::Map<const Color>;
    using Matrix           = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
    using MatrixMap        = Eigen::Map<Matrix>;
    using ConstMatrixMap   = Eigen::Map<const Matrix>;

    // Image -------------------------------------------------------------------
public:
    inline ImageT();
    inline ImageT(int height, int width);
    inline ImageT(int height, int width, unsigned char* data);
    inline ImageT(int height, int width, unsigned char* data, int depth);
    inline ImageT(const ImageT&) = default;
    inline ImageT(ImageT&&) = default;

    inline ImageT& operator=(const ImageT&) = default;
    inline ImageT& operator=(ImageT&&) = default;

    template<typename T2, int C2>
    inline explicit ImageT(const ImageT<T2,C2>& other);

    template<typename T2, int C2>
    inline ImageT& operator=(const ImageT<T2,C2>& other);

    // Cast --------------------------------------------------------------------
public:
    template<class ImageT2>
    inline ImageT2 cast() const;

    template<typename T2, int C2>
    inline ImageT<T2,C2> cast() const;

    template<typename T2>
    inline ImageT<T2,C> cast() const;

    template<int C2>
    inline ImageT<T,C2> cast() const;

    // Capacity ----------------------------------------------------------------
public:
    inline bool empty();
    inline int height() const;
    inline int width() const;
    static constexpr int depth();
    inline int size() const;
    inline int capacity() const;

    // Accessors ---------------------------------------------------------------
public:
    inline      ColorAccess operator()(int i, int j);
    inline ConstColorAccess operator()(int i, int j) const;

    inline      ColorAccess operator()(int k);
    inline ConstColorAccess operator()(int k) const;

    inline const std::vector<T>& data() const;
    inline       std::vector<T>& data();

    inline const T* raw() const;
    inline       T* raw();

    inline ConstColorAccess eval(float u, float v) const;

    inline ConstMatrixMap as_matrix() const;
    inline      MatrixMap as_matrix();

    //TODO as_tensor()

    // Modifiers ---------------------------------------------------------------
public:
    inline void clear();
    inline void resize(int height, int width);
    inline void fill(const T& value);
    inline void fill(const Color& color);

    // Internal ----------------------------------------------------------------
protected:
    inline const T* at(int i, int j) const;
    inline       T* at(int i, int j);

    inline const T* at(int k) const;
    inline       T* at(int k);

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

namespace internal {

template<typename T> constexpr T channel_one();
template<> constexpr int    channel_one<int   >() {return 255;}
template<> constexpr float  channel_one<float >() {return 1.f;}
template<> constexpr double channel_one<double>() {return 1.;}

template<typename TFrom, typename TTo>
inline TTo cast_channel(TFrom val) {return val;}
template<> inline int    cast_channel(float val) {return int(255.f * val);}
template<> inline int    cast_channel(double val){return int(255.  * val);}
template<> inline float  cast_channel(int val)   {return float(val)  / 255.f;}
template<> inline double cast_channel(int val)   {return double(val) / 255.;}
template<> inline float  cast_channel(unsigned char val)   {return float(val)  / 255.f;}
template<> inline double cast_channel(unsigned char val)   {return double(val) / 255.;}

// use struct since partial specialization are not allowed for functions
template<typename TFrom, typename TTo> struct Average {
    static TTo compute(TFrom r, TFrom g, TFrom b) {
        return (r + g + b) / TFrom(3);
    }
};

template<typename TFrom> struct Average<TFrom,int> {
    static int compute(TFrom r, TFrom g, TFrom b)
    {
        return cast_channel<TFrom,int>((r + g + b) / TFrom(3));
    }
};

template<typename TTo> struct Average<int,TTo> {
    static TTo compute(int r, int g, int b) {
        return double(r + g + b) / 3. / 255.;
    }
};

template<> struct Average<int,int> {
    static int compute(int r, int g, int b) {
        return int(std::round(double(r + g + b) / 3.));
    }
};

template<typename TFrom, int CFrom, typename TTo, int CTo>
struct DefaultCaster {
    typename ImageT<TTo,CTo>::Color operator()(
      const typename ImageT<TFrom,CFrom>::ConstColorAccess& c);
};

//! \brief RGBA to G
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,4,TTo,1> {
    auto operator()(const typename ImageT<TFrom,4>::ConstColorAccess& c) {
        return typename ImageT<TTo,1>::Color(
              Average<TFrom,TTo>::compute(c[0],c[1],c[2]));
    }
};

//! \brief RGB to G
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,3,TTo,1> {
    auto operator()(const typename ImageT<TFrom,3>::ConstColorAccess& c) {
        return typename ImageT<TTo,1>::Color(
              Average<TFrom,TTo>::compute(c[0],c[1],c[2]));
    }
};

//! \brief GA to G
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,2,TTo,1> {
    auto operator()(const typename ImageT<TFrom,2>::ConstColorAccess& c) {
        return typename ImageT<TTo,1>::Color(cast_channel<TFrom,TTo>(c[0]));
    }
};

//! \brief RGBA to GA
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,4,TTo,2> {
    auto operator()(const typename ImageT<TFrom,4>::ConstColorAccess& c) {
        return typename ImageT<TTo,2>::Color(
              Average<TFrom,TTo>::compute(c[0],c[1],c[2]),
              cast_channel<TFrom,TTo>(c[3]));
    }
};

//! \brief RGB to GA
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,3,TTo,2> {
    auto operator()(const typename ImageT<TFrom,3>::ConstColorAccess& c) {
        return typename ImageT<TTo,2>::Color(
              Average<TFrom,TTo>::compute(c[0],c[1],c[2]), channel_one<TTo>());
    }
};

//! \brief G to GA
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,1,TTo,2> {
    auto operator()(const typename ImageT<TFrom,1>::ConstColorAccess& c) {
        return typename ImageT<TTo,2>::Color(
              cast_channel<TFrom,TTo>(c[0]), channel_one<TTo>());
    }
};

//! \brief RGBA to RGB
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,4,TTo,3> {
    auto operator()(const typename ImageT<TFrom,4>::ConstColorAccess& c) {
        return typename ImageT<TTo,3>::Color(
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[1]),
              cast_channel<TFrom,TTo>(c[2]));
    }
};

//! \brief GA to RGB
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,2,TTo,3> {
    auto operator()(const typename ImageT<TFrom,2>::ConstColorAccess& c) {
        return typename ImageT<TTo,3>::Color(
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[0]));
    }
};

//! \brief G to RGB
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,1,TTo,3> {
    auto operator()(const typename ImageT<TFrom,1>::ConstColorAccess& c) {
      return typename ImageT<TTo,3>::Color(
            cast_channel<TFrom,TTo>(c[0]),
            cast_channel<TFrom,TTo>(c[0]),
            cast_channel<TFrom,TTo>(c[0]));
    }
};

//! \brief RGB to RGBA
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,3,TTo,4> {
    auto operator()(const typename ImageT<TFrom,3>::ConstColorAccess& c) {
        return typename ImageT<TTo,4>::Color(
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[1]),
              cast_channel<TFrom,TTo>(c[2]),
              channel_one<TTo>());
    }
};

//! \brief GA to RGBA
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,2,TTo,4> {
    auto operator()(const typename ImageT<TFrom,2>::ConstColorAccess& c) {
        return typename ImageT<TTo,4>::Color(
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[1]));
    }
};

//! \brief G to RGBA
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,1,TTo,4> {
    auto operator()(const typename ImageT<TFrom,1>::ConstColorAccess& c) {
        return typename ImageT<TTo,4>::Color(
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[0]),
              cast_channel<TFrom,TTo>(c[0]),
              channel_one<TTo>());
    }
};

template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,1,TTo,1> {
    auto operator()(const typename ImageT<TFrom,1>::ConstColorAccess& c) {
        return typename ImageT<TTo,1>::Color(cast_channel<TFrom,TTo>(c[0]));
    }
};
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,2,TTo,2> {
    auto operator()(const typename ImageT<TFrom,2>::ConstColorAccess& c) {
        return typename ImageT<TTo,2>::Color(cast_channel<TFrom,TTo>(c[0]),
                                             cast_channel<TFrom,TTo>(c[1]));
    }
};
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,3,TTo,3> {
    auto operator()(const typename ImageT<TFrom,3>::ConstColorAccess& c) {
        return typename ImageT<TTo,3>::Color(cast_channel<TFrom,TTo>(c[0]),
                                             cast_channel<TFrom,TTo>(c[1]),
                                             cast_channel<TFrom,TTo>(c[2]));
    }
};
template<typename TFrom, typename TTo> struct DefaultCaster<TFrom,4,TTo,4> {
    auto operator()(const typename ImageT<TFrom,4>::ConstColorAccess& c) {
        return typename ImageT<TTo,4>::Color(cast_channel<TFrom,TTo>(c[0]),
                                             cast_channel<TFrom,TTo>(c[1]),
                                             cast_channel<TFrom,TTo>(c[2]),
                                             cast_channel<TFrom,TTo>(c[3]));
    }
};

} // namespace internal

// cast ------------------------------------------------------------------------

template<typename TFrom, int CFrom, typename TTo, int CTo, class Caster>
void cast(const ImageT<TFrom, CFrom>& from, ImageT<TTo, CTo>& to, Caster&& caster)
{
    to.resize(from.height(), from.width());
    //TODO omp ?
    for (int k = 0; k < from.size(); ++k)
    {
        to(k) = caster(from(k));
    }
}

template<typename TFrom, int CFrom, typename TTo, int CTo>
void cast(const ImageT<TFrom, CFrom>& from, ImageT<TTo, CTo>& to)
{
    cast(from, to, internal::DefaultCaster<TFrom, CFrom, TTo, CTo>());
}

// io --------------------------------------------------------------------------

template<typename T, int C>
bool save(const std::string& filename, const ImageT<T,C>& image)
{
    std::ofstream ofs(filename);
    if(!ofs.is_open()) return false;

    std::abort(); //TODO

    return true;
}

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

//!
//! \brief used for io operations
//! \warning data must point to an array of size height*size*depth
//! \todo could be optimized since cast is used
//!
template<typename T, int C>
ImageT<T,C>::ImageT(int height, int width, unsigned char* data, int depth) : ImageT(height, width)
{
    assert(0 < depth && depth <= 4);

         if(depth == 1) *this = ImageT<T,1>(height, width, data);
    else if(depth == 2) *this = ImageT<T,2>(height, width, data);
    else if(depth == 3) *this = ImageT<T,3>(height, width, data);
    else if(depth == 4) *this = ImageT<T,4>(height, width, data);
}

//!
//! \brief used for io operations
//! \warning data must point to an array of size height*size*C
//!
template<typename T, int C>
ImageT<T,C>::ImageT(int height, int width, unsigned char* data) : ImageT(height, width)
{
    for(int k = 0; k < height * width * C; ++k)
    {
        m_data[k] = internal::cast_channel<unsigned char,T>(data[k]);
    }
}

template<typename T, int C>
template<typename T2, int C2>
ImageT<T,C>::ImageT(const ImageT<T2,C2>& other)  :
    m_height(other.height()),
    m_width(other.width()),
    m_data(C * other.width() * other.height())
{
    img::cast(other, *this);
}

template<typename T, int C>
template<typename T2, int C2>
ImageT<T,C>& ImageT<T,C>::operator=(const ImageT<T2,C2>& other)
{
    img::cast(other, *this);
    return *this;
}

// Cast ------------------------------------------------------------------------

template<typename T, int C>
template<class ImageT2>
ImageT2 ImageT<T,C>::cast() const
{
    return ImageT2(*this);
}

template<typename T, int C>
template<typename T2, int C2>
ImageT<T2,C2> ImageT<T,C>::cast() const
{
    return ImageT<T2,C2>(*this);
}

template<typename T, int C>
template<typename T2>
ImageT<T2,C> ImageT<T,C>::cast() const
{
    return ImageT<T2,C>(*this);
}

template<typename T, int C>
template<int C2>
ImageT<T,C2> ImageT<T,C>::cast() const
{
    return ImageT<T,C2>(*this);
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
constexpr int ImageT<T,C>::depth()
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
typename ImageT<T,C>::ColorAccess ImageT<T,C>::operator()(int k)
{
    return ColorAccess(at(k));
}

template<typename T, int C>
typename ImageT<T,C>::ConstColorAccess ImageT<T,C>::operator()(int k) const
{
    return ConstColorAccess(at(k));
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

template<typename T, int C>
typename ImageT<T,C>::ConstMatrixMap ImageT<T,C>::as_matrix() const
{
    std::abort(); //TODO
}

template<typename T, int C>
typename ImageT<T,C>::MatrixMap ImageT<T,C>::as_matrix()
{
    std::abort(); //TODO
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
const T* ImageT<T,C>::at(int i, int j) const
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return &m_data[index(i,j)];
}

template<typename T, int C>
T* ImageT<T,C>::at(int i, int j)
{
    assert(0 <= i && i < height() && 0 <= j && j <= width());
    return &m_data[index(i,j)];
}

template<typename T, int C>
const T* ImageT<T,C>::at(int k) const
{
    assert(0 <= k && k < height() * width());
    return &m_data[C * k];
}

template<typename T, int C>
T* ImageT<T,C>::at(int k)
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

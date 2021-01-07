#include "Image.h"

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "internal/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "internal/stb_image_write.h"

namespace pcp {

// Image -----------------------------------------------------------------------

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

// IO --------------------------------------------------------------------------

bool Image::load(const std::string& filename, bool flip)
{
    this->clear();

    int channel = 0;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(filename.c_str(), &m_width, &m_height, &channel, 4);

    if(data == nullptr) return false;

    resize(m_height, m_width);

    for(int k=0; k<capacity(); ++k)
        m_data[k] = float(data[k]) / 255;

    free(data);

    return true;
}

bool Image::save(const std::string& filename, bool flip) const
{
    char* data = (char*)malloc(capacity()*sizeof(char));
    for(int k=0; k<capacity(); ++k)
        data[k] = 255*m_data[k];

    stbi_flip_vertically_on_write(flip);
    int ok = stbi_write_png(filename.c_str(), m_width, m_height, 4, data, 0);

    free(data);

    return ok;
}

bool Image::load_bin(const std::string& filename)
{
    std::ifstream ifs(filename);
    if(!ifs.is_open()) return false;

    ifs.read(reinterpret_cast<char*>(&m_height), sizeof(int));
    ifs.read(reinterpret_cast<char*>(&m_width),  sizeof(int));

    resize(m_height, m_width);

    ifs.read(reinterpret_cast<char*>(m_data), 4 * m_height * m_width * sizeof(float));

    return true;
}

bool Image::save_bin(const std::string& filename) const
{
    std::ofstream ofs(filename);
    if(!ofs.is_open()) return false;

    ofs.write(reinterpret_cast<const char*>(&m_height), sizeof(int));
    ofs.write(reinterpret_cast<const char*>(&m_width), sizeof(int));
    ofs.write(reinterpret_cast<const char*>(m_data), 4 * m_height * m_width * sizeof(float));

    return true;
}

// Accessors -------------------------------------------------------------------

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

} // namespace pcp

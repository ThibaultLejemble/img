#include <img/GrayScaleImage.h>
#include <img/Image.h>
#include <img/BinaryImage.h>

#include <fstream>

//#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <img/stb/stb_image.h>
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <img/stb/stb_image_write.h>

namespace img {

// GrayScaleImage --------------------------------------------------------------

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

GrayScaleImage::GrayScaleImage(const Image& other) : GrayScaleImage()
{
    this->resize(other.height(), other.width());

    for(int k=0; k<capacity(); ++k)
    {
        this->operator()(k) = other(k).head<3>().sum()/3;
    }
}

GrayScaleImage& GrayScaleImage::operator = (const Image& other)
{
    this->resize(other.height(), other.width());

    for(int k=0; k<size(); ++k)
    {
        this->operator()(k) = other(k).head<3>().sum()/3;
    }
    return *this;
}

GrayScaleImage::GrayScaleImage(const BinaryImage& other) : GrayScaleImage()
{
    this->resize(other.height(), other.width());

    for(int k=0; k<capacity(); ++k)
    {
        this->operator()(k) = other(k) ? 1 : 0;
    }
}

GrayScaleImage& GrayScaleImage::operator = (const BinaryImage& other)
{
    this->resize(other.height(), other.width());

    for(int k=0; k<size(); ++k)
    {
        this->operator()(k) = other(k) ? 1 : 0;
    }
    return *this;
}

GrayScaleImage::~GrayScaleImage()
{
    this->clear();
}

// IO --------------------------------------------------------------------------

bool GrayScaleImage::load(const std::string& filename, bool flip)
{
    this->clear();

    int channel = 0;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(filename.c_str(), &m_width, &m_height, &channel, 1);

    if(data == nullptr) return false;

    resize(m_height, m_width);

    for(int k=0; k<capacity(); ++k)
        m_data[k] = float(data[k]) / 255;

    free(data);

    return true;
}

bool GrayScaleImage::save(const std::string& filename, bool flip) const
{
    char* data = (char*)malloc(capacity()*sizeof(char));
    for(int k=0; k<capacity(); ++k)
        data[k] = 255*m_data[k];

    stbi_flip_vertically_on_write(flip);
    int ok = stbi_write_png(filename.c_str(), m_width, m_height, 1, data, 0);

    free(data);

    return ok;
}

bool GrayScaleImage::load_bin(const std::string& filename)
{
    std::ifstream ifs(filename);
    if(!ifs.is_open()) return false;

    ifs.read(reinterpret_cast<char*>(&m_height), sizeof(int));
    ifs.read(reinterpret_cast<char*>(&m_width),  sizeof(int));

    resize(m_height, m_width);

    ifs.read(reinterpret_cast<char*>(m_data), 1 * m_height * m_width * sizeof(float));

    return true;
}

bool GrayScaleImage::save_bin(const std::string& filename) const
{
    std::ofstream ofs(filename);
    if(!ofs.is_open()) return false;

    ofs.write(reinterpret_cast<const char*>(&m_height), sizeof(int));
    ofs.write(reinterpret_cast<const char*>(&m_width), sizeof(int));
    ofs.write(reinterpret_cast<const char*>(m_data), 1 * m_height * m_width * sizeof(float));

    return true;
}

// Accessors -------------------------------------------------------------------

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

} // namespace img

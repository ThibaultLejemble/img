#include <img/IO/LabelImage_load.h>
#include <img/Image/LabelImage.h>

#include <fstream>

namespace img {

bool load(const std::string& filename, LabelImage& labels)
{
    labels.clear();

    std::ifstream ifs(filename);
    if(!ifs.is_open())
    {
        return false;
    }

    int width  = 0;
    int height = 0;

    ifs >> height;
    ifs >> width;

    if(height == 0 || width == 0) return false;

    labels.resize(height, width);

    for(int i=0; i<labels.height(); ++i)
    {
        for(int j=0; j<labels.width(); ++j)
        {
            ifs >> labels(i,j);
        }
    }

    return true;
}

} // namespace img

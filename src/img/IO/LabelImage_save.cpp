#include <img/IO/LabelImage_save.h>
#include <img/Image/LabelImage.h>

#include <fstream>

namespace img {

bool save(const std::string& filename, const LabelImage& labels)
{
    std::ofstream ofs(filename);
    if(!ofs.is_open())
    {
        return false;
    }

    ofs << labels.height() << ' ' << labels.width() << '\n';
    for(int i=0; i<labels.height(); ++i)
    {
        for(int j=0; j<labels.width(); ++j)
        {
            ofs << labels(i,j) << ' ';
        }
        ofs << '\n';
    }

    return true;
}

} // namespace img

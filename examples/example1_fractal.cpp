#include <img/ImageT.h>

using namespace img;

int main()
{
    ImageRGBf image(610,1080);

    const auto black = ImageRGBf::Color(0,0,0);
    const auto green = ImageRGBf::Color(0,1,0);
    const auto white = ImageRGBf::Color(1,1,1);

    constexpr auto max_iteration = 100;

    for(int i=0; i<image.height(); ++i)
    {
        for(int j=0; j<image.width(); ++j)
        {
            const auto x0 = float(j)/(image.width()-1)  * 3.5f - 2.5f;
            const auto y0 = float(i)/(image.height()-1) * 2.0f - 1.0f;
            auto x = 0.f;
            auto y = 0.f;
            auto iteration = 0;
            while(x*x + y*y <= 4 && iteration < max_iteration)
            {
                const auto xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                ++iteration;
            }
            if(iteration == max_iteration)
            {
                image(i,j) = white;
            }
            else
            {
                const auto t = float(iteration)/max_iteration;
                image(i,j) = (1.f-t)*black + t*green;
            }
        }
    }

    // save image
    const bool ok = save("example1_fractal.png", image);
    return !ok;
}

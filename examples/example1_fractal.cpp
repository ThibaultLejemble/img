#include <img/Image/Image.h>
#include <img/IO/Image_save.h>

using namespace img;

int main()
{
    Image image(610,1080);

    const Image::Color black(0,0,0,1);
    const Image::Color green(0,1,0,1);
    const Image::Color white(1,1,1,1);
    int max_iteration = 100;
    for(int i=0; i<image.height(); ++i)
    {
        for(int j=0; j<image.width(); ++j)
        {
            const float x0 = float(j)/(image.width()-1)  * 3.5f - 2.5f;
            const float y0 = float(i)/(image.height()-1) * 2.0f - 1.0f;
            float x = 0.f;
            float y = 0.f;
            int iteration = 0;
            while(x*x + y*y <= 4 && iteration < max_iteration)
            {
                const float xtemp = x*x - y*y + x0;
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
                const float t = float(iteration)/max_iteration;
                image(i,j) = (1.f-t)*black + t*green;
            }
        }
    }

    // save image
    const bool ok = save("example1_fractal.png", image);
    return !ok;
}

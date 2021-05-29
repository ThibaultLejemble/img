#include <img/Image/ImageT.h>
#include <img/IO/ImageT.h>

#include <iostream>

using namespace img;

int main()
{
    constexpr auto N = 256;
    ImageRGBAd image(N,N);
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            const double u = double(i)/(image.height()-1);
            const double v = double(j)/(image.width()-1);

            const double r = 0.5 * (1.0 + std::cos(M_PI * (1+1*v) * u));
            const double g = 0.5 * (1.0 + std::cos(M_PI * (1+2*u) * v));
            const double b = 0.5 * (1.0 + std::cos(M_PI * (1+4*v) * u));
            const double a = 0.5 * (1.0 + std::cos(M_PI * (1-u)*v));

            image(i,j)[0] = r;
            image(i,j)[1] = g;
            image(i,j)[2] = b;
            image(i,j)[3] = a;
        }
    }

    io::save("exmaple0_Gi.png",    ImageGi   (image));
    io::save("exmaple0_Gf.png",    ImageGf   (image));
    io::save("exmaple0_Gd.png",    ImageGd   (image));
    io::save("exmaple0_GAi.png",   ImageGAi  (image));
    io::save("exmaple0_GAf.png",   ImageGAf  (image));
    io::save("exmaple0_GAd.png",   ImageGAd  (image));
    io::save("exmaple0_RGBi.png",  ImageRGBi (image));
    io::save("exmaple0_RGBf.png",  ImageRGBf (image));
    io::save("exmaple0_RGBd.png",  ImageRGBd (image));
    io::save("exmaple0_RGBAi.png", ImageRGBAi(image));
    io::save("exmaple0_RGBAf.png", ImageRGBAf(image));
    io::save("exmaple0_RGBAd.png", ImageRGBAd(image));

    return 1;
}
